#pragma once

#include "Block.h"
#include "Constants.h"

namespace Game
{
    class GlassBlock : public Block
    {
    public:
        explicit GlassBlock(sf::Vector2f position);

        void OnHit() override;
        bool deflectsBall() const override;
        HitSound hitSound() const override { return HitSound::Glass; }
        int score() const override { return GLASS_BLOCK_SCORE; }
    };
}
