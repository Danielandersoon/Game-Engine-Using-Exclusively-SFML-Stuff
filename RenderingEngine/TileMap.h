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
            std::shared_ptr<GUESS::rendering::Material> material; 
            bool isVisible;
            int tileId;

            Tile() = default;
            Tile(const Tile& other) :
                sprite(other.sprite),
                material(other.material),
                isVisible(other.isVisible),
                tileId(other.tileId) {}

        };

        std::vector<std::vector<Tile>> tiles;
        sf::Vector2u tileSize;
        sf::Vector2u mapSize;

        sf::Texture textureAtlas;
        std::vector<sf::IntRect> tileTextureRects;

    public:
        void loadFromFile(const std::string& path);
        void setTileSize(sf::Vector2u size) { tileSize = size; }
        void setMapSize(sf::Vector2u size) { mapSize = size; }
        void render(sf::RenderTarget& target);
        const std::vector<std::vector<Tile>>& getTiles() const { return tiles; }
        bool loadTextureAtlas(const std::string& texturePath, unsigned int tileWidth, unsigned int tileHeight);
        void setTileTextureRect(int tileId, const sf::IntRect& rect);

    };
}

#endif