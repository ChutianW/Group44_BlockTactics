# Stage 4 — Restart System Output

## Date
2026-04-08

## Type
Feature Implementation

---

## Goal
Implement restart functionality that restores the level to its initial state.

---

## Tasks Completed

### Person A Tasks

#### 1. R Input Handling
Added in `game.cpp`:
```cpp
case 'r':
    restartLevel();
    break;
```

#### 2. Restart Level Function
```cpp
void Game::restartLevel() {
    game_map.restoreInitialState();
    player.setPosition(game_map.getPlayerStartX(), game_map.getPlayerStartY());
    player.resetSteps();
    undo_system.clear();
}
```

#### 3. Player Position Reset
- Added `setPosition()` method to Player struct
- Resets to stored start coordinates

#### 4. Step Count Reset
- Calls `player.resetSteps()` on restart

---

### Person B Tasks

#### 1. Initial Map State Storage
```cpp
void Map::saveInitialState() {
    initial_grid = grid;
    initial_target_positions = target_positions;
}
```

#### 2. Initial Box Positions
- Stored as part of `initial_grid`
- Restored when `restoreInitialState()` called

#### 3. Target State Storage
- `initial_target_positions` vector stores original targets
- Restored on restart

#### 4. Restoration Function
```cpp
void Map::restoreInitialState() {
    grid = initial_grid;
    target_positions = initial_target_positions;
}
```

---

## Merge 4 Checkpoint

### Verified Behavior
- ✅ Press R during level → same map returns
- ✅ Box positions reset to original
- ✅ Player position reset to start
- ✅ Step count reset to 0
- ✅ Undo history cleared
- ✅ Does NOT generate new map

---

## Files Modified

| File | Changes |
|------|---------|
| src/player.h | Added `setPosition()` method |
| src/player.cpp | Implemented `setPosition()` |
| src/map.cpp | Enhanced `saveInitialState()` and `restoreInitialState()` |
| src/game.cpp | Added `restartLevel()` function |

---

## Output
- Stable restart feature complete
- Level can be retried without regeneration
