#pragma once

#include "GameObject.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace Game
{
    class Platform;

    class Ball : public GameObject
    {
    public:
        Ball();

        void update(sf::Time dt, const Platform& platform);
        void launch();
        void attachTo(const Platform& platform);

        bool bounceFrom(const GameObject& object);
        bool isStuck() const;
        bool hasFallen() const { return mFell; }
        bool takeBounce();

    private:
        sf::Shape& shape() override;
        const sf::Shape& shape() const override;

        sf::CircleShape mShape;
        sf::Vector2f mVelocity;
        bool mStuck = true;
        bool mFell = false;
        bool mBounced = false;
    };
}
