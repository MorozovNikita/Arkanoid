#pragma once

#include "BlockFactory.h"

namespace Game
{
    class UnbreakableBlockFactory : public BlockFactory
    {
    public:
        std::shared_ptr<Block> CreateBlock(
            const sf::Vector2f& position,
            const sf::Color& color,
            int hits) override;

        std::string_view type() const override { return "unbreakable"; }
    };
}
