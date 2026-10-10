#pragma once

#include "Block.h"
#include "Constants.h"

namespace Game
{
    class DurableBlock : public Block
    {
    public:
        DurableBlock(sf::Vector2f position, sf::Color color, int hitPoints = DURABLE_BLOCK_HIT_POINTS);

        void OnHit() override;

    protected:
        int hitsLeft() const { return mHitsLeft; }
        int maxHits() const { return mMaxHits; }

        virtual void refreshAppearance();

    private:
        int mMaxHits = 1;
        int mHitsLeft = 1;
        sf::Color mIntactColor;
    };
}
