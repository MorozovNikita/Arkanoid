#pragma once

#include "Block.h"

namespace Game
{
    class UnbreakableBlock : public Block
    {
    public:
        explicit UnbreakableBlock(sf::Vector2f position);

        void OnHit() override;
        bool mustBeCleared() const override;
        HitSound hitSound() const override { return HitSound::Wall; }
    };
}
