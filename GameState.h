#pragma once

#include "State.h"
#include "Constants.h"
#include "Platform.h"
#include "Ball.h"
#include "Block.h"
#include "Bonus.h"
#include "IObserver.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>

#include <memory>
#include <vector>

namespace Game
{
    class GameState : public State, public IObserver, public BonusTarget, public std::enable_shared_from_this<GameState>
    {
    public:
        GameState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;
        void attached() override;
        void Notify(std::shared_ptr<IObservable> observable) override;

        void grantExtraBall() override;
        void grantWidePlatform() override;
        void grantExtraLife() override;

    private:
        enum class Phase
        {
            Playing,
            Won,
            Lost
        };

        void spawnBlocks();
        void subscribeAll();
        void beginPlay();
        void restart();
        void showResult(Phase phase);
        void refreshHud();
        void drawHud();
        void launchBalls();
        bool allBallsStuck() const;
        void resolveLostBalls();
        void spendLife();
        void updateBonuses(sf::Time dt);
        void updateWidePlatform(sf::Time dt);

        Platform mPlatform;
        std::vector<std::shared_ptr<Ball>> mBalls;
        std::vector<std::shared_ptr<Ball>> mPendingLostBalls;
        std::vector<std::shared_ptr<Block>> mBlocks;
        std::vector<std::unique_ptr<Bonus>> mBonuses;
        int mBreakableBlocksLeft = 0;
        int mScore = 0;
        int mLives = STARTING_LIVES;
        float mWideTime = 0.f;

        sf::Text mHint;
        sf::Text mScoreText;
        sf::Sound mBubbleSound;
        sf::Sound mBonusSound;
        sf::Sound mGlassSound;
        sf::Sound mWallSound;

        Phase mPhase = Phase::Playing;
    };
}
