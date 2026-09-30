#pragma once

#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Game
{
    class SettingsState : public State
    {
    public:
        SettingsState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        void toggleControl();
        void refreshVisuals();

        sf::Text mTitle;
        sf::Text mHint;
        sf::Text mLabel;
        sf::RectangleShape mBox;
        sf::RectangleShape mMark;
    };
}
