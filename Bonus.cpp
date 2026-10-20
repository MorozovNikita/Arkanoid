#include "Bonus.h"

#include "Constants.h"
#include "Heart.h"
#include "Platform.h"

#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{
    namespace
    {
        void centerOn(sf::Shape& shape, sf::Vector2f position, sf::Vector2f origin)
        {
            shape.setOrigin(origin);
            shape.setPosition(position);
        }
    }

    void Bonus::update(sf::Time dt)
    {
        shape().move({ 0.f, BONUS_FALL_SPEED * dt.asSeconds() });
    }

    bool Bonus::caughtBy(const Platform& platform) const
    {
        return bounds().findIntersection(platform.bounds()).has_value();
    }

    bool Bonus::missed(const Platform& platform) const
    {
        const sf::FloatRect platformBounds = platform.bounds();
        return bounds().position.y > platformBounds.position.y + platformBounds.size.y;
    }

    ExtraBallBonus::ExtraBallBonus(sf::Vector2f position)
    {
        mShape.setRadius(BALL_RADIUS);
        mShape.setFillColor(sf::Color(255, 220, 70));
        mShape.setOutlineThickness(2.f);
        mShape.setOutlineColor(sf::Color(180, 120, 20));
        centerOn(mShape, position, { BALL_RADIUS, BALL_RADIUS });
    }

    void ExtraBallBonus::apply(BonusTarget& target)
    {
        target.grantExtraBall();
    }

    sf::Shape& ExtraBallBonus::shape()
    {
        return mShape;
    }

    const sf::Shape& ExtraBallBonus::shape() const
    {
        return mShape;
    }

    WidePlatformBonus::WidePlatformBonus(sf::Vector2f position)
    {
        mShape.setSize({ BONUS_SIZE * 1.7f, BONUS_SIZE * 0.55f });
        mShape.setFillColor(sf::Color(70, 170, 255));
        mShape.setOutlineThickness(2.f);
        mShape.setOutlineColor(sf::Color(18, 70, 130));
        centerOn(mShape, position, mShape.getSize() / 2.f);
    }

    void WidePlatformBonus::apply(BonusTarget& target)
    {
        target.grantWidePlatform();
    }

    sf::Shape& WidePlatformBonus::shape()
    {
        return mShape;
    }

    const sf::Shape& WidePlatformBonus::shape() const
    {
        return mShape;
    }

    ExtraLifeBonus::ExtraLifeBonus(sf::Vector2f position)
    {
        mShape.setSize({ BONUS_SIZE, BONUS_SIZE });
        centerOn(mShape, position, { BONUS_SIZE / 2.f, BONUS_SIZE / 2.f });
    }

    void ExtraLifeBonus::apply(BonusTarget& target)
    {
        target.grantExtraLife();
    }

    void ExtraLifeBonus::drawVisual(sf::RenderTarget& target) const
    {
        drawHeart(target, bounds().position, BONUS_SIZE, sf::Color(230, 64, 84));
    }

    sf::Shape& ExtraLifeBonus::shape()
    {
        return mShape;
    }

    const sf::Shape& ExtraLifeBonus::shape() const
    {
        return mShape;
    }
}
