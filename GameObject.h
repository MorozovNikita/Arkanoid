#pragma once

#include <SFML/Graphics/Rect.hpp>

namespace sf
{
    class RenderTarget;
    class Shape;
}

namespace Game
{
    class GameObject
    {
    public:
        virtual ~GameObject() = default;

        void draw(sf::RenderTarget& target) const;
        sf::FloatRect bounds() const;

        bool isAlive() const { return mAlive; }
        void destroy() { mAlive = false; }

    protected:
        virtual sf::Shape& shape() = 0;
        virtual const sf::Shape& shape() const = 0;

    private:
        bool mAlive = true;
    };
}
