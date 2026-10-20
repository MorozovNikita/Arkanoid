#pragma once

#include "GameObject.h"
#include "Settings.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

namespace sf
{
    class RenderWindow;
}

namespace Game
{
    class Platform : public GameObject
    {
    public:
        Platform();

        void update(sf::Time dt, const sf::RenderWindow& window, const Settings& settings);
        void setWidth(float width);

    private:
        void clampToField();

        sf::Shape& shape() override;
        const sf::Shape& shape() const override;

        sf::RectangleShape mShape;
    };
}
