#include "GameState.h"

#include "BonusFactory.h"
#include "Constants.h"
#include "Heart.h"
#include "Leaderboard.h"
#include "Level.h"
#include "ResourceHolder.h"
#include "Settings.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#include <algorithm>

namespace Game
{
    GameState::GameState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mPlatform()
        , mHint(context.fonts.get(Fonts::Main), ""s, 14)
        , mScoreText(context.fonts.get(Fonts::Main), "Score 0"s, 12)
        , mBubbleSound(context.soundBuffers.get(SoundEffects::Bubble))
        , mBonusSound(context.soundBuffers.get(SoundEffects::Bonus))
        , mGlassSound(context.soundBuffers.get(SoundEffects::Glass))
        , mWallSound(context.soundBuffers.get(SoundEffects::Wall))
    {
        const bool mouse = context.settings.control == ControlMode::Mouse;
        mHint.setString(mouse ? "Click or Space - launch"s : "Space - launch"s);
        mHint.setFillColor(sf::Color(180, 180, 180));

        const auto bounds = mHint.getLocalBounds();
        mHint.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y });
        mHint.setPosition({ SCREEN_WIDTH / 2.f, 16.f });

        mScoreText.setFillColor(sf::Color(255, 214, 96));

        mBalls.push_back(std::make_shared<Ball>());
        mBalls.front()->attachTo(mPlatform);
        spawnBlocks();
        refreshHud();
    }

    void GameState::attached()
    {
        beginPlay();
    }

    void GameState::Notify(std::shared_ptr<IObservable> observable)
    {
        if (mPhase != Phase::Playing)
            return;

        if (const auto block = std::dynamic_pointer_cast<Block>(observable))
        {
            mScore += block->score();
            refreshHud();

            if (auto bonus = tryCreateBonus(block->bounds().getCenter()))
                mBonuses.push_back(std::move(bonus));

            if (mBreakableBlocksLeft > 0)
                --mBreakableBlocksLeft;

            if (mBreakableBlocksLeft == 0)
                showResult(Phase::Won);
        }
        else if (const auto ball = std::dynamic_pointer_cast<Ball>(observable))
        {
            if (!ball->hasFallen())
                return;

            const auto already = std::find(mPendingLostBalls.begin(), mPendingLostBalls.end(), ball);
            if (already == mPendingLostBalls.end())
                mPendingLostBalls.push_back(ball);
        }
    }

    void GameState::grantExtraBall()
    {
        auto ball = std::make_shared<Ball>();
        const auto flying = std::find_if(mBalls.begin(), mBalls.end(), [](const auto& item)
        {
            return !item->hasFallen() && !item->isStuck();
        });

        if (flying != mBalls.end())
            ball->splitFrom(**flying);
        else
            ball->attachTo(mPlatform, 26.f * static_cast<float>(mBalls.size()));

        ball->AddObserver(weak_from_this());
        mBalls.push_back(std::move(ball));
    }

    void GameState::grantWidePlatform()
    {
        mWideTime = WIDE_PLATFORM_TIME;
        mPlatform.setWidth(PLATFORM_WIDTH * WIDE_PLATFORM_SCALE);
    }

    void GameState::grantExtraLife()
    {
        ++mLives;
        refreshHud();
    }

    void GameState::spawnBlocks()
    {
        mBlocks = getContext().level.createBlocks();
        mBreakableBlocksLeft = 0;
        for (const auto& block : mBlocks)
        {
            if (block->mustBeCleared())
                ++mBreakableBlocksLeft;
        }
    }

    void GameState::subscribeAll()
    {
        for (const auto& ball : mBalls)
            ball->AddObserver(weak_from_this());

        for (const auto& block : mBlocks)
            block->AddObserver(weak_from_this());
    }

    void GameState::beginPlay()
    {
        subscribeAll();
        if (mBreakableBlocksLeft == 0)
            showResult(Phase::Won);
    }

    void GameState::restart()
    {
        mPlatform = Platform();
        mWideTime = 0.f;
        mScore = 0;
        mLives = STARTING_LIVES;
        mBonuses.clear();
        mPendingLostBalls.clear();
        mBalls.clear();

        auto ball = std::make_shared<Ball>();
        ball->attachTo(mPlatform);
        mBalls.push_back(std::move(ball));

        spawnBlocks();
        mPhase = Phase::Playing;
        refreshHud();
        beginPlay();
    }

    void GameState::showResult(Phase phase)
    {
        if (mPhase != Phase::Playing)
            return;

        mPhase = phase;

        SessionResult result;
        result.score = mScore;
        result.won = phase == Phase::Won;
        getContext().leaderboard.onSessionFinished(result);
        requestStackPush(States::Leaderboard);
    }

    void GameState::refreshHud()
    {
        mScoreText.setString("Score " + std::to_string(mScore));
        const auto bounds = mScoreText.getLocalBounds();
        mScoreText.setOrigin({ bounds.position.x + bounds.size.x, bounds.position.y });
        mScoreText.setPosition({ static_cast<float>(SCREEN_WIDTH) - 16.f, 16.f });
    }

    void GameState::drawHud()
    {
        auto& window = getContext().window;
        window.draw(mScoreText);

        constexpr float heartSize = 16.f;
        constexpr float gap = 6.f;
        const float y = static_cast<float>(SCREEN_HEIGHT) - heartSize - 6.f;
        for (int life = 0; life < mLives; ++life)
        {
            const float x = 14.f + static_cast<float>(life) * (heartSize + gap);
            drawHeart(window, { x, y }, heartSize, sf::Color(230, 64, 84));
        }
    }

    void GameState::launchBalls()
    {
        bool mirrored = false;
        for (auto& ball : mBalls)
        {
            if (!ball->isStuck())
                continue;

            ball->launch(mirrored);
            mirrored = !mirrored;
        }
    }

    bool GameState::allBallsStuck() const
    {
        return !mBalls.empty() && std::all_of(mBalls.begin(), mBalls.end(), [](const auto& ball)
        {
            return ball->isStuck();
        });
    }

    void GameState::resolveLostBalls()
    {
        if (mPendingLostBalls.empty())
            return;

        for (const auto& lost : mPendingLostBalls)
        {
            std::erase_if(mBalls, [&lost](const std::shared_ptr<Ball>& ball)
            {
                return ball == lost;
            });
        }
        mPendingLostBalls.clear();

        if (!mBalls.empty() || mPhase != Phase::Playing)
            return;

        spendLife();
    }

    void GameState::spendLife()
    {
        if (mLives > 0)
            --mLives;
        refreshHud();

        if (mLives <= 0)
        {
            showResult(Phase::Lost);
            return;
        }

        auto ball = std::make_shared<Ball>();
        ball->attachTo(mPlatform);
        ball->AddObserver(weak_from_this());
        mBalls.push_back(std::move(ball));
    }

    void GameState::updateWidePlatform(sf::Time dt)
    {
        if (mWideTime <= 0.f)
            return;

        mWideTime -= dt.asSeconds();
        if (mWideTime > 0.f)
            return;

        mWideTime = 0.f;
        mPlatform.setWidth(PLATFORM_WIDTH);
    }

    void GameState::updateBonuses(sf::Time dt)
    {
        for (auto& bonus : mBonuses)
            bonus->update(dt);

        for (auto& bonus : mBonuses)
        {
            if (!bonus->isAlive())
                continue;

            if (bonus->caughtBy(mPlatform))
            {
                bonus->apply(*this);
                bonus->destroy();
                mBonusSound.play();
            }
            else if (bonus->missed(mPlatform))
            {
                bonus->destroy();
            }
        }

        std::erase_if(mBonuses, [](const std::unique_ptr<Bonus>& bonus)
        {
            return !bonus->isAlive();
        });
    }

    void GameState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(24, 28, 36));

        for (const auto& block : mBlocks)
            block->draw(window);

        for (const auto& bonus : mBonuses)
            bonus->draw(window);

        mPlatform.draw(window);

        for (const auto& ball : mBalls)
            ball->draw(window);

        if (mPhase == Phase::Playing && allBallsStuck())
            window.draw(mHint);

        drawHud();
    }

    bool GameState::update(sf::Time dt)
    {
        if (mPhase != Phase::Playing)
            return false;

        const auto context = getContext();
        mPlatform.update(dt, context.window, context.settings);
        updateWidePlatform(dt);

        bool bounced = false;
        for (auto& ball : mBalls)
        {
            ball->update(dt, mPlatform);
            bounced = bounced || ball->takeBounce();
        }
        if (bounced)
            mBubbleSound.play();

        resolveLostBalls();
        if (mPhase != Phase::Playing)
            return false;

        updateBonuses(dt);
        if (mPhase != Phase::Playing)
            return false;

        for (auto& block : mBlocks)
            block->update(dt);

        if (mPhase != Phase::Playing)
            return false;

        for (auto& ball : mBalls)
        {
            if (ball->hasFallen() || ball->isStuck())
                continue;

            for (auto& block : mBlocks)
            {
                if (!block->canCollide())
                    continue;

                const bool hit = block->deflectsBall() ? ball->bounceFrom(*block) : ball->overlaps(*block);
                if (!hit)
                    continue;

                block->OnHit();
                switch (block->hitSound())
                {
                case HitSound::Glass:
                    mGlassSound.play();
                    break;
                case HitSound::Wall:
                    mWallSound.play();
                    break;
                case HitSound::Bonus:
                    mBonusSound.play();
                    break;
                }
                break;
            }

            if (mPhase != Phase::Playing)
                return false;
        }

        return false;
    }

    bool GameState::handleEvent(const sf::Event& event)
    {
        if (mPhase != Phase::Playing)
            return false;

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            const auto& input = getContext().settings.input;
            if (keyPressed->code == input.pause)
            {
                if (allBallsStuck())
                    launchBalls();
                else
                    requestStackPush(States::Pause);
            }
            else if (keyPressed->code == input.back)
            {
                requestStackPop();
            }

            return false;
        }

        if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left && allBallsStuck())
                launchBalls();

            return false;
        }

        return true;
    }
}
