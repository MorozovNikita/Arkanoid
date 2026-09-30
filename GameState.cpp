#include "GameState.h"

#include "Constants.h"
#include "ResourceHolder.h"
#include "Settings.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

namespace Game
{
    GameState::GameState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mPlatform()
        , mBall()
        , mHint(context.fonts.get(Fonts::Main), ""s, 14)
    {
        const bool mouse = context.settings.control == ControlMode::Mouse;
        mHint.setString(mouse ? "Click or Space - launch"s : "Space - launch"s);
        mHint.setFillColor(sf::Color(180, 180, 180));

        const auto bounds = mHint.getLocalBounds();
        mHint.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y });
        mHint.setPosition({ SCREEN_WIDTH / 2.f, 16.f });

        mBall.attachTo(mPlatform);
    }

    void GameState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(24, 28, 36));

        mPlatform.draw(window);
        mBall.draw(window);

        if (mBall.isStuck())
            window.draw(mHint);
    }

    bool GameState::update(sf::Time dt)
    {
        const auto context = getContext();
        mPlatform.update(dt, context.window, context.settings);
        mBall.update(dt, mPlatform);
        return false;
    }

    bool GameState::handleEvent(const sf::Event& event)
    {
        const auto& input = getContext().settings.input;

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->code == input.pause)
            {
                if (mBall.isStuck())
                    mBall.launch();
                else
                    requestStackPush(States::Pause);
            }
            else if (keyPressed->code == input.back)
            {
                requestStackPop();
            }

            return false;
        }

        if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left && mBall.isStuck())
                mBall.launch();

            return false;
        }

        return true;
    }
}
