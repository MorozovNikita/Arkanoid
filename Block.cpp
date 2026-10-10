#include "Block.h"

#include "Constants.h"

namespace Game
{
    Block::Block(sf::Vector2f position, sf::Color fill, sf::Color outline)
    {
        mShape.setSize({ BLOCK_WIDTH, BLOCK_HEIGHT });
        mShape.setPosition(position);
        mShape.setFillColor(fill);
        mShape.setOutlineColor(outline);
        mShape.setOutlineThickness(2.f);
    }

    void Block::update(sf::Time)
    {
    }

    bool Block::mustBeCleared() const
    {
        return isAlive();
    }

    sf::FloatRect Block::collisionBounds() const
    {
        return bounds();
    }

    bool Block::canCollide() const
    {
        return isAlive();
    }

    bool Block::deflectsBall() const
    {
        return true;
    }

    void Block::setFillColor(sf::Color color)
    {
        mShape.setFillColor(color);
    }

    void Block::setOutlineColor(sf::Color color)
    {
        mShape.setOutlineColor(color);
    }

    sf::Color Block::fillColor() const
    {
        return mShape.getFillColor();
    }

    sf::Color Block::outlineColor() const
    {
        return mShape.getOutlineColor();
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
