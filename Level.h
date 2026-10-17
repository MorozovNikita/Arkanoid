#pragma once

#include <SFML/Graphics/Color.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace Game
{
    class Block;

    struct LevelBlock
    {
        std::string type;
        int column = 0;
        int row = 0;
        sf::Color color = sf::Color::White;
        int hits = 1;
    };

    class Level
    {
    public:
        static std::vector<std::string> listFiles();
        static Level loadFromFile(const std::filesystem::path& path);
        static Level loadDefault();

        std::vector<std::shared_ptr<Block>> createBlocks() const;
        const std::string& name() const { return mName; }

    private:
        std::vector<LevelBlock> mBlocks;
        std::string mName;
    };
}
