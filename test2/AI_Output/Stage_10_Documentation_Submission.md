# Stage 10 — Documentation + Submission Output

## Date
2026-04-08

## Type
Documentation

---

## Goal
Finalize all documentation and prepare submission package.

---

## Tasks Completed

### Person A Tasks

#### 1. README.md Finalized
- Game description
- How to play instructions
- Controls table
- Symbol legend
- Difficulty levels
- Compilation instructions
- Project structure
- Requirement mapping

#### 2. Design Overview Updated
- All features documented
- Implementation details
- Architecture diagram

#### 3. Requirement Mapping

| Requirement | Implementation |
|-------------|----------------|
| Random Events | `generateRandomMap()` with constraints |
| Data Structures | `vector<vector<char>>`, `struct`, `stack`, `pair` |
| Dynamic Memory | `UndoSystem` with `new`/`delete` |
| File I/O | `loadUserData()`, `saveUserData()` |
| Multiple Files | 5 source files, 5 header files |

---

### Person B Tasks

#### 1. Function Comments
All major functions documented with:
- Purpose description
- Parameter explanations
- Return value descriptions

#### 2. Code Formatting
- Consistent indentation (4 spaces)
- Proper spacing
- Logical grouping

#### 3. File Headers
Each source file includes:
```cpp
/*
 * Block Tactics - [Module Name]
 * COMP2113 / ENGG1340 Course Project
 * Group 44
 */
```

---

## Final Submission Checklist

### Repository Contents
- [x] README.md
- [x] Source code (src/)
- [x] Makefile
- [x] Data directory (data/)
- [x] Documentation (AI_Output/)

### Code Quality
- [x] Compiles without errors
- [x] Compiles without warnings
- [x] Runs correctly
- [x] No memory leaks

### Documentation
- [x] Clear README
- [x] Code comments
- [x] Design overview

### Features
- [x] Random map generation
- [x] Multiple difficulty levels
- [x] Player movement
- [x] Box pushing
- [x] Win condition
- [x] Restart system
- [x] Undo system
- [x] Save/load system
- [x] Color rendering

---

## File Summary

### Source Files
| File | Lines | Purpose |
|------|-------|---------|
| main.cpp | 33 | Entry point |
| game.h | 55 | Game class declaration |
| game.cpp | ~450 | Game logic and UI |
| map.h | 128 | Map class declaration |
| map.cpp | 433 | Map implementation |
| player.h | 76 | Player and undo declarations |
| player.cpp | 227 | Movement and undo |
| file_io.h | 32 | File I/O declarations |
| file_io.cpp | 131 | Save/load implementation |

### Total: ~1,565 lines of code

---

## Compilation Command

```bash
make
```

Or:
```bash
g++ -std=c++11 -Wall -pedantic-errors -o block_tactics \
    src/main.cpp src/map.cpp src/player.cpp \
    src/file_io.cpp src/game.cpp
```

---

## Demo Script

1. **Start Game**
   - Run `./block_tactics`
   - Enter username

2. **Show Menu**
   - Display main menu options

3. **New Game (Easy)**
   - Select difficulty
   - Show generated map

4. **Gameplay**
   - Move player (W/A/S/D)
   - Push boxes
   - Show color changes

5. **Undo**
   - Make moves
   - Press U to undo

6. **Restart**
   - Press R
   - Show map reset

7. **Win**
   - Complete level
   - Show win screen

8. **Save/Load**
   - Quit and restart
   - Show progress loaded

---

## Output
- Project fully complete
- Ready for submission
