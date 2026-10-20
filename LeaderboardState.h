#pragma once

#include "State.h"

#include <SFML/Graphics/Text.hpp>

#include <string>
#include <vector>

namespace Game
{
    class LeaderboardState : public State
    {
    public:
        LeaderboardState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        enum class Mode
        {
            Browse,
            EnterName,
            Summary
        };

        void rebuild();
        void playAgain();
        void leaveToMenu();

        Mode mMode = Mode::Browse;
        int mHighlight = -1;
        std::string mName;

        sf::Text mTitle;
        sf::Text mScoreLine;
        sf::Text mNameLine;
        sf::Text mHint;
        std::vector<sf::Text> mRows;
    };
}
