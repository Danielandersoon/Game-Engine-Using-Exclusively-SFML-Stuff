#ifndef PHYSICS_CONST_H
#define PHYSICS_CONST_H

namespace GUESS::physics {
    // Gravity is negative to accelerate objects toward decreasing Y in world/render coordinates.
    constexpr float GRAVITY = -9.81f;

    // Collision damping constants
    constexpr float RESTING_VELOCITY_THRESHOLD = 0.5f;  // Velocity threshold to consider object at rest
    constexpr float RESTING_RESTITUTION = 0.0f;          // No bounce for resting contacts
    constexpr float CONTACT_DAMPING = 0.95f;             // Damping factor for velocity along collision normal (friction-like)

    // Contact sleeping - prevent oscillation around equilibrium
    constexpr float SLEEP_LINEAR_VELOCITY_THRESHOLD = 0.1f;   // If velocity is below this, allow sleeping (increased from 0.05)
    constexpr float SLEEP_PENETRATION_THRESHOLD = 0.02f;      // Do not sleep while visibly intersecting

    // Legacy threshold retained for compatibility with older logs/settings.
    constexpr float RESTING_CORRECTION_PENETRATION_THRESHOLD = 0.1f;
}

#endif