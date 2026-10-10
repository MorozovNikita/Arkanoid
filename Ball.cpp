#include "Ball.h"

#include "Collidable.h"
#include "Constants.h"
#include "Platform.h"

#include <algorithm>
#include <cmath>
#include <numbers>

namespace Game
{
    namespace
    {
        constexpr float BounceSpread = 75.f * std::numbers::pi_v<float> / 180.f;
        constexpr float LaunchAngle = -70.f * std::numbers::pi_v<float> / 180.f;
    }

    Ball::Ball()
    {
        mShape.setRadius(BALL_RADIUS);
        mShape.setFillColor(sf::Color(255, 220, 70));
        mShape.setOutlineThickness(2.f);
        mShape.setOutlineColor(sf::Color(180, 120, 20));
    }

    void Ball::launch()
    {
        if (!mStuck)
            return;

        mStuck = false;
        mVelocity = 
        {
            std::cos(LaunchAngle) * BALL_SPEED,
            std::sin(LaunchAngle) * BALL_SPEED
        };
    }

    void Ball::attachTo(const Platform& platform)
    {
        mStuck = true;
        mVelocity = {};

        const sf::FloatRect platformBounds = platform.bounds();
        const float diameter = BALL_RADIUS * 2.f;
        mShape.setPosition({
            platformBounds.position.x + (platformBounds.size.x - diameter) / 2.f,
            platformBounds.position.y - diameter - mShape.getOutlineThickness()
        });
    }

    bool Ball::isStuck() const
    {
        return mStuck;
    }

    bool Ball::takeBounce()
    {
        const bool bounced = mBounced;
        mBounced = false;
        return bounced;
    }

    void Ball::update(sf::Time dt, const Platform& platform)
    {
        if (mStuck)
        {
            attachTo(platform);
            return;
        }

        sf::Vector2f position = mShape.getPosition();
        position += mVelocity * dt.asSeconds();
        mShape.setPosition(position);

        const float fieldWidth = static_cast<float>(SCREEN_WIDTH);
        const float fieldHeight = static_cast<float>(SCREEN_HEIGHT);
        const sf::FloatRect bounds = mShape.getGlobalBounds();
        bool bounced = false;

        if (bounds.position.x < 0.f)
        {
            position.x -= bounds.position.x;
            mVelocity.x = std::abs(mVelocity.x);
            bounced = true;
        }
        else if (bounds.position.x + bounds.size.x > fieldWidth)
        {
            position.x -= bounds.position.x + bounds.size.x - fieldWidth;
            mVelocity.x = -std::abs(mVelocity.x);
            bounced = true;
        }

        if (bounds.position.y < 0.f)
        {
            position.y -= bounds.position.y;
            mVelocity.y = std::abs(mVelocity.y);
            bounced = true;
        }
        else if (bounds.position.y + bounds.size.y > fieldHeight)
        {
            position.y -= bounds.position.y + bounds.size.y - fieldHeight;
            mShape.setPosition(position);
            mFell = true;
            mBounced = bounced;
            return;
        }

        mShape.setPosition(position);

        const sf::FloatRect ballBounds = mShape.getGlobalBounds();
        const sf::FloatRect platformBounds = platform.bounds();
        const auto hit = ballBounds.findIntersection(platformBounds);
        if (!hit)
        {
            mBounced = bounced;
            return;
        }

        mBounced = true;

        const sf::Vector2f ballCenter = ballBounds.getCenter();
        const sf::Vector2f platformCenter = platformBounds.getCenter();

        if (hit->size.x < hit->size.y)
        {
            position.x += (ballCenter.x < platformCenter.x) ? -hit->size.x : hit->size.x;
            mVelocity.x = (ballCenter.x < platformCenter.x) ? -std::abs(mVelocity.x) : std::abs(mVelocity.x);
            mShape.setPosition(position);
            return;
        }

        if (ballCenter.y >= platformCenter.y)
        {
            position.y += hit->size.y;
            mVelocity.y = std::abs(mVelocity.y);
            mShape.setPosition(position);
            return;
        }

        const float halfWidth = platformBounds.size.x / 2.f;
        const float offset = std::clamp((ballCenter.x - platformCenter.x) / halfWidth, -1.f, 1.f);
        const float angle = offset * BounceSpread;

        mVelocity = 
        {
            std::sin(angle) * BALL_SPEED,
            -std::cos(angle) * BALL_SPEED
        };

        position.y -= hit->size.y;
        mShape.setPosition(position);
    }

    bool Ball::overlaps(const Collidable& object) const
    {
        if (mStuck || mFell || !object.canCollide())
            return false;

        return bounds().findIntersection(object.collisionBounds()).has_value();
    }

    bool Ball::bounceFrom(const Collidable& object)
    {
        if (!overlaps(object))
            return false;

        const auto hit = bounds().findIntersection(object.collisionBounds());
        if (!hit)
            return false;

        sf::Vector2f position = mShape.getPosition();
        const sf::Vector2f ballCenter = bounds().getCenter();
        const sf::Vector2f objectCenter = object.collisionBounds().getCenter();

        if (hit->size.x < hit->size.y)
        {
            position.x += (ballCenter.x < objectCenter.x) ? -hit->size.x : hit->size.x;
            mVelocity.x = (ballCenter.x < objectCenter.x) ? -std::abs(mVelocity.x) : std::abs(mVelocity.x);
        }
        else if (ballCenter.y < objectCenter.y)
        {
            position.y -= hit->size.y;
            mVelocity.y = -std::abs(mVelocity.y);
        }
        else
        {
            position.y += hit->size.y;
            mVelocity.y = std::abs(mVelocity.y);
        }

        mShape.setPosition(position);
        return true;
    }

    sf::Shape& Ball::shape()
    {
        return mShape;
    }

    const sf::Shape& Ball::shape() const
    {
        return mShape;
    }
}
