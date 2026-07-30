#include "./PhysicsWorld.h"
#include "./PhysicsConst.h"
#include "./PhysicsManager.h"
#include "./RigidBody.h"
#include "./RigidBody.h"
#include "../Logger.h"
#include <cstdint>
#include <unordered_set>

namespace GUESS::physics {
    void PhysicsWorld::removeBody(RigidBody<GUESS::core::math::Vector3f>* body) {
        bodies3D.erase(
            std::remove(bodies3D.begin(), bodies3D.end(), body),
            bodies3D.end()
        );
        restingFrames3D.erase(body);
        GUESS::core::Logger::log(GUESS::core::Logger::INFO, "Removed 3D rigidbody from physics world");
    }

    void PhysicsWorld::step(float deltaTime) {
        const float dt = deltaTime > 0.0f ? deltaTime : fixedTimeStep;

        // Update 3D physics bodies
        for (auto* body : bodies3D) {
            body->update(dt);
        }

        // Sync colliders to updated rigidbody positions so collision checks use correct positions
        for (auto* body : bodies3D) {
            if (auto* col = body->getCollider()) {
                col->setPosition(body->getPosition());
            }
        }

        checkCollisions();
    }

    void PhysicsWorld::checkCollisions() {
        if (bodies3D.empty()) return;

        size_t batchSize3D = bodies3D.size() / NUM_THREADS;
        if (batchSize3D == 0) batchSize3D = bodies3D.size();

        std::vector<std::future<void>> futures;

        // Shared collection for detected overlapping pairs
        std::vector<std::pair<size_t,size_t>> overlaps;
        std::mutex overlapsMutex;

        // Launch 3D collision detection threads (detection only)
        for (size_t i = 0; i < NUM_THREADS; i++) {
            size_t start = i * batchSize3D;
            size_t end = (i == NUM_THREADS - 1) ? bodies3D.size() : std::min((i + 1) * batchSize3D, bodies3D.size());

            if (start >= end) break;

            futures.push_back(std::async(std::launch::async,
                &PhysicsWorld::processCollisionBatch3D, this, start, end, std::ref(bodies3D), &overlaps, &overlapsMutex));
        }

        // Wait for all threads to complete detection
        for (auto& future : futures) {
            future.wait();
        }

        // Resolve detected overlaps serially to avoid data races
        const float percent = 0.6f;  // Reduced from 0.8 to prevent over-correction and bouncing
        const float baseSlop = 0.005f;  // Keep contact tolerance small to avoid visible sinking
        std::unordered_set<RigidBody<GUESS::core::math::Vector3f>*> supportedBodies;

        for (auto& p : overlaps) {
            size_t i = p.first;
            size_t j = p.second;
            if (i >= bodies3D.size() || j >= bodies3D.size()) continue;

            auto* bodyA = bodies3D[i];
            auto* bodyB = bodies3D[j];
            auto colliderA = bodyA->getCollider();
            auto colliderB = bodyB->getCollider();
            if (!colliderA || !colliderB) continue;

            const auto bodyPtrA = static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(bodyA));
            const auto bodyPtrB = static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(bodyB));
            const auto velA = bodyA->getVelocity();
            const auto velB = bodyB->getVelocity();

