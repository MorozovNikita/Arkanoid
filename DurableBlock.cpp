#include "DurableBlock.h"

#include "Constants.h"

namespace Game
{
    DurableBlock::DurableBlock(sf::Vector2f position, sf::Color color, int hitPoints)
        : Block(position, color, sf::Color(48, 28, 72))
        , mMaxHits(hitPoints > 0 ? hitPoints : DURABLE_BLOCK_HIT_POINTS)
        , mHitsLeft(mMaxHits)
        , mIntactColor(color)
    {
        refreshAppearance();
    }

    void DurableBlock::OnHit()
    {
        if (mHitsLeft <= 0)
            return;

        --mHitsLeft;
        if (mHitsLeft == 0)
        {
            destroy();
            return;
        }

        refreshAppearance();
    }

    void DurableBlock::refreshAppearance()
    {
        // When sprites arrive, draw the frame (maxHits - hitsLeft) from drawVisual().
        const float health = static_cast<float>(mHitsLeft) / static_cast<float>(mMaxHits);
        const float shade = 0.4f + 0.6f * health;

        sf::Color color = mIntactColor;
        color.r = static_cast<std::uint8_t>(static_cast<float>(color.r) * shade);
        color.g = static_cast<std::uint8_t>(static_cast<float>(color.g) * shade);
        color.b = static_cast<std::uint8_t>(static_cast<float>(color.b) * shade);
        setFillColor(color);
    }
}
