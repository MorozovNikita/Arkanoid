#pragma once

#include "State.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <string>
#include <vector>

namespace Game
{
    class LevelSelectState : public State
    {
    public:
        LevelSelectState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        void refresh();
        void confirm();

        std::vector<std::string> mFileNames;
        std::vector<sf::Text> mLabels;
        int mSelected = 0;

        sf::Text mTitle;
        sf::Text mEmpty;
        sf::Text mError;
        sf::Text mHint;
        sf::VertexArray mArrow;
    };
}
