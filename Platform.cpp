#include "Platform.h"

#include "Constants.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <algorithm>

namespace Game
{
    Platform::Platform()
    {
        mShape.setSize({ PLATFORM_WIDTH, PLATFORM_HEIGHT });
        mShape.setFillColor(sf::Color(70, 170, 255));
        mShape.setOutlineThickness(2.f);
        mShape.setOutlineColor(sf::Color(18, 70, 130));
        mShape.setPosition({
            (SCREEN_WIDTH - PLATFORM_WIDTH) / 2.f,
            SCREEN_HEIGHT - PLATFORM_MARGIN_BOTTOM - PLATFORM_HEIGHT
        });
    }

    void Platform::update(sf::Time dt, const sf::RenderWindow& window, const Settings& settings)
    {
        sf::Vector2f position = mShape.getPosition();

        if (settings.control == ControlMode::Mouse)
        {
            const sf::Vector2i mouse = sf::Mouse::getPosition(window);
            position.x = static_cast<float>(mouse.x) - mShape.getSize().x / 2.f;
        }
        else
        {
            float direction = 0.f;
            if (sf::Keyboard::isKeyPressed(settings.input.moveLeft))
                direction -= 1.f;
            if (sf::Keyboard::isKeyPressed(settings.input.moveRight))
                direction += 1.f;

            position.x += direction * PLATFORM_SPEED * dt.asSeconds();
        }

        mShape.setPosition(position);
        clampToField();
    }

    sf::Shape& Platform::shape()
    {
        return mShape;
    }

    const sf::Shape& Platform::shape() const
    {
        return mShape;
    }

    void Platform::clampToField()
    {
        sf::Vector2f position = mShape.getPosition();
        const float maxX = static_cast<float>(SCREEN_WIDTH) - mShape.getSize().x;
        position.x = std::clamp(position.x, 0.f, maxX);
        mShape.setPosition(position);
    }
}
