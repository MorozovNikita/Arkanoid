#pragma once

#include "Bonus.h"

#include <memory>

namespace Game
{
    class BonusFactory
    {
    public:
        virtual ~BonusFactory() = default;

        virtual std::unique_ptr<Bonus> create(sf::Vector2f position) const = 0;
    };

    class ExtraBallBonusFactory : public BonusFactory
    {
    public:
        std::unique_ptr<Bonus> create(sf::Vector2f position) const override;
    };

    class WidePlatformBonusFactory : public BonusFactory
    {
    public:
        std::unique_ptr<Bonus> create(sf::Vector2f position) const override;
    };

    class ExtraLifeBonusFactory : public BonusFactory
    {
    public:
        std::unique_ptr<Bonus> create(sf::Vector2f position) const override;
    };

    std::unique_ptr<Bonus> tryCreateBonus(sf::Vector2f position);
}
