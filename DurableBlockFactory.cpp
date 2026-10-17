#include "DurableBlockFactory.h"

#include "DurableBlock.h"

namespace Game
{
    std::shared_ptr<Block> DurableBlockFactory::CreateBlock(
        const sf::Vector2f& position,
        const sf::Color& color,
        int hits)
    {
        ++createdBreackableBlocksCount;
        return std::make_shared<DurableBlock>(position, color, hits);
    }
}
