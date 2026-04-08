# Stage 2 — Core Gameplay Logic Output

## Date
2026-04-06

## Type
Feature Implementation

---

## Goal
Implement player movement with W/A/S/D, collision checking, and box pushing logic.

---

## Tasks Completed

### Person A Tasks (Movement & Box Pushing)

#### 1. Player Movement with W/A/S/D
Implemented in `player.cpp`:

```cpp
Direction getInputDirection(char input) {
    switch (input) {
        case 'w': return UP;
        case 's': return DOWN;
        case 'a': return LEFT;
        case 'd': return RIGHT;
    }
}
```

#### 2. Collision Checking
- Walls block movement
- Obstacles block movement
- Empty cells allow movement
- Targets allow movement

```cpp
bool canMove(const Player& player, const Map& game_map, Direction dir) {
    int dx, dy;
    getDirectionOffset(dir, dx, dy);
    int new_x = player.x + dx;
    int new_y = player.y + dy;

    if (!game_map.isValidPosition(new_x, new_y)) {
        return false;
    }

    char target_cell = game_map.getCell(new_x, new_y);

    // Can move to empty space or target
    if (target_cell == EMPTY || target_cell == TARGET) {
        return true;
    }

    // Can push box if space behind it is free
    if (target_cell == BOX || target_cell == BOX_ON_TARGET) {
        return canPushBox(game_map, new_x, new_y, dir);
    }

    return false;
}
```

#### 3. Box Pushing Logic
- Push one box at a time
- Cannot push multiple boxes
- Cannot push into walls/obstacles

```cpp
bool canPushBox(const Map& game_map, int box_x, int box_y, Direction dir) {
    int dx, dy;
    getDirectionOffset(dir, dx, dy);
    int behind_x = box_x + dx;
    int behind_y = box_y + dy;

    if (!game_map.isValidPosition(behind_x, behind_y)) {
        return false;
    }

    char behind_cell = game_map.getCell(behind_x, behind_y);
    return behind_cell == EMPTY || behind_cell == TARGET;
}
```

#### 4. Move Execution
```cpp
bool movePlayer(Player& player, Map& game_map, Direction dir) {
    if (!canMove(player, game_map, dir)) {
        return false;
    }

    // Handle box pushing
    if (target_cell == BOX || target_cell == BOX_ON_TARGET) {
        // Move box to new position
        if (game_map.isTarget(box_new_x, box_new_y)) {
            game_map.setCell(box_new_x, box_new_y, BOX_ON_TARGET);
        } else {
            game_map.setCell(box_new_x, box_new_y, BOX);
        }
    }

    // Update player position
    game_map.setCell(new_x, new_y, PLAYER);
    player.x = new_x;
    player.y = new_y;
    player.incrementSteps();

    return true;
}
```

---

### Person B Tasks (Map State)

#### 1. Expanded Map State Representation
- Added `BOX_ON_TARGET` symbol for tracking
- Target positions stored separately in `vector<pair<int, int>>`

#### 2. Boxes and Targets in Map
Sample map includes:
- 3 boxes at positions (4,3), (5,4), (3,5)
- 3 targets at positions (7,4), (3,6), (7,7)

#### 3. Target Position Tracking
```cpp
vector<pair<int, int>> target_positions;

void Map::addTarget(int x, int y) {
    target_positions.push_back(make_pair(x, y));
}

bool Map::isTarget(int x, int y) const {
    for (const auto& pos : target_positions) {
        if (pos.first == x && pos.second == y) {
            return true;
        }
    }
    return false;
}
```

#### 4. Clean Print Logic
```cpp
void Map::printMap() const {
    cout << "\n";
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            cout << grid[y][x];
        }
        cout << "\n";
    }
    cout << "\n";
}
```

---

## Merge 2 Checkpoint

### Joint Tasks Verified
- ✅ Movement logic connected to map content
- ✅ Player can push boxes
- ✅ Box moves onto empty cells
- ✅ Blocked pushes prevented

### Expected Behavior
- Player moves correctly with W/A/S/D
- Box pushing works (single box only)
- No illegal overlap
- No broken map state
- Basic Sokoban gameplay functional

---

## Key Implementation Details

### Direction Enum
```cpp
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};
```

### Direction Offset Calculation
```cpp
void getDirectionOffset(Direction dir, int& dx, int& dy) {
    dx = 0;
    dy = 0;
    switch (dir) {
        case UP:    dy = -1; break;
        case DOWN:  dy = 1;  break;
        case LEFT:  dx = -1; break;
        case RIGHT: dx = 1;  break;
    }
}
```

---

## Verification
- Movement in all 4 directions works
- Wall collision prevents movement
- Box pushing follows Sokoban rules
- Map state remains consistent after moves
- Step counter increments on valid moves
