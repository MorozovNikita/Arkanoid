#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace Game
{
    class Collidable
    {
    public:
        virtual ~Collidable() = default;

        virtual sf::FloatRect collisionBounds() const = 0;
        virtual bool canCollide() const = 0;
        virtual bool deflectsBall() const = 0;
    };
}
