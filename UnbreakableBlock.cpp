#include "UnbreakableBlock.h"

namespace Game
{
    UnbreakableBlock::UnbreakableBlock(sf::Vector2f position)
        : Block(position, sf::Color(92, 98, 112), sf::Color(214, 220, 230))
    {
    }

    void UnbreakableBlock::OnHit()
    {
    }

    bool UnbreakableBlock::mustBeCleared() const
    {
        return false;
    }
}
