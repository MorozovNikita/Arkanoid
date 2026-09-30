#pragma once

#include <SFML/Window/Keyboard.hpp>

namespace Game
{
    enum class ControlMode
    {
        Keyboard,
        Mouse
    };

    struct InputBindings
    {
        sf::Keyboard::Key moveLeft  = sf::Keyboard::Key::Left;
        sf::Keyboard::Key moveRight = sf::Keyboard::Key::Right;
        sf::Keyboard::Key pause     = sf::Keyboard::Key::Space;
        sf::Keyboard::Key confirm   = sf::Keyboard::Key::Enter;
        sf::Keyboard::Key back      = sf::Keyboard::Key::Escape;
        sf::Keyboard::Key menuUp    = sf::Keyboard::Key::Up;
        sf::Keyboard::Key menuDown  = sf::Keyboard::Key::Down;
    };

    struct Settings
    {
        ControlMode control = ControlMode::Keyboard;
        InputBindings input;
    };
}
