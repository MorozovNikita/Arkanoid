#pragma once

#include "State.h"
#include "Platform.h"
#include "Ball.h"
#include "Block.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Sound.hpp>

#include <memory>
#include <vector>

namespace Game
{
    class GameState : public State
    {
    public:
        GameState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        enum class Phase
        {
            Playing,
            Won,
            Lost
        };

        void spawnBlocks();
        void restart();
        void showResult(Phase phase);
        void refreshResult();
        bool blocksRemain() const;
        bool handleResultEvent(const sf::Event::KeyPressed& key);

        Platform mPlatform;
        Ball mBall;
        std::vector<std::unique_ptr<Block>> mBlocks;

        sf::Text mHint;
        sf::Text mResultTitle;
        sf::Text mResultQuestion;
        sf::Text mResultYes;
        sf::Text mResultNo;
        sf::RectangleShape mDimmer;
        sf::Sound mBubbleSound;
        sf::Sound mBonusSound;
        sf::Sound mGlassSound;
        sf::Sound mWallSound;

        Phase mPhase = Phase::Playing;
        int mResultChoice = 0;
    };
}
