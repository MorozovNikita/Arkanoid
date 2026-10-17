#include "SmoothDestroyableBlockFactory.h"

#include "SmoothDestroyableBlock.h"

namespace Game
{
    std::shared_ptr<Block> SmoothDestroyableBlockFactory::CreateBlock(
        const sf::Vector2f& position,
        const sf::Color& color,
        int)
    {
        ++createdBreackableBlocksCount;
        return std::make_shared<SmoothDestroyableBlock>(position, color);
    }
}
