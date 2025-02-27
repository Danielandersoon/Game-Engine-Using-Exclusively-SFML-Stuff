#include <SFML/Graphics/RenderStates.hpp>
#include "./TileMap.h"
#include "../JSONParser.h"
#include "../JsonValue.h"

namespace GUESS::rendering::twod {
    void TileMap::loadFromFile(const std::string& path) {
        // Parse tilemap JSON data
        std::ifstream file(path);
        std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        GUESS::core::JsonValue mapData = GUESS::core::JsonParser::parse(jsonContent);

        // Get map dimensions
        mapSize.x = mapData.get<std::map<std::string, GUESS::core::JsonValue>>()["width"].get<double>();
        mapSize.y = mapData.get<std::map<std::string, GUESS::core::JsonValue>>()["height"].get<double>();

        // Get tile dimensions
        tileSize.x = mapData.get<std::map<std::string, GUESS::core::JsonValue>>()["tilewidth"].get<double>();
        tileSize.y = mapData.get<std::map<std::string, GUESS::core::JsonValue>>()["tileheight"].get<double>();

        // Initialize tile grid
        tiles.resize(mapSize.y);
        for (auto& row : tiles) {
            row.resize(mapSize.x);
        }

        // Load tile data
        const auto& layers = mapData.get<std::map<std::string, GUESS::core::JsonValue>>()["layers"].get<std::vector<GUESS::core::JsonValue>>();
        for (const auto& layer : layers) {
            const auto& data = layer.get<std::map<std::string, GUESS::core::JsonValue>>()["data"].get<std::vector<GUESS::core::JsonValue>>();

            for (size_t y = 0; y < mapSize.y; y++) {
                for (size_t x = 0; x < mapSize.x; x++) {
                    int tileId = static_cast<int>(data[y * mapSize.x + x].get<double>());
                    tiles[y][x].tileId = tileId;
                    tiles[y][x].isVisible = tileId != 0;

                    if (tiles[y][x].isVisible) {
                        tiles[y][x].sprite.setPosition(sf::Vector2f(x * tileSize.x, y * tileSize.y));
                        tiles[y][x].sprite.setScale(sf::Vector2f(1.0f, 1.0f));
                    }
                }
            }
        }
    }

    void TileMap::render(sf::RenderTarget& target) {
        for (size_t y = 0; y < mapSize.y; y++) {
            for (size_t x = 0; x < mapSize.x; x++) {
                if (tiles[y][x].isVisible) {
                    tiles[y][x].material.bind();
                    sf::RenderStates states;
                    states.shader = &tiles[y][x].material.getShader().getNativeShader();

                    // Create a sprite from the tile's existing sprite
                    sf::Sprite renderSprite;
                    renderSprite.setTexture(tiles[y][x].sprite.getTexture());
                    renderSprite.setPosition(tiles[y][x].sprite.getPosition());
                    renderSprite.setScale(tiles[y][x].sprite.getScale());

                    target.draw(renderSprite, states);
                }
            }
        }
    }
}