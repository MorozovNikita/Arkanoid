#include "UnbreakableBlockFactory.h"

#include "UnbreakableBlock.h"

namespace Game
{
    std::shared_ptr<Block> UnbreakableBlockFactory::CreateBlock(
        const sf::Vector2f& position,
        const sf::Color&,
        int)
    {
        return std::make_shared<UnbreakableBlock>(position);
    }
}
