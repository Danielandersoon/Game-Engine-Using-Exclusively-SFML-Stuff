# GUESS Engine (Game Engine Using Exclusively SFML Stuff)
A modern C++ game engine built on SFML, designed for both 2D and 3D game development.

# Engine Architecture
## InstanceManager
### Engine
#### Window
#### Core Systems
##### Rendering
- 2D Pipeline
  - Sprites
  - TileMap
  - UI Elements
- 3D Pipeline
  - Mesh Rendering
  - PBR Materials
  - Lighting
  - Shadows
  - Post Processing
  - Particles
  - Skybox

##### Physics
- RigidBody2D/3D
- Box Collider
- Capsule Collider
- Physics Materials
- Physics World

##### Input
- Keyboard
- Mouse
- Touch

##### Events
- Publisher/Subscriber System
- Event Queue

##### Scene Management
- Scene Loading/Saving
- GameObject System
- Component System
- Transform Hierarchy

##### Utilities
- JSON Parser/Serializer
- Math Library
- Logger
- Timing System


# Features

## Core Systems
- Multi-instance engine support
- Event-driven architecture
- Scene management with serialization
- Component-based game objects
- Input system supporting keyboard, mouse and touch
- JSON-based serialization

## Graphics
- Deferred rendering pipeline
- PBR material system
- Dynamic and baked lighting
- Shadow mapping
- Post-processing effects
- Particle system
- Frustum culling
- Mesh loading (OBJ, FBX, Blender)

## 2D Features
- Sprite rendering
- Tilemap system
- UI framework
- 2D lighting

