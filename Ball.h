#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace sf
{
    class RenderTarget;
}

namespace Game
{
    class Platform;

    class Ball
    {
    public:
        Ball();

        void update(sf::Time dt, const Platform& platform);
        void draw(sf::RenderTarget& target) const;

        void launch();
        void attachTo(const Platform& platform);
        bool isStuck() const;

    private:
        sf::CircleShape mShape;
        sf::Vector2f mVelocity;
        bool mStuck = true;
    };
}
