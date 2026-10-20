#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace Game
{
    inline void drawHeart(sf::RenderTarget& target, sf::Vector2f topLeft, float size, sf::Color color)
    {
        const float radius = size * 0.28f;

        sf::CircleShape lobe(radius);
        lobe.setFillColor(color);
        lobe.setOrigin({ radius, radius });
        lobe.setPosition(topLeft + sf::Vector2f{ size * 0.30f, size * 0.32f });
        target.draw(lobe);

        lobe.setPosition(topLeft + sf::Vector2f{ size * 0.70f, size * 0.32f });
        target.draw(lobe);

        sf::ConvexShape tip(3);
        tip.setFillColor(color);
        tip.setPoint(0, topLeft + sf::Vector2f{ size * 0.06f, size * 0.40f });
        tip.setPoint(1, topLeft + sf::Vector2f{ size * 0.94f, size * 0.40f });
        tip.setPoint(2, topLeft + sf::Vector2f{ size * 0.50f, size * 0.98f });
        target.draw(tip);
    }
}
