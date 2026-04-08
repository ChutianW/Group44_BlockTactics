# Stage 3 — Target Logic + Win Condition Output

## Date
2026-04-06

## Type
Feature Implementation

---

## Goal
Implement target state tracking, box-on-target logic, and win condition detection.

---

## Tasks Completed

### Person A Tasks (Win Condition)

#### 1. Win Condition Checking
Implemented in `main.cpp`:

```cpp
bool checkWinCondition(const Map& game_map) {
    int boxes_on_targets = 0;
    for (int y = 0; y < game_map.getHeight(); y++) {
        for (int x = 0; x < game_map.getWidth(); x++) {
            if (game_map.getCell(x, y) == BOX_ON_TARGET) {
                boxes_on_targets++;
            }
        }
    }
    return boxes_on_targets == game_map.getTargetCount();
}
```

#### 2. Target Completion Counting
Displays current progress in game status:

```cpp
void displayGameStatus(const Player& player, int target_count, int completed_count) {
    cout << "========================================\n";
    cout << "  Steps: " << player.getSteps();
    cout << "  |  Targets: " << completed_count << "/" << target_count << "\n";
    cout << "========================================\n";
}
```

#### 3. Game Loop Win Detection
```cpp
// In main game loop
if (checkWinCondition(game_map)) {
    level_complete = true;
    cout << "========================================\n";
    cout << "   CONGRATULATIONS! Level Complete!\n";
    cout << "   Steps taken: " << player.getSteps() << "\n";
    cout << "========================================\n";
}
```

---

### Person B Tasks (Target State Tracking)

#### 1. Target State Tracking System
Using separate target position storage:

```cpp
// In Map class
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

int Map::getTargetCount() const {
    return target_positions.size();
}
```

#### 2. Box-on-Target Logic
When a box is pushed onto a target:

```cpp
// In movePlayer() - player.cpp
if (target_cell == BOX || target_cell == BOX_ON_TARGET) {
    int box_new_x = new_x + dx;
    int box_new_y = new_y + dy;

    // Move box to new position
    if (game_map.isTarget(box_new_x, box_new_y)) {
        game_map.setCell(box_new_x, box_new_y, BOX_ON_TARGET);  // Box on target
    } else {
        game_map.setCell(box_new_x, box_new_y, BOX);  // Regular box
    }

    // Restore target if box was on target
    if (target_cell == BOX_ON_TARGET) {
        game_map.setCell(new_x, new_y, TARGET);  // Restore target
    } else {
        game_map.setCell(new_x, new_y, EMPTY);
    }
}
```

#### 3. Rendering for Completed Targets
Symbol differentiation:
- `$` = Box (not on target)
- `*` = Box on target (completed)
- `^` = Empty target (not completed)

#### 4. State Flag Approach
Using `BOX_ON_TARGET = '*'` as special state:
- Visual distinction between completed and incomplete
- Easy win condition checking
- Target restoration when box moves away

---

## Merge 3 Checkpoint

### Joint Tasks Verified
- ✅ Box moved onto target changes to `*`
- ✅ Completed target counted logically
- ✅ Level ends only when ALL targets filled
- ✅ Visual state matches logical state

### Win Condition Logic
```
Win = (count of BOX_ON_TARGET cells) == (total target count)
```

---

## Implementation Details

### Target Interaction Flow

1. **Box pushed onto target:**
   - Cell changes from `^` to `*`
   - Completed count increases

2. **Box pushed off target:**
   - Cell changes from `*` to `^`
   - Completed count decreases

3. **Player walks on target:**
   - Player shown as `@`
   - Target tracked separately
   - When player leaves, target `^` restored

### State Transitions

| Before | Action | After |
|--------|--------|-------|
| `^` (target) | Box pushed onto | `*` (box on target) |
| `*` (box on target) | Box pushed off | `^` (target restored) |
| `$` (box) | Pushed to empty | `$` (box) |
| `$` (box) | Pushed to target | `*` (box on target) |

---

## Verification

### Test Cases
1. ✅ Push box onto target → shows `*`
2. ✅ Push box off target → target `^` restored
3. ✅ All targets filled → win message displayed
4. ✅ Partial completion → game continues
5. ✅ Step count shown on win

### Expected Output on Win
```
========================================
   CONGRATULATIONS! Level Complete!
   Steps taken: XX
========================================
```

---

## Files Modified

| File | Changes |
|------|---------|
| src/main.cpp | Added checkWinCondition(), displayGameStatus() |
| src/player.cpp | Box-on-target handling in movePlayer() |
| src/map.cpp | Target tracking with isTarget(), getTargetCount() |
| src/map.h | Added target_positions vector, getTargetCount() |

---

## Next Stage Preview
Stage 4 will implement the restart system:
- Press `R` to restart
- Restore initial map state
- Reset player position
- Reset step count
