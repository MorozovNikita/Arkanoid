#pragma once

#include "Settings.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

namespace sf
{
    class RenderTarget;
    class RenderWindow;
}

namespace Game
{
    class Platform
    {
    public:
        Platform();

        void update(sf::Time dt, const sf::RenderWindow& window, const Settings& settings);
        void draw(sf::RenderTarget& target) const;

        sf::FloatRect bounds() const;

    private:
        void clampToField();

        sf::RectangleShape mShape;
    };
}
