#pragma once

#include "GameObject.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

namespace Game
{
    class Block : public GameObject
    {
    public:
        Block(sf::Vector2f position, sf::Color color);

        void update(sf::Time dt);

    private:
        sf::Shape& shape() override;
        const sf::Shape& shape() const override;

        sf::RectangleShape mShape;
    };
}