            std::string pairMsg = "[PAIR] i=" + std::to_string(i) + " j=" + std::to_string(j) +
                " ptrA=" + std::to_string(bodyPtrA) + " ptrB=" + std::to_string(bodyPtrB) +
                " massA=" + std::to_string(bodyA->getMass()) + " massB=" + std::to_string(bodyB->getMass()) +
                " velA_y=" + std::to_string(velA.y) + " velB_y=" + std::to_string(velB.y);
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, pairMsg);

            auto ca = colliderA->getCenter();
            auto cb = colliderB->getCenter();

            // Calculate world-space dimensions once
            auto dimA = colliderA->getDimensions() * colliderA->getScale();
            auto dimB = colliderB->getDimensions() * colliderB->getScale();

            std::string centersMsg = "[CENTERS] BodyA: col_center=" + std::to_string(ca.y) + " rb_pos=" + std::to_string(bodyA->getPosition().y) + " | BodyB: col_center=" + std::to_string(cb.y) + " rb_pos=" + std::to_string(bodyB->getPosition().y);
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, centersMsg);

            // Calculate adaptive slop based on world-space height (consistent for all uses)
            float avgHeightSize = (dimA.y + dimB.y) / 2.0f;
            float slop = baseSlop * avgHeightSize;
            slop = std::max(slop, 0.01f);

            std::string scaleMsg = "[SCALE] BodyA: storedDim=" + std::to_string(colliderA->getDimensions().y) + " scale=" + std::to_string(colliderA->getScale().y) + " final=" + std::to_string(dimA.y) + " | BodyB: storedDim=" + std::to_string(colliderB->getDimensions().y) + " scale=" + std::to_string(colliderB->getScale().y) + " final=" + std::to_string(dimB.y) + " | FinalSlop=" + std::to_string(slop);
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, scaleMsg);

            GUESS::core::math::Vector3f minA(ca.x - dimA.x*0.5f, ca.y - dimA.y*0.5f, ca.z - dimA.z*0.5f);
            GUESS::core::math::Vector3f maxA(ca.x + dimA.x*0.5f, ca.y + dimA.y*0.5f, ca.z + dimA.z*0.5f);
            GUESS::core::math::Vector3f minB(cb.x - dimB.x*0.5f, cb.y - dimB.y*0.5f, cb.z - dimB.z*0.5f);
            GUESS::core::math::Vector3f maxB(cb.x + dimB.x*0.5f, cb.y + dimB.y*0.5f, cb.z + dimB.z*0.5f);

            std::string boundsMsg = "[BOUNDS] BodyA: Y[" + std::to_string(minA.y) + "," + std::to_string(maxA.y) + "] Dim=" + std::to_string(dimA.y) + " | BodyB: Y[" + std::to_string(minB.y) + "," + std::to_string(maxB.y) + "] Dim=" + std::to_string(dimB.y);
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, boundsMsg);

            float overlapX = std::min(maxA.x, maxB.x) - std::max(minA.x, minB.x);
            float overlapY = std::min(maxA.y, maxB.y) - std::max(minA.y, minB.y);
            float overlapZ = std::min(maxA.z, maxB.z) - std::max(minA.z, minB.z);

            // Keep exact-touching contacts (overlap == 0) so support/sleep logic remains stable.
            if (overlapX < 0 || overlapY < 0 || overlapZ < 0) continue; // separated

            // Find smallest penetration axis
            float penetration = overlapX;
            GUESS::core::math::Vector3f normal((cb.x > ca.x) ? 1.0f : -1.0f, 0.0f, 0.0f);
            if (overlapY < penetration) { penetration = overlapY; normal = GUESS::core::math::Vector3f(0.0f, (cb.y > ca.y) ? 1.0f : -1.0f, 0.0f); }
            if (overlapZ < penetration) { penetration = overlapZ; normal = GUESS::core::math::Vector3f(0.0f, 0.0f, (cb.z > ca.z) ? 1.0f : -1.0f); }

            std::string penetrationMsg = "[COLLISION] Penetration=" + std::to_string(penetration) + " Normal=(" + std::to_string(normal.x) + "," + std::to_string(normal.y) + "," + std::to_string(normal.z) + ")";
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, penetrationMsg);

            float invMassA = bodyA->getIsStatic() ? 0.0f : 1.0f / bodyA->getMass();
            float invMassB = bodyB->getIsStatic() ? 0.0f : 1.0f / bodyB->getMass();

            std::string massMsg = "[COLLISION] BodyA static=" + std::to_string(bodyA->getIsStatic()) + " BodyB static=" + std::to_string(bodyB->getIsStatic());
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, massMsg);

            // Detect resting contacts using relative motion at contact.
            auto relativeVel = bodyB->getVelocity() - bodyA->getVelocity();
            float relVelMagnitude = relativeVel.magnitude();
            bool isRestingContact = relVelMagnitude < GUESS::physics::RESTING_VELOCITY_THRESHOLD;

            bool shouldSleep = (relVelMagnitude < GUESS::physics::SLEEP_LINEAR_VELOCITY_THRESHOLD) && 
                               (penetration < GUESS::physics::SLEEP_PENETRATION_THRESHOLD);

            std::string sleepMsg = "[COLLISION] VelMag=" + std::to_string(relVelMagnitude) + " Pen=" + std::to_string(penetration) + " Sleep=" + std::to_string(shouldSleep) + " Resting=" + std::to_string(isRestingContact);
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, sleepMsg);

            if (invMassA + invMassB > 0.0f) {
                std::string slopCalcMsg = "[SLOP_CALC] avgHeight=" + std::to_string(avgHeightSize) + " baseSlop=" + std::to_string(baseSlop) + " calculatedSlop=" + std::to_string(slop) + " penetration=" + std::to_string(penetration);
                GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, slopCalcMsg);

                // Resting contacts against static bodies should depenetrate fully.
                bool aggressiveRestingCorrection = isRestingContact && (bodyA->getIsStatic() || bodyB->getIsStatic());
                float effectiveSlop = aggressiveRestingCorrection ? 0.0f : slop;
                float correctionPercent = aggressiveRestingCorrection ? 1.0f : percent;

                // Correct whenever penetration exceeds effective slop.
                bool shouldCorrect = penetration > effectiveSlop;

                std::string correctionDecision = "[CORRECTION] isResting=" + std::to_string(isRestingContact) + " pen=" + std::to_string(penetration) + " slop=" + std::to_string(slop) + " effSlop=" + std::to_string(effectiveSlop) + " shouldCorrect=" + std::to_string(shouldCorrect);
                GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, correctionDecision);

                if (shouldCorrect) {
                    bool snappedToSurface = false;

                    // For static-vs-dynamic vertical contacts, snap exactly to the supporting surface.
                    if (std::fabs(normal.y) > 0.5f && (bodyA->getIsStatic() != bodyB->getIsStatic())) {
                        if (!bodyA->getIsStatic() && bodyB->getIsStatic()) {
                            auto snappedPosA = bodyA->getPosition();
                            if (normal.y < 0.0f) {
                                // BodyA is above BodyB
                                snappedPosA.y = maxB.y + dimA.y * 0.5f;
                            } else {
                                // BodyA is below BodyB
                                snappedPosA.y = minB.y - dimA.y * 0.5f;
                            }
                            bodyA->setPosition(snappedPosA);
                            if (auto c = bodyA->getCollider()) c->setPosition(snappedPosA);
                            snappedToSurface = true;
                        } else if (!bodyB->getIsStatic() && bodyA->getIsStatic()) {
                            auto snappedPosB = bodyB->getPosition();
                            if (normal.y > 0.0f) {
                                // BodyB is above BodyA
                                snappedPosB.y = maxA.y + dimB.y * 0.5f;
                            } else {
                                // BodyB is below BodyA
                                snappedPosB.y = minA.y - dimB.y * 0.5f;
                            }
                            bodyB->setPosition(snappedPosB);
                            if (auto c = bodyB->getCollider()) c->setPosition(snappedPosB);
                            snappedToSurface = true;
                        }

                        if (snappedToSurface) {
                            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, "[SNAP] Applied exact surface snap for static-dynamic Y contact");
                        }
                    }

                    if (!snappedToSurface) {
                    // Correct penetration beyond effective slop.
                    float penToCorrect = std::max(0.0f, penetration - effectiveSlop);
                    if (penToCorrect <= 0.0f) {
                        continue;
                    }
                    float correctionMag = penToCorrect / (invMassA + invMassB) * correctionPercent;
                    GUESS::core::math::Vector3f correction = normal * correctionMag;

                    std::string correctionMsg = "[PENETRATION] CorrectionMag=" + std::to_string(correctionMag) +
                        " PenToCorrect=" + std::to_string(penToCorrect) + " Percent=" + std::to_string(correctionPercent) +
                        " EffSlop=" + std::to_string(effectiveSlop) + " AggressiveRest=" + std::to_string(aggressiveRestingCorrection);
                    GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, correctionMsg);

                    if (invMassA > 0.0f) {
                        auto oldPosA = bodyA->getPosition();
                        auto newPosA = bodyA->getPosition() - correction * invMassA;
                        bodyA->setPosition(newPosA);
                        if (auto c = bodyA->getCollider()) c->setPosition(newPosA);
                        std::string posMsgA = "[POSITION] BodyA Y: " + std::to_string(oldPosA.y) + " → " + std::to_string(newPosA.y) + " (Δ" + std::to_string(newPosA.y - oldPosA.y) + ")";
                        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, posMsgA);

                        auto verifyPosA = bodyA->getPosition();
                        auto verifyColA = colliderA->getCenter();
                        std::string verifyMsgA = "[POSITION_VERIFY] BodyA ptr=" + std::to_string(bodyPtrA) +
                            " rb_y=" + std::to_string(verifyPosA.y) + " col_y=" + std::to_string(verifyColA.y);
                        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, verifyMsgA);
                    }
                    if (invMassB > 0.0f) {
                        auto oldPosB = bodyB->getPosition();
                        auto newPosB = bodyB->getPosition() + correction * invMassB;
                        bodyB->setPosition(newPosB);
                        if (auto c = bodyB->getCollider()) c->setPosition(newPosB);
                        std::string posMsgB = "[POSITION] BodyB Y: " + std::to_string(oldPosB.y) + " → " + std::to_string(newPosB.y) + " (Δ" + std::to_string(newPosB.y - oldPosB.y) + ")";
                        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, posMsgB);

                        auto verifyPosB = bodyB->getPosition();
                        auto verifyColB = colliderB->getCenter();
                        std::string verifyMsgB = "[POSITION_VERIFY] BodyB ptr=" + std::to_string(bodyPtrB) +
                            " rb_y=" + std::to_string(verifyPosB.y) + " col_y=" + std::to_string(verifyColB.y);
                        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, verifyMsgB);
                    }
                    }
                } else {
                    GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, "[CORRECTION] Skipping correction (penetration within slop)");
                }

                // After positional correction, also resolve velocities to prevent re-penetration jitter
                // Use the penetration analysis normal instead of recalculating from centers
                resolveCollision(bodyA, bodyB, normal);

                // Mark grounded/supporting contacts for sleeping logic.
                // normal points from A to B. For A on top of static B, normal.y is negative.
                if (isRestingContact && (bodyA->getIsStatic() != bodyB->getIsStatic())) {
                    if (!bodyA->getIsStatic() && bodyB->getIsStatic() && normal.y < -0.5f) {
                        auto v = bodyA->getVelocity();
                        if (v.y < 0.0f) {
                            v.y = 0.0f;
                            bodyA->setVelocity(v);
                        }
                        supportedBodies.insert(bodyA);
                    }
                    if (!bodyB->getIsStatic() && bodyA->getIsStatic() && normal.y > 0.5f) {
                        auto v = bodyB->getVelocity();
                        if (v.y < 0.0f) {
                            v.y = 0.0f;
                            bodyB->setVelocity(v);
                        }
                        supportedBodies.insert(bodyB);
                    }
                }
            }

            colliderA->onCollisionEnter(*colliderB);
            colliderB->onCollisionEnter(*colliderA);
        }

        // Apply basic persistent sleeping for bodies stably supported by static colliders.
        for (auto* body : bodies3D) {
            if (!body || body->getIsStatic()) {
                continue;
            }

            const bool isSupported = supportedBodies.find(body) != supportedBodies.end();
            const float speed = body->getVelocity().magnitude();

            if (isSupported && speed < GUESS::physics::SLEEP_LINEAR_VELOCITY_THRESHOLD) {
                int& frames = restingFrames3D[body];
                frames += 1;
                if (frames >= REST_FRAMES_TO_SLEEP) {
                    body->setSleeping(true);
                }
            } else {
                restingFrames3D[body] = 0;
                if (body->getSleeping()) {
                    body->setSleeping(false);
                }
            }
        }
    }

    void PhysicsWorld::resolveCollision(RigidBody<GUESS::core::math::Vector3f>* bodyA, RigidBody<GUESS::core::math::Vector3f>* bodyB) {
        // unchanged existing impulse resolution (kept for API completeness)
        GUESS::core::math::Vector3f posA = bodyA->getPosition();
        GUESS::core::math::Vector3f posB = bodyB->getPosition();
        auto colliderA = bodyA->getCollider();
        auto colliderB = bodyB->getCollider();
        if (colliderA && colliderB) {
            posA = colliderA->getCenter();
            posB = colliderB->getCenter();
        }

        GUESS::core::math::Vector3f normal = (posB - posA).normalized();
        auto relativeVel = bodyB->getVelocity() - bodyA->getVelocity();
        float velAlongNormal = relativeVel.dot(normal);

        std::string velMsg = "[IMPULSE] velAlongNormal=" + std::to_string(velAlongNormal) + " relVel=(" + std::to_string(relativeVel.x) + "," + std::to_string(relativeVel.y) + "," + std::to_string(relativeVel.z) + ") normal=(" + std::to_string(normal.x) + "," + std::to_string(normal.y) + "," + std::to_string(normal.z) + ")";
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, velMsg);

        if (velAlongNormal > 0.0f) {
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, "[IMPULSE] Early return: velocities already separating");
            return;
        }

        float restitution = std::min(bodyA->getRestitution(), bodyB->getRestitution());
        float invMassA = bodyA->getIsStatic() ? 0.0f : 1.0f / bodyA->getMass();
        float invMassB = bodyB->getIsStatic() ? 0.0f : 1.0f / bodyB->getMass();

        if (invMassA + invMassB <= 0.0f) {
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, "[IMPULSE] Early return: both bodies static");
            return;
        }

        float j = -(1.0f + restitution) * velAlongNormal;
        j /= (invMassA + invMassB);
        GUESS::core::math::Vector3f impulse = normal * j;

        std::string impulseMsg = "[IMPULSE] restitution=" + std::to_string(restitution) + " j=" + std::to_string(j) + " impulse=(" + std::to_string(impulse.x) + "," + std::to_string(impulse.y) + "," + std::to_string(impulse.z) + ")";
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, impulseMsg);

        if (invMassA > 0.0f) {
            auto oldVelA = bodyA->getVelocity();
            auto newVelA = bodyA->getVelocity() - impulse * invMassA;
            bodyA->setVelocity(newVelA);
            std::string velMsgA = "[IMPULSE] BodyA velocity changed from (" + std::to_string(oldVelA.x) + "," + std::to_string(oldVelA.y) + "," + std::to_string(oldVelA.z) + ") to (" + std::to_string(newVelA.x) + "," + std::to_string(newVelA.y) + "," + std::to_string(newVelA.z) + ")";
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, velMsgA);
        }
        if (invMassB > 0.0f) {
            auto oldVelB = bodyB->getVelocity();
            auto newVelB = bodyB->getVelocity() + impulse * invMassB;
            bodyB->setVelocity(newVelB);
            std::string velMsgB = "[IMPULSE] BodyB velocity changed from (" + std::to_string(oldVelB.x) + "," + std::to_string(oldVelB.y) + "," + std::to_string(oldVelB.z) + ") to (" + std::to_string(newVelB.x) + "," + std::to_string(newVelB.y) + "," + std::to_string(newVelB.z) + ")";
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, velMsgB);
        }
    }

    void PhysicsWorld::resolveCollision(RigidBody<GUESS::core::math::Vector3f>* bodyA, RigidBody<GUESS::core::math::Vector3f>* bodyB, const GUESS::core::math::Vector3f& normal) {
        // Overload that uses the collision normal from penetration analysis (more stable)
        auto relativeVel = bodyB->getVelocity() - bodyA->getVelocity();
        float velAlongNormal = relativeVel.dot(normal);

        std::string velCheckMsg = "[IMPULSE_CHECK] relVel=(" + std::to_string(relativeVel.x) + "," + std::to_string(relativeVel.y) + "," + std::to_string(relativeVel.z) + 
            ") velAlongNormal=" + std::to_string(velAlongNormal) + " normal=(" + std::to_string(normal.x) + "," + std::to_string(normal.y) + "," + std::to_string(normal.z) + ")";
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, velCheckMsg);

        if (velAlongNormal > 0.0f) {
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, "[IMPULSE] Early return: velocities already separating");
            return;
        }

        float invMassA = bodyA->getIsStatic() ? 0.0f : 1.0f / bodyA->getMass();
        float invMassB = bodyB->getIsStatic() ? 0.0f : 1.0f / bodyB->getMass();

        std::string massCheckMsg = "[IMPULSE_MASS] invMassA=" + std::to_string(invMassA) + " invMassB=" + std::to_string(invMassB) + " sum=" + std::to_string(invMassA + invMassB);
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, massCheckMsg);

        if (invMassA + invMassB <= 0.0f) {
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, "[IMPULSE] Early return: both bodies static");
            return;
        }

        // Detect resting contact: if relative velocity is very small, use zero restitution
        float relVelMagnitude = relativeVel.magnitude();
        bool isRestingContact = relVelMagnitude < GUESS::physics::RESTING_VELOCITY_THRESHOLD;

        std::string restingCheckMsg = "[IMPULSE_RESTING] relVelMag=" + std::to_string(relVelMagnitude) + " threshold=" + std::to_string(GUESS::physics::RESTING_VELOCITY_THRESHOLD) + 
            " isResting=" + std::to_string(isRestingContact);
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, restingCheckMsg);

        float restitution = isRestingContact ? GUESS::physics::RESTING_RESTITUTION : std::min(bodyA->getRestitution(), bodyB->getRestitution());

        std::string restitutionMsg = "[IMPULSE_RESTITUTION] isResting=" + std::to_string(isRestingContact) + " restitution=" + std::to_string(restitution);
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, restitutionMsg);

        float j = -(1.0f + restitution) * velAlongNormal;
        j /= (invMassA + invMassB);
        GUESS::core::math::Vector3f impulse = normal * j;

        std::string impulseCalcMsg = "[IMPULSE_CALC] velAlongNormal=" + std::to_string(velAlongNormal) + " restitution=" + std::to_string(restitution) + 
            " j=" + std::to_string(j) + " impulse=(" + std::to_string(impulse.x) + "," + std::to_string(impulse.y) + "," + std::to_string(impulse.z) + ")";
        GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, impulseCalcMsg);

        if (invMassA > 0.0f) {
            auto oldVelA = bodyA->getVelocity();
            auto newVelA = bodyA->getVelocity() - impulse * invMassA;

            // Apply aggressive damping to resting contacts to prevent oscillation
            if (isRestingContact) {
                // Remove velocity component along the collision normal (prevents bouncing along that axis)
                GUESS::core::math::Vector3f velAlongNormalVec = normal * (newVelA.dot(normal));
                newVelA = newVelA - velAlongNormalVec;
                std::string dampingMsg = "[IMPULSE_DAMPING_A] beforeDamping=(" + std::to_string((bodyA->getVelocity() - impulse * invMassA).x) + "," + 
                    std::to_string((bodyA->getVelocity() - impulse * invMassA).y) + "," + std::to_string((bodyA->getVelocity() - impulse * invMassA).z) + ") afterDamping=(" + 
                    std::to_string(newVelA.x) + "," + std::to_string(newVelA.y) + "," + std::to_string(newVelA.z) + ")";
                GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, dampingMsg);
            }

            bodyA->setVelocity(newVelA);
            std::string velChangeMsg = "[IMPULSE_VELOCITY_A] old=(" + std::to_string(oldVelA.x) + "," + std::to_string(oldVelA.y) + "," + std::to_string(oldVelA.z) + 
                ") new=(" + std::to_string(newVelA.x) + "," + std::to_string(newVelA.y) + "," + std::to_string(newVelA.z) + ")";
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, velChangeMsg);
        }
        if (invMassB > 0.0f) {
            auto oldVelB = bodyB->getVelocity();
            auto newVelB = bodyB->getVelocity() + impulse * invMassB;

            // Apply aggressive damping to resting contacts to prevent oscillation
            if (isRestingContact) {
                // Remove velocity component along the collision normal (prevents bouncing along that axis)
                GUESS::core::math::Vector3f velAlongNormalVec = normal * (newVelB.dot(normal));
                newVelB = newVelB - velAlongNormalVec;
                std::string dampingMsg = "[IMPULSE_DAMPING_B] beforeDamping=(" + std::to_string((bodyB->getVelocity() + impulse * invMassB).x) + "," + 
                    std::to_string((bodyB->getVelocity() + impulse * invMassB).y) + "," + std::to_string((bodyB->getVelocity() + impulse * invMassB).z) + ") afterDamping=(" + 
                    std::to_string(newVelB.x) + "," + std::to_string(newVelB.y) + "," + std::to_string(newVelB.z) + ")";
                GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, dampingMsg);
            }

            bodyB->setVelocity(newVelB);
            std::string velChangeMsg = "[IMPULSE_VELOCITY_B] old=(" + std::to_string(oldVelB.x) + "," + std::to_string(oldVelB.y) + "," + std::to_string(oldVelB.z) + 
                ") new=(" + std::to_string(newVelB.x) + "," + std::to_string(newVelB.y) + "," + std::to_string(newVelB.z) + ")";
            GUESS::core::Logger::log(GUESS::core::Logger::DEBUG, velChangeMsg);
        }
    }

    void PhysicsWorld::processCollisionBatch3D(size_t start, size_t end,
        std::vector<RigidBody<GUESS::core::math::Vector3f>*>& bodies,
        std::vector<std::pair<size_t,size_t>>* outPairs, std::mutex* outMutex) {
        for (size_t i = start; i < end; i++) {
            for (size_t j = i + 1; j < bodies.size(); j++) {
                auto colliderA = bodies[i]->getCollider();
                auto colliderB = bodies[j]->getCollider();

                if (colliderA && colliderB && colliderA->checkCollision(*colliderB)) {
                    std::lock_guard<std::mutex> lock(*outMutex);
                    outPairs->emplace_back(i, j);
                }
            }
        }
    }

    void PhysicsWorld::updateThermodynamics(float deltaTime)
    {
        std::vector<std::shared_ptr<ThermodynamicBody>> thermalBodies = envManager.getThermalBodies();
        // Update all thermal bodies in one pass
        for (auto body : thermalBodies) {
            body->updateTemperature(deltaTime, this);
        }
        // Process heat transfer in batches
        for (size_t i = 0; i < thermalBodies.size(); i += BATCH_SIZE) {
            processThermalBatch(i, std::min(i + BATCH_SIZE, thermalBodies.size()));
        }
    }

    void PhysicsWorld::processThermalBatch(size_t start, size_t end) {
        std::vector<std::shared_ptr<ThermodynamicBody>> thermalBodies = envManager.getThermalBodies();
        for (size_t i = start; i < end; i++) {
            for (size_t j = i + 1; j < thermalBodies.size(); j++) {
                thermalBodies[i]->transferHeat(thermalBodies[j].get());
            }
        }
    }
}
