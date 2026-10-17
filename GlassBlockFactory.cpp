#include "GlassBlockFactory.h"

#include "GlassBlock.h"

namespace Game
{
    std::shared_ptr<Block> GlassBlockFactory::CreateBlock(
        const sf::Vector2f& position,
        const sf::Color&,
        int)
    {
        ++createdBreackableBlocksCount;
        return std::make_shared<GlassBlock>(position);
    }
}
