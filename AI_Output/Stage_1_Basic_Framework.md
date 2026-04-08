# Stage 1 — Basic Framework Output

## Date
2026-04-06

## Type
Feature Implementation

---

## Goal
Create the basic framework with game loop skeleton, map structure, and player definition.

---

## Tasks Completed

### Person A Tasks (main.cpp, player)

#### 1. Game Loop Skeleton in main.cpp
- Welcome message display
- Controls display
- Main game loop with input handling
- Win condition checking
- Game status display

#### 2. Player Struct Defined in player.h
```cpp
struct Player {
    int x;
    int y;
    int step_count;
    
    Player();
    Player(int start_x, int start_y);
    void resetSteps();
    void incrementSteps();
    int getSteps() const;
};
```

#### 3. Movement Function Prototypes
```cpp
bool movePlayer(Player& player, Map& game_map, Direction dir);
bool canMove(const Player& player, const Map& game_map, Direction dir);
bool canPushBox(const Map& game_map, int box_x, int box_y, Direction dir);
void getDirectionOffset(Direction dir, int& dx, int& dy);
```

---

### Person B Tasks (map system)

#### 1. Map Class in map.h / map.cpp
- 10x10 grid structure using `vector<vector<char>>`
- Map printing function
- Cell get/set functions
- Boundary validation
- Target position tracking

#### 2. Fixed Sample Map Created
```
##########
#        #
# @##    #
#   $    #
#    $  ^#
#  $##   #
#  ^     #
#       ^#
#        #
##########
```

#### 3. Symbol Constants Defined
```cpp
const char PLAYER = '@';
const char BOX = '$';
const char TARGET = '^';
const char WALL = '#';
const char OBSTACLE = '.';
const char EMPTY = ' ';
const char BOX_ON_TARGET = '*';
```

---

## Files Created

| File | Purpose |
|------|---------|
| src/main.cpp | Game loop, input handling |
| src/map.h | Map class declaration |
| src/map.cpp | Map class implementation |
| src/player.h | Player struct and movement declarations |
| src/player.cpp | Player and movement implementations |
| src/file_io.h | File I/O declarations (skeleton) |
| src/file_io.cpp | File I/O implementations (skeleton) |
| Makefile | Build configuration |
| data/user_data.txt | Empty user data file |

---

## Merge 1 Checkpoint

### Joint Tasks Completed
- ✅ main.cpp connected with map.cpp
- ✅ Map displays on screen
- ✅ Player placed on map
- ✅ Project structure ready for compilation

### Expected Behavior
- Program runs
- Map displays correctly
- No crash
- Basic input loop works

---

## Verification
- Code structure follows design overview
- All header guards in place
- Modular file organization
- Ready for Stage 2 core gameplay implementation
