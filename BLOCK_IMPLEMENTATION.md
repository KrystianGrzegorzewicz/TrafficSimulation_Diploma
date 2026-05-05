## Block Class Implementation

### Overview
The enhanced `Block` class now represents dynamic obstacles in traffic simulation that can toggle between active ("on") and inactive ("off") states, mimicking traffic lights or pedestrians crossing at intersections.

### Key Features

#### 1. **Timing Properties**
```cpp
float onDuration;      // How long the block remains active (seconds)
float offDuration;     // How long the block remains inactive (seconds)
float elapsedTime;     // Current time in the cycle
bool isCurrentlyOn;    // Current state
```

**Usage Example:**
```cpp
// Traffic light that's green for 5 seconds, red for 5 seconds
Block trafficLight(0, 0, 5, 5, 5.0f, 5.0f);

// Pedestrian crossing: active for 3 seconds, inactive for 7 seconds
Block pedestrianCrossing(-10, -5, 10, 5, 3.0f, 7.0f);
```

#### 2. **State Management**
- `update(float dt)`: Updates block state based on elapsed time
- `isActive()`: Returns true if block is currently "on"
- `getProgress()`: Returns current cycle progress (0.0 to 1.0)

#### 3. **Collision Detection Methods**
- `getDistanceToPoint(Vec2 point)`: Calculates shortest distance from a point to the block
- `containsPoint(Vec2 point)`: Checks if a point is inside the block
- `getCenter()`: Returns block's center position

### Integration with Car Behavior

#### BlockPerception Module
The `BlockPerception` class handles hazard detection:

```cpp
// In Car::update()
Perception perceptionWithBlocks = perception;
BlockHazard blockHazard;

// Detect nearest active block
BlockPerception::updateBlockPerception(blocks, perception.self, blockHazard);
perceptionWithBlocks.blockHazard = blockHazard;
perceptionWithBlocks.hasBlockHazard = (blockHazard.blockIndex >= 0);
```

#### Behavior Model Integration
When a car detects an active block, it triggers defensive driving:

```cpp
// If block is active and within reaction distance
if (perception.hasBlockHazard && perception.blockHazard.isActive)
{
    float blockDecel = computeBlockAvoidanceDeceleration(
        currentSpeed,
        maxDecel,
        perception
    );
    accel = std::min(accel, blockDecel);
}
```

### Deceleration Strategy

The `computeBlockAvoidanceDeceleration()` implements three-phase braking:

1. **Emergency Zone** (< 5 meters): Full deceleration (-maxDecel)
2. **Reaction Zone** (5-20 meters): Gradual deceleration based on distance
3. **Safe Zone** (> 20 meters): No additional deceleration

```cpp
if (distance < 5.0f)
    return -maxDecel;  // Emergency brake
else if (distance < 20.0f)
    return -maxDecel * (1.0f - (distance / 20.0f));
else
    return 0.0f;  // No braking
```

### Simulation Loop Integration

#### Update Sequence
```cpp
void Simulation::step(float dt)
{
    // 1. Update block states (toggle on/off based on timers)
    updateBlocks(dt);
    
    // 2. Build car states
    std::vector<CarState> states;
    for (const auto& car : cars)
        states.push_back({ car.getPosition(), ... });
    
    // 3. Update perception and behavior (with block awareness)
    for (size_t i = 0; i < cars.size(); i++)
    {
        Perception p;
        p.self = states[i];
        updatePerception(p, states);
        
        // Car reacts to blocks during update
        cars[i].update(dt, p, blocks);
    }
    
    // 4. Remove finished cars
    cars.erase(finished cars);
}

void Simulation::updateBlocks(float dt)
{
    for (auto& block : blocks)
        block.update(dt);
}
```

### Visualization

The Python visualizer already supports block rendering:

```python
def draw_blocks():
    for block in blocks:
        x1, y1 = to_screen(block["x1"], block["y1"])
        x2, y2 = to_screen(block["x2"], block["y2"])
        
        # Render as reddish rectangle
        pygame.draw.rect(screen, (100, 50, 50), (x1, y1, x2 - x1, y2 - y1))
```

Blocks can be enhanced with visual states:
- **Active (ON)**: Bright color (e.g., red for traffic light)
- **Inactive (OFF)**: Dim color (e.g., dark for traffic light)

### Example Configurations

#### Symmetric Traffic Light (Intersection)
```cpp
// Alternating: 5s red, 5s green
Block trafficLightN(0, 10, 2, 12, 5.0f, 5.0f);
Block trafficLightE(10, 0, 12, 2, 5.0f, 5.0f);
```

#### Pedestrian Crossing (Random Timing)
```cpp
// Active 3s for crossing, inactive 7s for wait
Block pedestrianCross(-8, -2, 8, 2, 3.0f, 7.0f);
```

#### Temporary Obstruction
```cpp
// Always on (construction zone)
Block construction(20, 20, 25, 25, 999999.0f, 0.0f);
```

### Performance Considerations

- **Lightweight**: Each block only stores 2 Vec2 positions + timing info
- **Per-frame cost**: O(n) distance calculations in BlockPerception (n = number of blocks)
- **Efficient collision**: Early exit when block is inactive
- **Scalable**: Can handle dozens of blocks without performance impact

### Future Enhancements

1. **Spatial partitioning**: Grid-based lookup for faster block detection
2. **Synchronized traffic lights**: Coordinate multiple blocks
3. **Dynamic block creation**: Road construction events
4. **Block types**: Different behavior for lights vs pedestrians
5. **Stochastic timing**: Random intervals for realistic pedestrian crossings
