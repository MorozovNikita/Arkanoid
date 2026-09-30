#include "MenuState.h"

#include "Constants.h"
#include "MenuConstants.h"
#include "ResourceHolder.h"
#include "Settings.h"
#include "StateIdentifiers.h"
#include "StateStack.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Game
{
    MenuState::MenuState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , m_selectedIndex(0)
        , m_titleText(context.fonts.get(Fonts::Main), "Arkanoid"s)
    {
        m_items = { "Game"s, "Difficulty"s, "Leaderboard"s, "Settings"s, "Exit"s };

        m_titleText.setCharacterSize(40);
        m_titleText.setFillColor(sf::Color::White);

        auto titleBounds = m_titleText.getLocalBounds();
        m_titleText.setOrigin(titleBounds.position + titleBounds.size / 2.f);
        m_titleText.setPosition({ SCREEN_WIDTH / 2.f, 80.f });

        const float centerX = SCREEN_WIDTH / 2.f;

        m_itemTexts.clear();
        for (size_t i = 0; i < m_items.size(); ++i)
        {
            sf::Text text(context.fonts.get(Fonts::Main), m_items[i], 24);

            auto bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f);
            text.setPosition({ centerX, FIRST_POINT_Y + i * DISTANCE });

            m_itemTexts.push_back(text);
        }

        m_arrow.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_arrow.resize(3);
        for (int v = 0; v < 3; ++v)
            m_arrow[v].color = sf::Color::Green;

        updateTextColors();
    }

    bool MenuState::isItemActive(int index) const
    {
        switch (index)
        {
        case Menu::Game:
        case Menu::Settings:
        case Menu::Exit:
            return true;
        default:
            return false;
        }
    }

    void MenuState::moveSelection(int step)
    {
        const int count = static_cast<int>(m_items.size());
        if (count == 0)
            return;

        for (int i = 0; i < count; ++i)
        {
            m_selectedIndex = (m_selectedIndex + step + count) % count;
            if (isItemActive(m_selectedIndex))
                break;
        }

        updateTextColors();
    }

    void MenuState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(30, 30, 30));

        window.draw(m_titleText);

        for (const auto& text : m_itemTexts)
            window.draw(text);

        const auto& selected = m_itemTexts[m_selectedIndex];
        auto bounds = selected.getGlobalBounds();

        float arrowX = bounds.position.x - ARROW_SIZE - 10.f;
        float arrowY = bounds.position.y + bounds.size.y / 2.f - ARROW_SIZE / 2.f;

        m_arrow[0].position = { arrowX, arrowY };
        m_arrow[1].position = { arrowX, arrowY + ARROW_SIZE };
        m_arrow[2].position = { arrowX + ARROW_SIZE, arrowY + ARROW_SIZE / 2.f };

        window.draw(m_arrow);
    }

    bool MenuState::update(sf::Time)
    {
        return true;
    }

    bool MenuState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return true;

        const auto& input = getContext().settings.input;
        if (keyPressed->code == input.menuUp)
            moveSelection(-1);
        else if (keyPressed->code == input.menuDown)
            moveSelection(1);
        else if (keyPressed->code == input.confirm)
            onItemSelected();
        else if (keyPressed->code == input.back)
            requestStackPop();

        return false;
    }

    void MenuState::updateTextColors()
    {
        for (size_t i = 0; i < m_itemTexts.size(); ++i)
        {
            if (!isItemActive(static_cast<int>(i)))
                m_itemTexts[i].setFillColor(sf::Color(110, 110, 110));
            else if (static_cast<int>(i) == m_selectedIndex)
                m_itemTexts[i].setFillColor(sf::Color::Green);
            else
                m_itemTexts[i].setFillColor(sf::Color::White);
        }
    }

    void MenuState::onItemSelected()
    {
        if (!isItemActive(m_selectedIndex))
            return;

        switch (m_selectedIndex)
        {
        case Menu::Game:
            requestStackPush(States::Game);
            break;
        case Menu::Settings:
            requestStackPush(States::Settings);
            break;
        case Menu::Exit:
            requestStackPop();
            break;
        default:
            break;
        }
    }
}
