#include "SettingsState.h"

#include "Constants.h"
#include "ResourceHolder.h"
#include "Settings.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Game
{
    namespace
    {
        constexpr float BoxSize = 28.f;
        constexpr float RowY = 240.f;
        constexpr float CenterX = SCREEN_WIDTH / 2.f;
        constexpr float BoxX = CenterX - 220.f;
    }

    SettingsState::SettingsState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mTitle(context.fonts.get(Fonts::Main), "Settings"s, 40)
        , mHint(context.fonts.get(Fonts::Main), "Enter - toggle    Esc - back"s, 12)
        , mLabel(context.fonts.get(Fonts::Main), ""s, 22)
    {
        const auto titleBounds = mTitle.getLocalBounds();
        mTitle.setOrigin(titleBounds.position + titleBounds.size / 2.f);
        mTitle.setPosition({ CenterX, 80.f });
        mTitle.setFillColor(sf::Color::White);

        const auto hintBounds = mHint.getLocalBounds();
        mHint.setOrigin(hintBounds.position + hintBounds.size / 2.f);
        mHint.setPosition({ CenterX, static_cast<float>(SCREEN_HEIGHT) - 48.f });
        mHint.setFillColor(sf::Color(180, 180, 180));

        mBox.setSize({ BoxSize, BoxSize });
        mBox.setFillColor(sf::Color::Transparent);
        mBox.setOutlineThickness(3.f);
        mBox.setPosition({ BoxX, RowY });

        mMark.setSize({ BoxSize - 12.f, BoxSize - 12.f });
        mMark.setPosition({ BoxX + 6.f, RowY + 6.f });

        mLabel.setPosition({ BoxX + BoxSize + 20.f, RowY + 2.f });

        refreshVisuals();
    }

    void SettingsState::toggleControl()
    {
        auto& control = getContext().settings.control;
        control = (control == ControlMode::Mouse) ? ControlMode::Keyboard : ControlMode::Mouse;
        refreshVisuals();
    }

    void SettingsState::refreshVisuals()
    {
        const bool mouse = getContext().settings.control == ControlMode::Mouse;
        const sf::Color color = sf::Color::Green;

        mLabel.setString(mouse ? "Control: mouse"s : "Control: arrows"s);
        mBox.setOutlineColor(color);
        mLabel.setFillColor(color);
        mMark.setFillColor(mouse ? color : sf::Color::Transparent);
    }

    void SettingsState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(30, 30, 30));

        window.draw(mTitle);
        window.draw(mBox);
        window.draw(mMark);
        window.draw(mLabel);
        window.draw(mHint);
    }

    bool SettingsState::update(sf::Time)
    {
        return false;
    }

    bool SettingsState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return false;

        const auto& input = getContext().settings.input;
        if (keyPressed->code == input.confirm || keyPressed->code == input.pause)
            toggleControl();
        else if (keyPressed->code == input.back)
            requestStackPop();

        return false;
    }
}
