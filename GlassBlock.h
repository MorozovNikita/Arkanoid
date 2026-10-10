#pragma once

#include "Block.h"

namespace Game
{
    class GlassBlock : public Block
    {
    public:
        explicit GlassBlock(sf::Vector2f position);

        void OnHit() override;
        bool deflectsBall() const override;
        HitSound hitSound() const override { return HitSound::Glass; }
    };
}
