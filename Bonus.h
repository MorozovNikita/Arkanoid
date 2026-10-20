#pragma once

#include "GameObject.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace Game
{
    class Platform;

    class BonusTarget
    {
    public:
        virtual ~BonusTarget() = default;

        virtual void grantExtraBall() = 0;
        virtual void grantWidePlatform() = 0;
        virtual void grantExtraLife() = 0;
    };

    class Bonus : public GameObject
    {
    public:
        void update(sf::Time dt);
        bool caughtBy(const Platform& platform) const;
        bool missed(const Platform& platform) const;

        virtual void apply(BonusTarget& target) = 0;
    };

    class ExtraBallBonus : public Bonus
    {
    public:
        explicit ExtraBallBonus(sf::Vector2f position);

        void apply(BonusTarget& target) override;

    private:
        sf::Shape& shape() override;
        const sf::Shape& shape() const override;

        sf::CircleShape mShape;
    };

    class WidePlatformBonus : public Bonus
    {
    public:
        explicit WidePlatformBonus(sf::Vector2f position);

        void apply(BonusTarget& target) override;

    private:
        sf::Shape& shape() override;
        const sf::Shape& shape() const override;

        sf::RectangleShape mShape;
    };

    class ExtraLifeBonus : public Bonus
    {
    public:
        explicit ExtraLifeBonus(sf::Vector2f position);

        void apply(BonusTarget& target) override;

    protected:
        void drawVisual(sf::RenderTarget& target) const override;

    private:
        sf::Shape& shape() override;
        const sf::Shape& shape() const override;

        sf::RectangleShape mShape;
    };
}
