# GUESS Engine (Game Engine Using Exclusively SFML Stuff)
A modern C++ game engine built on SFML, designed for both 2D and 3D game development.

## Features

### Core Systems
- Multi-instance engine support
- Event-driven architecture
- Scene management with serialization
- Component-based game objects
- Input system supporting keyboard, mouse and touch
- JSON-based serialization

### Graphics
- Deferred rendering pipeline
- PBR material system
- Dynamic and baked lighting
- Shadow mapping
- Post-processing effects
- Particle system
- Frustum culling
- Mesh loading (OBJ, FBX, Blender)

### 2D Features
- Sprite rendering
- Tilemap system
- UI framework
- 2D lighting

[INSTANCE MANAGER]
        │
        ▼
    [ENGINE]
        │
┌───────┴───────┐
▼               ▼
[WINDOW]    [CORE SYSTEMS]
                │
    ┌───────────┼───────────┬──────────────┐
    ▼           ▼           ▼              ▼
[RENDERING] [PHYSICS]  [INPUT]     [EVENTS]
│           │          │           │
└─2D        └─Bodies   └─Keyboard  └─Publisher
└─3D        └─Colliders └─Mouse    └─Subscriber
└─Lighting  └─Materials └─Touch
└─Particles └─World
└─Post FX
└─UI

[UTILITIES]
└─ JSON Parser
└─ Logger
└─ Math Library
└─ Timing System
