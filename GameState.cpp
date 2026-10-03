#include "GameState.h"

#include "Constants.h"
#include "ResourceHolder.h"
#include "Settings.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Game
{
    namespace
    {
        void centerText(sf::Text& text, sf::Vector2f position)
        {
            const auto bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f);
            text.setPosition(position);
        }
    }

    GameState::GameState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mPlatform()
        , mBall()
        , mHint(context.fonts.get(Fonts::Main), ""s, 14)
        , mResultTitle(context.fonts.get(Fonts::Main), "Congratulations!"s, 28)
        , mResultQuestion(context.fonts.get(Fonts::Main), "Play again?"s, 18)
        , mResultYes(context.fonts.get(Fonts::Main), "Yes"s, 22)
        , mResultNo(context.fonts.get(Fonts::Main), "No"s, 22)
        , mDimmer({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) })
        , mBubbleSound(context.soundBuffers.get(SoundEffects::Bubble))
        , mBonusSound(context.soundBuffers.get(SoundEffects::Bonus))
    {
        const bool mouse = context.settings.control == ControlMode::Mouse;
        mHint.setString(mouse ? "Click or Space - launch"s : "Space - launch"s);
        mHint.setFillColor(sf::Color(180, 180, 180));

        const auto bounds = mHint.getLocalBounds();
        mHint.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y });
        mHint.setPosition({ SCREEN_WIDTH / 2.f, 16.f });

        mDimmer.setFillColor(sf::Color(0, 0, 0, 160));
        mResultQuestion.setFillColor(sf::Color(200, 200, 200));

        spawnBlocks();
        mBall.attachTo(mPlatform);
        refreshResult();
    }

    void GameState::spawnBlocks()
    {
        mBlocks.clear();

        const float gridWidth = BLOCK_COLUMNS * BLOCK_WIDTH + (BLOCK_COLUMNS - 1) * BLOCK_GAP_X;
        const float startX = (static_cast<float>(SCREEN_WIDTH) - gridWidth) / 2.f;
        const sf::Color rowColors[] = {
            sf::Color(220, 82, 86),
            sf::Color(240, 168, 64),
            sf::Color(86, 176, 214),
        };

        for (int row = 0; row < BLOCK_ROWS; ++row)
        {
            for (int column = 0; column < BLOCK_COLUMNS; ++column)
            {
                const sf::Vector2f position{
                    startX + column * (BLOCK_WIDTH + BLOCK_GAP_X),
                    BLOCK_FIELD_TOP + row * (BLOCK_HEIGHT + BLOCK_GAP_Y)
                };
                mBlocks.emplace_back(position, rowColors[row]);
            }
        }
    }

    void GameState::restart()
    {
        mPlatform = Platform();
        mBall = Ball();
        mBall.attachTo(mPlatform);
        spawnBlocks();
        mPhase = Phase::Playing;
        mResultChoice = 0;
        refreshResult();
    }

    void GameState::showResult(Phase phase)
    {
        mPhase = phase;
        mResultChoice = 0;
        mResultTitle.setString(phase == Phase::Won ? "Congratulations!"s : "You lose"s);
        refreshResult();
    }

    void GameState::refreshResult()
    {
        const float centerX = SCREEN_WIDTH / 2.f;
        centerText(mResultTitle, { centerX, 210.f });
        centerText(mResultQuestion, { centerX, 280.f });
        centerText(mResultYes, { centerX, 350.f });
        centerText(mResultNo, { centerX, 410.f });

        const sf::Color selected = sf::Color::Green;
        const sf::Color idle = sf::Color::White;
        mResultTitle.setFillColor(sf::Color::White);
        mResultYes.setFillColor(mResultChoice == 0 ? selected : idle);
        mResultNo.setFillColor(mResultChoice == 1 ? selected : idle);
    }

    bool GameState::blocksRemain() const
    {
        for (const Block& block : mBlocks)
        {
            if (block.isAlive())
                return true;
        }
        return false;
    }

    void GameState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(24, 28, 36));

        for (const Block& block : mBlocks)
            block.draw(window);

        mPlatform.draw(window);
        mBall.draw(window);

        if (mPhase == Phase::Playing && mBall.isStuck())
            window.draw(mHint);

        if (mPhase == Phase::Playing)
            return;

        window.draw(mDimmer);
        window.draw(mResultTitle);
        window.draw(mResultQuestion);
        window.draw(mResultYes);
        window.draw(mResultNo);
    }

    bool GameState::update(sf::Time dt)
    {
        if (mPhase != Phase::Playing)
            return false;

        const auto context = getContext();
        mPlatform.update(dt, context.window, context.settings);
        mBall.update(dt, mPlatform);

        if (mBall.takeBounce())
            mBubbleSound.play();

        if (mBall.hasFallen())
        {
            showResult(Phase::Lost);
            return false;
        }

        for (Block& block : mBlocks)
        {
            block.update(dt);
            if (block.isAlive() && mBall.bounceFrom(block))
            {
                block.destroy();
                mBonusSound.play();
                break;
            }
        }

        if (!blocksRemain())
            showResult(Phase::Won);

        return false;
    }

    bool GameState::handleResultEvent(const sf::Event::KeyPressed& key)
    {
        const auto& input = getContext().settings.input;
        if (key.code == input.menuUp || key.code == input.menuDown)
        {
            mResultChoice = 1 - mResultChoice;
            refreshResult();
        }
        else if (key.code == input.confirm || key.code == input.pause)
        {
            if (mResultChoice == 0)
                restart();
            else
                requestStackPop();
        }
        else if (key.code == input.back)
        {
            requestStackPop();
        }

        return false;
    }

    bool GameState::handleEvent(const sf::Event& event)
    {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (mPhase != Phase::Playing)
                return handleResultEvent(*keyPressed);

            const auto& input = getContext().settings.input;
            if (keyPressed->code == input.pause)
            {
                if (mBall.isStuck())
                    mBall.launch();
                else
                    requestStackPush(States::Pause);
            }
            else if (keyPressed->code == input.back)
            {
                requestStackPop();
            }

            return false;
        }

        if (mPhase != Phase::Playing)
            return false;

        if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left && mBall.isStuck())
                mBall.launch();

            return false;
        }

        return true;
    }
}
