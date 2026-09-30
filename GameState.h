#pragma once

#include "State.h"
#include "Platform.h"
#include "Ball.h"

#include <SFML/Graphics/Text.hpp>

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
        Platform mPlatform;
        Ball mBall;
        sf::Text mHint;
    };
}
