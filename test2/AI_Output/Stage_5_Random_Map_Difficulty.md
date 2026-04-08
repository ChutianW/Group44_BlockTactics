# Stage 5 — Random Map + Difficulty System Output

## Date
2026-04-08

## Type
Feature Implementation

---

## Goal
Implement random map generation with difficulty-based parameters and constraints.

---

## Tasks Completed

### Person B Tasks (Main Owner)

#### 1. Random Map Generation
```cpp
void Map::generateRandomMap(Difficulty diff) {
    DifficultySettings settings = getDifficultySettings(diff);
    
    clearMap();
    addBorderWalls();
    
    // Place obstacles (if any)
    if (num_obstacles > 0) {
        placeObstacles(num_obstacles);
    }
    
    // Place boxes and targets
    placeBoxesAndTargets(settings.num_boxes);
    
    // Place player
    placePlayer();
    
    saveInitialState();
}
```

#### 2. Placement Constraints

**No Corner Placement:**
```cpp
bool Map::isCorner(int x, int y) const {
    if (x <= 1 || x >= MAP_WIDTH - 2 || y <= 1 || y >= MAP_HEIGHT - 2) {
        return true;
    }
    return false;
}
```

**Targets Not Adjacent to Outer Walls:**
```cpp
bool Map::isAdjacentToOuterWall(int x, int y) const {
    return (x == 1 || x == MAP_WIDTH - 2 || y == 1 || y == MAP_HEIGHT - 2);
}
```

**Box Mobility Validation:**
```cpp
bool Map::hasMinimumMobility(int x, int y) const {
    return countAdjacentFree(x, y) >= 2;
}
```

**Obstacle Path Blocking Prevention:**
```cpp
// Don't place if it would create a blocked corridor
if (adjacent_walls >= 2) continue;
```

#### 3. Difficulty Settings
```cpp
DifficultySettings getDifficultySettings(Difficulty diff) {
    switch (diff) {
        case EASY:
            settings.num_boxes = 3;
            settings.min_obstacles = 0;
            settings.max_obstacles = 0;
            break;
        case MEDIUM:
            settings.num_boxes = 5;
            settings.min_obstacles = 3;
            settings.max_obstacles = 5;
            break;
        case HARD:
            settings.num_boxes = 7;
            settings.min_obstacles = 6;
            settings.max_obstacles = 10;
            break;
    }
}
```

---

### Person A Tasks

#### 1. Difficulty Selection Menu
```cpp
void Game::displayDifficultyMenu() {
    cout << "  [1] Easy   - 3 boxes, no obstacles\n";
    cout << "  [2] Medium - 5 boxes, 3-5 obstacles\n";
    cout << "  [3] Hard   - 7 boxes, 6-10 obstacles\n";
}
```

#### 2. Difficulty Parameter Passing
```cpp
void Game::startNewGame() {
    selectDifficulty();
    game_map.generateRandomMap(current_difficulty);
    // ...
}
```

---

## Merge 5 Checkpoint

### Verified Behavior
- ✅ Easy: 3 boxes, 0 obstacles
- ✅ Medium: 5 boxes, 3-5 obstacles
- ✅ Hard: 7 boxes, 6-10 obstacles
- ✅ No boxes/targets in corners
- ✅ Targets not adjacent to outer walls
- ✅ Boxes have minimum mobility
- ✅ Maps are playable

---

## Constraint Summary

| Constraint | Implementation |
|------------|----------------|
| No corner boxes | `isCorner()` check |
| No corner targets | `isCorner()` check |
| Targets away from walls | `isAdjacentToOuterWall()` check |
| Box mobility | `hasMinimumMobility()` - 2+ free cells |
| No blocked corridors | Adjacent wall count < 2 |

---

## Files Modified

| File | Changes |
|------|---------|
| src/map.h | Added Difficulty enum, DifficultySettings struct, generation methods |
| src/map.cpp | Implemented all generation and constraint functions |
| src/game.cpp | Added difficulty menu and selection |

---

## Output
- Playable random levels with three difficulty settings
- Constraints prevent immediate deadlocks
