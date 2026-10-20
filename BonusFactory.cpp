#include "BonusFactory.h"

#include "Constants.h"

#include <array>
#include <random>

namespace Game
{
    std::unique_ptr<Bonus> ExtraBallBonusFactory::create(sf::Vector2f position) const
    {
        return std::make_unique<ExtraBallBonus>(position);
    }

    std::unique_ptr<Bonus> WidePlatformBonusFactory::create(sf::Vector2f position) const
    {
        return std::make_unique<WidePlatformBonus>(position);
    }

    std::unique_ptr<Bonus> ExtraLifeBonusFactory::create(sf::Vector2f position) const
    {
        return std::make_unique<ExtraLifeBonus>(position);
    }

    namespace
    {
        const std::array<const BonusFactory*, 3>& factories()
        {
            static const ExtraBallBonusFactory extraBall;
            static const WidePlatformBonusFactory widePlatform;
            static const ExtraLifeBonusFactory extraLife;
            static const std::array<const BonusFactory*, 3> all = {
                &extraBall,
                &widePlatform,
                &extraLife
            };
            return all;
        }

        std::mt19937& randomEngine()
        {
            static std::mt19937 engine{ std::random_device{}() };
            return engine;
        }
    }

    std::unique_ptr<Bonus> tryCreateBonus(sf::Vector2f position)
    {
        std::uniform_real_distribution<float> chance(0.f, 1.f);
        if (chance(randomEngine()) >= BONUS_DROP_CHANCE)
            return nullptr;

        std::uniform_int_distribution<std::size_t> pick(0, factories().size() - 1);
        return factories()[pick(randomEngine())]->create(position);
    }
}
