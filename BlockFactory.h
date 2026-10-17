#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <string_view>

namespace Game
{
    class Block;

    class BlockFactory
    {
    protected:
        int createdBreackableBlocksCount = 0;

    public:
        virtual ~BlockFactory() = default;

        virtual std::shared_ptr<Block> CreateBlock(
            const sf::Vector2f& position,
            const sf::Color& color = sf::Color::White,
            int hits = 1) = 0;

        virtual std::string_view type() const = 0;
        virtual bool requiresColor() const { return false; }
        virtual bool usesHits() const { return false; }

        int GetCreatedBreackableBlocksCount();
        void ClearCounter();
    };

    BlockFactory* findBlockFactory(std::string_view type);
    void clearBlockFactoryCounters();
}
