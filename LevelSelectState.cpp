#include "LevelSelectState.h"

#include "Constants.h"
#include "Level.h"
#include "MenuConstants.h"
#include "ResourceHolder.h"
#include "Settings.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <filesystem>

namespace Game
{
    namespace
    {
        constexpr int VisibleCount = 8;
        constexpr float ListTop = 160.f;
        constexpr float ListGap = 42.f;
        constexpr float CenterX = SCREEN_WIDTH / 2.f;

        void centerText(sf::Text& text, sf::Vector2f position)
        {
            const auto bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f);
            text.setPosition(position);
        }

        std::string levelTitle(const std::string& fileName)
        {
            return std::filesystem::path(fileName).stem().string();
        }
    }

    LevelSelectState::LevelSelectState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mFileNames(Level::listFiles())
        , mTitle(context.fonts.get(Fonts::Main), "Level"s, 40)
        , mEmpty(context.fonts.get(Fonts::Main), "No levels"s, 20)
        , mError(context.fonts.get(Fonts::Main), ""s, 12)
        , mHint(context.fonts.get(Fonts::Main), "Enter - select    Esc - back"s, 12)
    {
        centerText(mTitle, { CenterX, 80.f });
        mTitle.setFillColor(sf::Color::White);

        centerText(mEmpty, { CenterX, 260.f });
        mEmpty.setFillColor(sf::Color(140, 140, 140));

        mError.setFillColor(sf::Color(220, 90, 90));
        centerText(mHint, { CenterX, static_cast<float>(SCREEN_HEIGHT) - 48.f });
        mHint.setFillColor(sf::Color(180, 180, 180));

        mArrow.setPrimitiveType(sf::PrimitiveType::Triangles);
        mArrow.resize(3);
        for (int vertex = 0; vertex < 3; ++vertex)
            mArrow[vertex].color = sf::Color::Green;

        const auto& font = context.fonts.get(Fonts::Main);
        for (const std::string& name : mFileNames)
            mLabels.emplace_back(font, levelTitle(name), 16);

        const std::string& current = context.level.name();
        for (int index = 0; index < static_cast<int>(mFileNames.size()); ++index)
        {
            if (levelTitle(mFileNames[static_cast<std::size_t>(index)]) == current)
            {
                mSelected = index;
                break;
            }
        }

        refresh();
    }

    void LevelSelectState::refresh()
    {
        const int count = static_cast<int>(mLabels.size());
        int first = 0;
        if (mSelected >= VisibleCount)
            first = mSelected - VisibleCount + 1;

        for (int index = 0; index < count; ++index)
        {
            const int visibleIndex = index - first;
            const bool visible = visibleIndex >= 0 && visibleIndex < VisibleCount;
            mLabels[index].setFillColor(index == mSelected ? sf::Color::Green : sf::Color::White);
            if (!visible)
                continue;

            centerText(mLabels[index], { CenterX, ListTop + visibleIndex * ListGap });
        }

        centerText(mError, { CenterX, static_cast<float>(SCREEN_HEIGHT) - 88.f });
    }

    void LevelSelectState::confirm()
    {
        if (mFileNames.empty())
            return;

        try
        {
            const auto path = std::filesystem::path(LEVELS_PATH) / mFileNames[static_cast<std::size_t>(mSelected)];
            getContext().level = Level::loadFromFile(path);
            mError.setString("");
            requestStackPop();
        }
        catch (const std::exception& error)
        {
            mError.setString(error.what());
        }

        refresh();
    }

    void LevelSelectState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(30, 30, 30));
        window.draw(mTitle);
        window.draw(mHint);

        if (mLabels.empty())
        {
            window.draw(mEmpty);
            window.draw(mError);
            return;
        }

        const int count = static_cast<int>(mLabels.size());
        int first = 0;
        if (mSelected >= VisibleCount)
            first = mSelected - VisibleCount + 1;

        for (int index = first; index < count && index < first + VisibleCount; ++index)
            window.draw(mLabels[static_cast<std::size_t>(index)]);

        const auto& selected = mLabels[static_cast<std::size_t>(mSelected)];
        const auto bounds = selected.getGlobalBounds();
        const float arrowX = bounds.position.x - ARROW_SIZE - 10.f;
        const float arrowY = bounds.position.y + bounds.size.y / 2.f - ARROW_SIZE / 2.f;
        mArrow[0].position = { arrowX, arrowY };
        mArrow[1].position = { arrowX, arrowY + ARROW_SIZE };
        mArrow[2].position = { arrowX + ARROW_SIZE, arrowY + ARROW_SIZE / 2.f };
        window.draw(mArrow);

        if (!mError.getString().isEmpty())
            window.draw(mError);
    }

    bool LevelSelectState::update(sf::Time)
    {
        return false;
    }

    bool LevelSelectState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return false;

        const auto& input = getContext().settings.input;
        const int count = static_cast<int>(mFileNames.size());
        if (keyPressed->code == input.menuUp && count > 0)
        {
            mSelected = (mSelected + count - 1) % count;
            refresh();
        }
        else if (keyPressed->code == input.menuDown && count > 0)
        {
            mSelected = (mSelected + 1) % count;
            refresh();
        }
        else if (keyPressed->code == input.confirm)
        {
            confirm();
        }
        else if (keyPressed->code == input.back)
        {
            requestStackPop();
        }

        return false;
    }
}
