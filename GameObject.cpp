#include "GameObject.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace Game
{
    void GameObject::draw(sf::RenderTarget& target) const
    {
        if (mAlive)
            target.draw(shape());
    }

    sf::FloatRect GameObject::bounds() const
    {
        return shape().getGlobalBounds();
    }
}
