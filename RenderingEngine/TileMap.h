#ifndef TILE_MAP_H
#define TILE_MAP_H
#include <vector>
#include <SFML/Graphics/Texture.hpp>

#include "./Sprite.h"
#include "./Material.h"

namespace GUESS::rendering::twod {
    class TileMap {
    private:
        struct Tile {
            Sprite sprite;
            GUESS::rendering::Material material;
            bool isVisible;
            int tileId;
        };

        std::vector<std::vector<Tile>> tiles;
        sf::Vector2u tileSize;
        sf::Vector2u mapSize;

    public:
        void loadFromFile(const std::string& path);
        void setTileSize(sf::Vector2u size) { tileSize = size; }
        void setMapSize(sf::Vector2u size) { mapSize = size; }
        void render(sf::RenderTarget& target);
        const std::vector<std::vector<Tile>>& getTiles() const { return tiles; }
    };
}

#endif