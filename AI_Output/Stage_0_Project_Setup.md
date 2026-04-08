# Stage 0 — Project Setup Output

## Date
2026-04-06

## Type
Project Setup / Documentation

---

## Goal
Establish the project structure and coding agreements before development begins.

---

## Tasks Completed

### 1. File Structure Created

```
Group44_BlockTactics-main/
├── src/
│   ├── main.cpp
│   ├── map.h
│   ├── map.cpp
│   ├── player.h
│   ├── player.cpp
│   ├── file_io.h
│   └── file_io.cpp
├── data/
│   └── user_data.txt
├── Makefile
└── README.md
```

### 2. Symbol System Confirmed

| Symbol | Description | Color |
|--------|-------------|-------|
| `@` | Player | Yellow |
| `$` | Box | Red |
| `^` | Target | Green |
| `#` | Wall | Gray |
| `.` | Obstacle | Dark Gray |
| ` ` | Empty | Default |

### 3. Module Ownership

**Person A:**
- main.cpp (game loop, input handling)
- player.cpp / player.h (movement, box pushing)

**Person B:**
- map.cpp / map.h (map structure, rendering)
- file_io.cpp / file_io.h (save/load system)

### 4. Coding Style Agreement

- Use `camelCase` for functions
- Use `snake_case` for variables
- Use `PascalCase` for structs/classes
- Consistent indentation (4 spaces)
- Comments for complex logic

### 5. Data Structures Agreed

```cpp
// Map representation
vector<vector<char>> game_map;
vector<vector<char>> initial_map;  // For restart

// Player structure
struct Player {
    int x;
    int y;
};

// Game state for undo
struct GameState {
    vector<vector<char>> map;
    Player player;
    int step_count;
};
```

---

## Output
- Empty project structure ready
- Everyone knows responsibilities
- Symbol system confirmed
- Coding style agreed

---

## Verification
- All team members understand the structure
- File naming conventions established
- Ready to proceed to Stage 1
