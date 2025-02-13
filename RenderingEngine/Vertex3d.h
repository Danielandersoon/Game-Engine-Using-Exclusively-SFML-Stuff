#ifndef VERTEX3D_H
#define VERTEX3D_H
#include <SFML/Graphics.hpp>
#include "../GMath.h"

namespace GUESS::rendering::threed {
    class Vertex3d {
    public:
        GUESS::core::math::Vector3f position;
        GUESS::core::math::Vector2f texCoords;
        sf::Color color;
        GUESS::core::math::Vector3f normal;

        Vertex3d() : position(), texCoords(), color(sf::Color::White), normal() {}

        Vertex3d(const GUESS::core::math::Vector3f& pos) :
            position(pos),
            texCoords(),
            color(sf::Color::White),
            normal() {}

        Vertex3d(const GUESS::core::math::Vector3f& pos, const sf::Color& col) :
            position(pos),
            texCoords(),
            color(col),
            normal() {}

        Vertex3d(const GUESS::core::math::Vector3f& pos, const GUESS::core::math::Vector2f& tex) :
            position(pos),
            texCoords(tex),
            color(sf::Color::White),
            normal() {}

        Vertex3d(const GUESS::core::math::Vector3f& pos, const GUESS::core::math::Vector2f& tex, const sf::Color& col) :
            position(pos),
            texCoords(tex),
            color(col),
            normal() {}

        Vertex3d(const GUESS::core::math::Vector3f& pos, const GUESS::core::math::Vector2f& tex, const sf::Color& col, const GUESS::core::math::Vector3f& norm) :
            position(pos),
            texCoords(tex),
            color(col),
            normal(norm) {}
    };
}
#endif
