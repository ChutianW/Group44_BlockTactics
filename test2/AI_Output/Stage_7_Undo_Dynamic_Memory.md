# Stage 7 — Undo System (Dynamic Memory) Output

## Date
2026-04-08

## Type
Feature Implementation

---

## Goal
Implement undo functionality using dynamic memory management with a stack data structure.

---

## Tasks Completed

### Person A Tasks

#### 1. Undo Input Handling
```cpp
case 'u':
    if (undo_system.canUndo()) {
        undo_system.undo(game_map, player);
    }
    break;
```

#### 2. Connect Undo to Game Loop
- Undo system passed to `movePlayer()` function
- State saved before each move

---

### Person B Tasks

#### 1. GameState Structure
```cpp
struct GameState {
    vector<vector<char>> grid;
    int player_x;
    int player_y;
    int step_count;

    GameState(const vector<vector<char>>& g, int px, int py, int steps)
        : grid(g), player_x(px), player_y(py), step_count(steps) {}
};
```

#### 2. UndoSystem Class (Dynamic Memory)
```cpp
class UndoSystem {
private:
    stack<GameState*> history;  // Pointers for dynamic memory
    int max_history;

public:
    UndoSystem(int max_size = 100);
    ~UndoSystem();  // Destructor cleans up memory

    void saveState(const Map& game_map, const Player& player);
    bool undo(Map& game_map, Player& player);
    void clear();
    bool canUndo() const;
};
```

#### 3. Dynamic Memory Management

**Allocation (new):**
```cpp
void UndoSystem::saveState(const Map& game_map, const Player& player) {
    GameState* state = new GameState(
        game_map.getGrid(),
        player.x,
        player.y,
        player.getSteps()
    );
    history.push(state);
}
```

**Deallocation (delete):**
```cpp
bool UndoSystem::undo(Map& game_map, Player& player) {
    GameState* state = history.top();
    history.pop();
    
    // Restore state
    game_map.setGrid(state->grid);
    player.x = state->player_x;
    player.y = state->player_y;
    player.step_count = state->step_count;
    
    delete state;  // Free memory
    return true;
}
```

**Destructor (cleanup):**
```cpp
UndoSystem::~UndoSystem() {
    clear();
}

void UndoSystem::clear() {
    while (!history.empty()) {
        GameState* state = history.top();
        history.pop();
        delete state;
    }
}
```

---

## Merge 7 Checkpoint

### Verified Behavior
- ✅ Make several moves
- ✅ Press U to undo
- ✅ Map state returns correctly
- ✅ Player position returns correctly
- ✅ Box positions return correctly
- ✅ Step count returns correctly
- ✅ No memory leaks (proper delete)
- ✅ Undo history cleared on restart

---

## Dynamic Memory Demonstration

This implementation demonstrates:

1. **new operator** - Creating GameState objects dynamically
2. **delete operator** - Freeing memory when undoing or clearing
3. **Destructor** - Automatic cleanup when UndoSystem is destroyed
4. **Stack data structure** - LIFO for undo operations
5. **Pointer management** - Using `GameState*` in stack

---

## Memory Safety

| Operation | Memory Action |
|-----------|---------------|
| Save state | `new GameState(...)` |
| Undo | `delete state` |
| Clear history | `delete` all states |
| Destructor | Calls `clear()` |
| Restart level | Calls `clear()` |

---

## Files Modified

| File | Changes |
|------|---------|
| src/player.h | Added GameState struct, UndoSystem class |
| src/player.cpp | Implemented UndoSystem with new/delete |
| src/map.h | Added getGrid(), setGrid() methods |
| src/map.cpp | Implemented grid getter/setter |
| src/game.cpp | Integrated undo into game loop |

---

## Output
- Undo system complete with dynamic memory
- Demonstrates proper memory management
- No memory leaks
