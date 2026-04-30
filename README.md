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

## How to play🤔

# Demo Video
[![Watch the video](https://img.youtube.com/vi/8Kya-oxEXeA/maxresdefault.jpg)](https://youtu.be/8Kya-oxEXeA)

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

### 1. Generation of Random Events
**Files:** `map.cpp`, `main.cpp`

Random events drive map generation and game replayability:
- `srand(time(nullptr))` seeds the random number generator
- `rand() %` used to randomly place obstacles, boxes, targets, and player
- Each new game generates a unique puzzle layout
- Fallback to fixed test map if random generation fails 50 times

**Example in `map.cpp`:**
```cpp
srand(static_cast<unsigned int>(time(nullptr)));

int box_x = 2 + rand() % (MAP_COLS - 4);
int box_y = 2 + rand() % (MAP_ROWS - 4);

int num_obstacles = settings.min_obstacles +
                    rand() % (settings.max_obstacles - settings.min_obstacles + 1);
```

### 2. Data Structures for Storing Data
**Files:** `game.h`, `player.h`, `map.h`, `file_io.h`

Multiple data structures store game state:
- `std::vector<std::vector<char>>` - 2D grid map representation
- `std::vector<std::pair<int, int>>` - target positions list
- `std::stack<UndoState *>` - undo history stack
- `std::vector<LeaderboardEntry>` - sorted leaderboard

**Example in `map.h`:**
```cpp
struct GameState {
    std::vector<std::vector<char>> grid;
    std::vector<std::pair<int, int>> targets;
    int player_row;
    int player_col;
    int step_count;
};
```

### 3. Dynamic Memory Management
**Files:** `player.h`, `player.cpp`

The `UndoSystem` class manages heap-allocated `UndoState` objects:
- `saveState()` allocates new `UndoState` on the heap with `new`
- `undo()` pops and deletes from stack with `delete`
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

### 5. Program Codes in Multiple Files
**Files:** `main.cpp`, `game.h/cpp`, `player.h/cpp`, `map.h/cpp`, `renderer.h/cpp`, `file_io.h/cpp`, `terminal.h/cpp`

The project is split into 8 source files, each with a specific responsibility:
- `main.cpp` - entry point and menu handling
- `game.h/cpp` - core game logic and difficulty classes
- `player.h/cpp` - player movement and undo system
- `map.h/cpp` - map generation and validation
- `renderer.h/cpp` - display rendering
- `file_io.h/cpp` - file persistence
- `terminal.h/cpp` - terminal capability detection

### 6. Multiple Difficulty Levels
**Files:** `game.h`, `game.cpp`

Three difficulty subclasses implement polymorphism:
- `EasyGame` - 3 boxes, 0 obstacles, 5 undos
- `MediumGame` - 5 boxes, 3-5 obstacles, 3 undos
- `HardGame` - 7 boxes, 6-10 obstacles, 0 undos

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
