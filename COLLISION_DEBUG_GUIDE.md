# Collision Debug Guide

## New Debug Output Format

### [BOUNDS] - Box Dimensions and Boundaries
```
[BOUNDS] BodyA: Y[min,max] Dim=height | BodyB: Y[min,max] Dim=height
```
**What it shows:**
- Min/Max Y positions of each collider
- Actual dimensions being used for collision (includes scale)
- Use this to diagnose if colliders are the wrong size

**Example:**
```
[BOUNDS] BodyA: Y[-560,-480] Dim=80 | BodyB: Y[-640,-560] Dim=80
```
This shows BodyA's bottom is at -560, top at -480, with 80 units height.

---

### [COLLISION] - Penetration and Collision Normal
```
[COLLISION] Penetration=X.XX Normal=(x,y,z)
```
**What it shows:**
- How deep the overlap is (in world units)
- Which direction to push objects apart

---

### [COLLISION] - Velocity and Contact State
```
[COLLISION] VelMag=X.XX Pen=X.XX Sleep=0/1 Resting=0/1
```
**What it shows:**
- `VelMag`: Relative velocity magnitude between objects
- `Pen`: Current penetration depth
- `Sleep`: Whether contact will be put to sleep (no more resolution)
- `Resting`: Whether it's a resting contact (low velocity)

---

### [BOUNDS] - Static Body Information
```
[COLLISION] BodyA static=0 BodyB static=1
```
**What it shows:**
- Which objects are static (immovable)

---

### [PENETRATION] - Position Correction
```
[PENETRATION] CorrectionMag=X.XX Slop=X.XX BodyA@(Y)
```
**What it shows:**
- How much the object is being pushed
- Slop value (small buffer zone)
- Current Y position before correction

---

### [POSITION] - Position Changes
```
[POSITION] BodyA Y: OLD ? NEW (?X.XX)
```
**What it shows:**
- Before and after Y position
- Delta (amount moved)
- Use to track if object is sinking or being corrected properly

---

## Diagnosing the Sinking Issue

### Possible Causes:

#### 1. **Incorrect Collider Dimensions**
Check the `[BOUNDS]` output:
```
[BOUNDS] BodyA: Y[-560,-400] Dim=160  ? WRONG! Should be Dim=80 for 80-unit object
```

**Fix:** Verify in your GameObject setup that collider dimensions match your visual size.

---

#### 2. **Collider Position Offset**
If center position is wrong:
```
[BOUNDS] BodyA: Y[-560,-480] Dim=80  ? Correct dimensions
BUT BodyA position=(-560)  ? Is this where you intended?
```

Check that `collider->setPosition()` is being called with the GameObject's position.

---

#### 3. **Scale Not Being Applied**
In `BoxColliderComponent.init()`, verify:
```cpp
auto ownerScale = ownerObj->getTransform().getScale();
collider->setScale(ownerScale);
```

If scale is (1,1,1) but should be (80,80,80), collider will be tiny.

---

#### 4. **Penetration Overcorrection**
Check `[PENETRATION]` and `[POSITION]` together:
```
[PENETRATION] CorrectionMag=50.00
[POSITION] BodyA Y: -560 ? -510 (?50)  ? Object moved 50 units in one frame!
```

If correction is too large, the object sinks too far.

---

## Understanding the Collision Flow

1. **Penetration detected** ? `[BOUNDS]` and `[COLLISION] Penetration=X`
2. **Velocity checked** ? `[COLLISION] VelMag=X Resting=0/1`
3. **Position corrected** ? `[PENETRATION]` and `[POSITION]` (if not resting)
4. **Velocity resolved** ? Object slowed/stopped
5. **Contact sleeping** ? `[COLLISION] Sleep=1` (no more corrections needed)

---

## Quick Diagnostics

### Object sinking halfway into ground:
```
Look for:
[BOUNDS] BodyA: Y[-560,-400]  ? max Y should be -560 (surface level)
         BodyB: Y[-640,-560]  ? this is the ground
```

If BodyA's bottom (minA.y) goes below -640, it's sinking.

### Object bouncing:
```
Look for repeated:
[COLLISION] VelMag=X Resting=0
```

If always showing `Resting=0`, collision velocity never gets low enough.

### Object frozen:
```
Look for:
[COLLISION] Sleep=1
```

Once sleeping, collision won't be resolved again until velocity increases.

