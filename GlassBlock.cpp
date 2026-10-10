#include "GlassBlock.h"

namespace Game
{
    GlassBlock::GlassBlock(sf::Vector2f position)
        : Block(position, sf::Color(190, 230, 255, 48), sf::Color(220, 245, 255))
    {
    }

    void GlassBlock::OnHit()
    {
        destroy();
    }

    bool GlassBlock::deflectsBall() const
    {
        return false;
    }
}
