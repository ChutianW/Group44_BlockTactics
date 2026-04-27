# Block Tactics

## Team Members

| Name | Student ID |
|------|------------|
| Wang Chutian | 3036481779 |
| Long Enjun | 3036279102 |

---

## Game Description

**Block Tactics** is a Sokoban-style puzzle game played in the terminal. The player controls a character `@` and pushes boxes `$` onto target positions `^`. When all boxes are on all targets, the level is complete.

**Objective:** Push all boxes onto all targets in as few steps as possible. Use the undo feature strategically to recover from mistakes.

---

## Features Implemented

### Core Gameplay
- **Player Movement**: W/A/S/D controls for 4-directional movement
- **Box Pushing**: Push boxes onto targets; boxes on targets display as `*`
- **Wall & Obstacle Collision**: Player cannot walk through walls `#` or obstacles `%`
- **Win Detection**: Level completes when all boxes are on all targets

### Three Difficulty Levels (Polymorphism)
- **Easy**: 3 boxes, 0 obstacles, 5 undos allowed
- **Medium**: 5 boxes, 3-5 obstacles, 3 undos allowed
- **Hard**: 7 boxes, 5-7 obstacles, 0 undos (disabled)
- Each difficulty implemented as a subclass of `Game` class using polymorphism

### Undo System
- Undo reverts player position and box positions
- Undo does NOT reduce step count
- Undo limit varies by difficulty (Easy=5, Medium=3, Hard=0)
- Status bar displays "U: X/Y" showing remaining/total undos

### Restart Feature
- Press R to restart current level
- Map layout, player position, and step count reset
- Undo count restores to maximum

### User Progress System
- New users can create an account (username stored in `data/user_data.txt`)
- Existing users can login and load their progress
- Best steps recorded per difficulty
- Total undos tracked per difficulty

### Leaderboard
- Accessible via [L] from main menu
- Sorted by: Difficulty (desc) > (Steps + Undos) (asc) > Time (asc)
- Top 3 ranks displayed with gold/silver/orange colors
- Shows username, difficulty, best steps, total undos, record date

### Random Map Generation
- Every new game generates a unique puzzle
- Boxes and targets placed with mobility validation (min 2 adjacent free cells)
- Obstacles placed avoiding path blocking
- Fallback to fixed test map if generation fails 50 times

### Help System
- Press [H] during gameplay to show help screen
- Displays all controls and game symbols

### Color Toggle
- Press [5] in main menu to enable/disable ANSI colors
- Colors: Player (Yellow), Box (Red), Target (Bright Green), Wall (Gray), Obstacle (Dark Gray)

---

## Coding Elements and Feature Support

### 1. OOP Encapsulation
**Files:** `game.h`, `player.h`, `map.h`, `file_io.h`

The `Game` class encapsulates all game state:
- Private members: `grid_`, `player_`, `undo_`, `user_` are not directly accessible
- Public interface: `run()`, `startGame()`, `restart()`, `setUserData()`
- Internal state protection ensures consistent game behavior

**Example in `game.h`:**
```cpp
private:
    std::vector<std::vector<char>> grid_;
    std::vector<std::pair<int, int>> targets_;
    Player player_;
    UndoSystem undo_;
```

### 2. OOP Polymorphism
**Files:** `game.h`, `game.cpp`

Three difficulty subclasses override base `Game` methods:
- `EasyGame`, `MediumGame`, `HardGame` inherit from `Game`
- `getUndoLimit()`, `getBoxCount()`, `getObstacleRange()` are virtual functions
- Each subclass provides different behavior for the same interface

**Example in `game.h`:**
```cpp
class Game {
    virtual int getUndoLimit() const;
    virtual int getBoxCount() const;
    virtual void getObstacleRange(int&, int&) const;
};

class EasyGame : public Game {
    int getUndoLimit() const override;  // returns 5
};
class MediumGame : public Game {
    int getUndoLimit() const override;  // returns 3
};
class HardGame : public Game {
    int getUndoLimit() const override;  // returns 0
};
```

### 3. Composition
**Files:** `game.h`, `player.h`, `renderer.h`

The `Game` class uses composition to hold multiple objects:
- `Renderer *renderer_` - handles all display logic
- `Player player_` - manages player position and steps
- `UndoSystem undo_` - manages undo history stack

**Example in `game.h`:**
```cpp
class Game {
    Renderer *renderer_;
    Player player_;
    UndoSystem undo_;
};
```

The `Renderer` class further demonstrates composition by managing display state.

### 4. File Input/Output
**Files:** `file_io.h`, `file_io.cpp`

User progress and leaderboard data persisted to text files:
- `saveUserData()` - writes user data to `data/user_data.txt`
- `loadUserData()` - reads existing user data
- `userExists()` - checks if username already registered
- `getLeaderboard()` - sorts and returns top players

**Data format in `data/user_data.txt`:**
```
username highest_level best_steps_easy best_steps_medium best_steps_hard total_undos_easy total_undos_medium total_undos_hard created_at
```

### 5. Dynamic Memory Management
**Files:** `player.h`, `player.cpp`

The `UndoSystem` class manages a stack of heap-allocated `UndoState` objects:
- `saveState()` allocates new `UndoState` on the heap
- `undo()` pops and deletes from stack
- `clear()` deletes all remaining states
- Destructor ensures no memory leaks

**Example in `player.h`:**
```cpp
class UndoSystem {
    std::stack<UndoState *> history;  // heap-allocated objects
public:
    ~UndoSystem();  // destructor cleans up
    void saveState(const std::vector<std::vector<char>>&, const Player&);
    bool undo(std::vector<std::vector<char>>&, Player&);
};
```

---

## Non-Standard Libraries Used

**None.** This project uses only standard C++17 libraries:

| Library | Purpose |
|---------|---------|
| `<vector>` | Grid and dynamic array storage |
| `<string>` | Color codes and text handling |
| `<fstream>` | File input/output |
| `<sstream>` | String parsing |
| `<ctime>` | Timestamp for leaderboard |
| `<algorithm>` | Sorting for leaderboard |
| `<stack>` | Undo history management |
| `<cstdlib>` | Random number generation for map generation |

All functionality is implemented using the C++ standard library only.

---

## Compilation and Execution Instructions

### macOS / Linux

```bash
cd /path/to/Group44_BlockTactics/Game\ Code
make
./blocktactics
```

### Windows (Command Prompt)

```cmd
cd C:\path\to\Group44_BlockTactics\Game Code
g++ -std=c++17 -Wall -Wextra -o blocktactics.exe main.cpp map.cpp player.cpp file_io.cpp renderer.cpp game.cpp
blocktactics.exe
```

### Requirements
- C++ compiler with C++17 support (g++ or clang++)
- Terminal that supports ANSI color codes (optional)
- Windows: MinGW-w64 recommended

---

## Project Structure

```
Group44_BlockTactics/
├── Game Code/
│   ├── main.cpp         # Entry point, menu handling
│   ├── game.h/cpp       # Game class and difficulty subclasses
│   ├── player.h/cpp     # Player struct and UndoSystem
│   ├── map.h/cpp        # Map generation and utilities
│   ├── renderer.h/cpp   # Display rendering and effects
│   ├── file_io.h/cpp    # User data and leaderboard I/O
│   ├── Makefile         # Build automation
│   └── data/
│       └── user_data.txt  # User progress storage
├── Journal.md           # Development work log
├── PlayingGuideline.md  # Player instructions
├── FeatureChecklist.md  # QA testing checklist
└── README.md            # This file
```