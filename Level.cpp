#include "Level.h"

#include "Block.h"
#include "BlockFactory.h"
#include "Constants.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>
#include <set>
#include <stdexcept>

namespace Game
{
    namespace
    {
        sf::Vector2f cellPosition(int column, int row)
        {
            return {
                column * (BLOCK_WIDTH + BLOCK_GAP_X),
                BLOCK_FIELD_TOP + row * (BLOCK_HEIGHT + BLOCK_GAP_Y)
            };
        }

        void validateCell(int column, int row)
        {
            if (column < 0 || column >= LEVEL_COLUMN_COUNT || row < 0 || row >= LEVEL_ROW_COUNT)
            {
                throw std::runtime_error(
                    "Block at column " + std::to_string(column) + ", row " + std::to_string(row)
                    + " does not fit in the upper half");
            }
        }

        sf::Color readColor(const nlohmann::json& block)
        {
            if (!block.contains("color") || !block["color"].is_array() || block["color"].size() != 3)
                throw std::runtime_error("Block needs color: [r, g, b]");

            int channels[3] = {};
            for (int i = 0; i < 3; ++i)
            {
                if (!block["color"][i].is_number_integer())
                    throw std::runtime_error("Color channels must be integers");

                channels[i] = block["color"][i].get<int>();
                if (channels[i] < 0 || channels[i] > 255)
                    throw std::runtime_error("Color channels must be from 0 to 255");
            }

            return sf::Color(
                static_cast<std::uint8_t>(channels[0]),
                static_cast<std::uint8_t>(channels[1]),
                static_cast<std::uint8_t>(channels[2]));
        }

        LevelBlock readBlock(const nlohmann::json& block)
        {
            if (!block.is_object() || !block.contains("type") || !block["type"].is_string())
                throw std::runtime_error("Each block needs a type");
            if (!block.contains("column") || !block["column"].is_number_integer())
                throw std::runtime_error("Each block needs an integer column");
            if (!block.contains("row") || !block["row"].is_number_integer())
                throw std::runtime_error("Each block needs an integer row");

            LevelBlock result;
            result.type = block["type"].get<std::string>();
            result.column = block["column"].get<int>();
            result.row = block["row"].get<int>();
            validateCell(result.column, result.row);

            BlockFactory* factory = findBlockFactory(result.type);
            if (!factory)
                throw std::runtime_error("Unknown block type: " + result.type);
            if (factory->requiresColor())
                result.color = readColor(block);

            if (factory->usesHits() && block.contains("hits"))
            {
                if (!block["hits"].is_number_integer() || block["hits"].get<int>() < 1)
                    throw std::runtime_error("hits must be an integer of 1 or more");
                result.hits = block["hits"].get<int>();
            }
            else
            {
                result.hits = DURABLE_BLOCK_HIT_POINTS;
            }

            return result;
        }
    }

    std::vector<std::string> Level::listFiles()
    {
        namespace fs = std::filesystem;

        std::vector<std::string> names;
        std::error_code error;
        if (!fs::exists(LEVELS_PATH, error))
            return names;

        for (const auto& entry : fs::directory_iterator(LEVELS_PATH, error))
        {
            if (error || !entry.is_regular_file())
                continue;
            if (entry.path().extension() != ".json")
                continue;
            names.push_back(entry.path().filename().string());
        }

        std::sort(names.begin(), names.end());
        return names;
    }

    Level Level::loadFromFile(const std::filesystem::path& path)
    {
        std::ifstream input(path);
        if (!input)
            throw std::runtime_error("Cannot open " + path.stem().string());

        nlohmann::json data;
        try
        {
            data = nlohmann::json::parse(input);
        }
        catch (const nlohmann::json::exception&)
        {
            throw std::runtime_error("Invalid JSON: " + path.stem().string());
        }

        if (!data.is_object() || !data.contains("blocks") || !data["blocks"].is_array())
            throw std::runtime_error(path.stem().string() + " needs a blocks array");

        Level level;
        std::set<std::pair<int, int>> occupied;
        for (const auto& block : data["blocks"])
        {
            LevelBlock item = readBlock(block);
            if (!occupied.insert({ item.column, item.row }).second)
            {
                throw std::runtime_error(
                    "Two blocks at column " + std::to_string(item.column)
                    + ", row " + std::to_string(item.row));
            }
            level.mBlocks.push_back(item);
        }

        level.mName = path.stem().string();
        return level;
    }

    Level Level::loadDefault()
    {
        const auto files = listFiles();
        if (files.empty())
            return Level{};

        return loadFromFile(std::filesystem::path(LEVELS_PATH) / files.front());
    }

    std::vector<std::shared_ptr<Block>> Level::createBlocks() const
    {
        std::vector<std::shared_ptr<Block>> blocks;
        blocks.reserve(mBlocks.size());
        clearBlockFactoryCounters();

        for (const LevelBlock& block : mBlocks)
        {
            BlockFactory* factory = findBlockFactory(block.type);
            if (!factory)
                continue;

            blocks.push_back(factory->CreateBlock(
                cellPosition(block.column, block.row),
                block.color,
                block.hits));
        }

        return blocks;
    }
}
