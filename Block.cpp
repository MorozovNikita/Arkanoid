#include "Block.h"

#include "Constants.h"

namespace Game
{
    Block::Block(sf::Vector2f position, sf::Color color)
    {
        mShape.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
        mShape.setPosition(position);
        mShape.setFillColor(color);
        mShape.setOutlineThickness(2.f);
        mShape.setOutlineColor(sf::Color(24, 28, 36));
    }

    void Block::update(sf::Time)
    {
    }

    sf::Shape& Block::shape()
    {
        return mShape;
    }

    const sf::Shape& Block::shape() const
    {
        return mShape;
    }
}
