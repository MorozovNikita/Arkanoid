#include "LeaderboardState.h"

#include "Constants.h"
#include "Leaderboard.h"
#include "ResourceHolder.h"
#include "Settings.h"
#include "StateIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <cctype>
#include <string>

namespace Game
{
    namespace
    {
        void centerText(sf::Text& text, float x, float y)
        {
            const auto bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f);
            text.setPosition({ x, y });
        }

        std::string rowText(int place, const Record& record)
        {
            std::string rank = std::to_string(place);
            if (rank.size() < 2)
                rank.insert(rank.begin(), ' ');

            std::string name = record.name.substr(0, 8);
            name.resize(8, ' ');

            std::string points = std::to_string(record.score);
            if (points.size() < 5)
                points.insert(0, 5 - points.size(), ' ');

            return rank + ". " + name + "  " + points;
        }
    }

    LeaderboardState::LeaderboardState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mTitle(context.fonts.get(Fonts::Main), ""s, 28)
        , mScoreLine(context.fonts.get(Fonts::Main), ""s, 16)
        , mNameLine(context.fonts.get(Fonts::Main), ""s, 16)
        , mHint(context.fonts.get(Fonts::Main), ""s, 12)
    {
        mTitle.setFillColor(sf::Color::White);
        mScoreLine.setFillColor(sf::Color(255, 214, 96));
        mNameLine.setFillColor(sf::Color::White);
        mHint.setFillColor(sf::Color(180, 180, 180));

        auto& board = context.leaderboard;
        if (!board.hasSession())
            mMode = Mode::Browse;
        else if (board.sessionNeedsName())
            mMode = Mode::EnterName;
        else
            mMode = Mode::Summary;

        rebuild();
    }

    void LeaderboardState::rebuild()
    {
        auto& board = getContext().leaderboard;
        const auto& font = getContext().fonts.get(Fonts::Main);
        const float centerX = SCREEN_WIDTH / 2.f;
        const bool session = mMode != Mode::Browse;

        mTitle.setString(session ? (board.session().won ? "You win"s : "You lose"s) : "Leaderboard"s);
        mTitle.setCharacterSize(session ? 28 : 32);
        centerText(mTitle, centerX, session ? 52.f : 64.f);

        mScoreLine.setString("");
        mNameLine.setString("");
        if (session)
        {
            std::string score = "Score " + std::to_string(board.session().score);
            if (mHighlight >= Leaderboard::PopupCount)
                score += "    Place " + std::to_string(mHighlight + 1);
            mScoreLine.setString(score);
            centerText(mScoreLine, centerX, 108.f);
        }

        if (mMode == Mode::EnterName)
        {
            mNameLine.setString("Name: " + mName + "_");
            centerText(mNameLine, centerX, 400.f);
            mHint.setString("Enter - save    Esc - skip"s);
        }
        else if (mMode == Mode::Summary)
        {
            mHint.setString("Enter - again    Esc - menu"s);
        }
        else
        {
            mHint.setString("Esc - back"s);
        }
        centerText(mHint, centerX, static_cast<float>(SCREEN_HEIGHT) - 42.f);

        mRows.clear();
        const auto& records = board.records();
        const int limit = session ? Leaderboard::PopupCount : Leaderboard::StoredCount;
        const int count = std::min(limit, static_cast<int>(records.size()));
        const float rowTop = session ? 168.f : 130.f;
        const float rowGap = session ? 36.f : 38.f;

        if (count == 0)
        {
            sf::Text empty(font, "No records"s, 16);
            empty.setFillColor(sf::Color::White);
            centerText(empty, centerX, session ? 240.f : 280.f);
            mRows.push_back(std::move(empty));
            return;
        }

        for (int index = 0; index < count; ++index)
        {
            const bool marked = index == mHighlight;
            sf::Text row(font, rowText(index + 1, records[static_cast<std::size_t>(index)]), 14);
            row.setFillColor(marked ? sf::Color::Green : sf::Color::White);
            centerText(row, centerX, rowTop + index * rowGap);
            mRows.push_back(std::move(row));
        }
    }

    void LeaderboardState::playAgain()
    {
        getContext().leaderboard.clearSession();
        requestStackPop();
        requestStackPop();
        requestStackPush(States::Game);
    }

    void LeaderboardState::leaveToMenu()
    {
        getContext().leaderboard.clearSession();
        requestStackPop();
        requestStackPop();
    }

    void LeaderboardState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(30, 30, 30));
        window.draw(mTitle);
        if (!mScoreLine.getString().isEmpty())
            window.draw(mScoreLine);
        for (const auto& row : mRows)
            window.draw(row);
        if (!mNameLine.getString().isEmpty())
            window.draw(mNameLine);
        window.draw(mHint);
    }

    bool LeaderboardState::update(sf::Time)
    {
        return false;
    }

    bool LeaderboardState::handleEvent(const sf::Event& event)
    {
        if (mMode == Mode::EnterName)
        {
            if (const auto* typed = event.getIf<sf::Event::TextEntered>())
            {
                const char32_t code = typed->unicode;
                if (code == 8 || code == 127)
                {
                    if (!mName.empty())
                        mName.pop_back();
                }
                else if (mName.size() < 8 && code < 128 && std::isalnum(static_cast<unsigned char>(code)))
                {
                    mName.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(code))));
                }
                else
                {
                    return false;
                }

                rebuild();
                return false;
            }
        }

        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return false;

        const auto& input = getContext().settings.input;
        if (keyPressed->code == input.confirm)
        {
            if (mMode == Mode::EnterName)
            {
                mHighlight = getContext().leaderboard.commitSession(mName);
                mMode = Mode::Summary;
                rebuild();
            }
            else if (mMode == Mode::Summary)
            {
                playAgain();
            }
        }
        else if (keyPressed->code == input.back)
        {
            if (mMode == Mode::EnterName)
            {
                mMode = Mode::Summary;
                rebuild();
            }
            else if (mMode == Mode::Summary)
            {
                leaveToMenu();
            }
            else
            {
                requestStackPop();
            }
        }

        return false;
    }
}
