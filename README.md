# 🧩 Block Tactics — Final Design Overview

## 📌 Course Project: COMP2113 / ENGG1340
**Type:** Text-based terminal game (C++)

---

## 1. 🧩 Introduction

Block Tactics is a text-based Sokoban-style puzzle game implemented in C++.  
The player pushes boxes onto target positions within a 10×10 grid.

This project satisfies all required coding elements:
- Random events
- Data structures
- Dynamic memory management
- File I/O
- Multi-file structure
- Multiple difficulty levels

---

## 2. 🎮 Game Overview

The player controls a character to push all boxes onto their targets.

The game emphasizes:
- Logical thinking
- Planning ahead
- Avoiding deadlocks

---

## 3. 🧱 Game Elements

| Symbol | Description | Color |
|--------|------------|-------|
| @ | Player | Yellow |
| $ | Box | Red |
| ^ | Target | Green |
| # | Wall | Gray |
| . | Obstacle | Dark Gray |
| (space) | Empty | Default |

---

## 🎯 Target Interaction Logic

- A box placed on a target is considered **completed**
- This is tracked by **game state logic**, not only visual color

### Implementation Concept

- Maintain logical tracking:
  - Either via a separate target grid
  - Or a special state (e.g., box-on-target flag)

- Win condition:
  - All targets are occupied by boxes

---

## 🎨 Visual Design

The game uses ANSI color codes for clarity.

### Color Mapping

- Player → Yellow  
- Box → Red  
- Target → Green  
- Box on Target → Bright Green  
- Wall → Gray  
- Obstacle → Dark Gray  

### Dynamic Behavior

- Box color changes from Red → Green when on target
- Provides immediate feedback to player

---

## 4. 📋 Game Rules

- Use W/A/S/D to move
- Push one box at a time
- Cannot push multiple boxes
- Cannot move through walls or obstacles
- Cannot pull boxes

---

## ⚠️ Deadlocks

- Box stuck in a corner (not on target)
- Blocking paths
- Losing access to targets

---

## 5. 🎲 Difficulty & Random Generation

### Difficulty Levels

| Level | Boxes | Obstacles |
|------|------|----------|
| Easy | 3 | 0 |
| Medium | 5 | 3–5 |
| Hard | 7 | 6–10 |

---

## Random Generation Constraints (Enhanced)

### 🔒 Hard Constraints (must satisfy)

- ❌ No boxes or targets in corners
- ❌ Targets cannot be adjacent to outer walls
- ❌ Obstacles cannot block essential paths
- ❌ Obstacles cannot overlap targets

### 🔄 Box Mobility Validation

- Each box must have at least **two adjacent free cells**
- Prevents immediate deadlock

### 🚧 Obstacle Placement Rules

- Obstacles must not:
  - Block narrow corridors completely
  - Isolate targets
  - Prevent player navigation

---

## 6. 🔄 Game Flow

Start  
→ Enter Username  
→ Load / Create User  
→ Select Difficulty  
→ Generate Map  
→ Save Initial State  

Game Loop:
- Display map  
- Input movement  
- Update state  
- Check win  
- Restart option  

---

## 7. 🔁 Restart System (Player-Level)

- Player can press `R` to restart the current level
- Useful when the puzzle becomes unsolvable

### Behavior

- Restores original map layout
- Resets:
  - Player position
  - Box positions
  - Step count
- Does NOT generate a new map

---

## 8. 🧠 System Architecture

### File Structure

- main.cpp  
- map.cpp / map.h  
- player.cpp / player.h  
- file_io.cpp / file_io.h  
- user_data.txt  

---

### Data Structures

```cpp
vector<vector<char>> map;

struct Player {
    int x, y;
};
```

---

## 9. 🛠️ Implementation Plan

- Map rendering
- Player movement
- Box pushing logic
- Win condition
- Restart system
- Random generation
- Difficulty system
- File I/O
- Dynamic memory
- Multi-file structure

---

## 10. 🧾 Requirement Mapping

| Requirement | Implementation |
|-------------|----------------|
| Random Events | Map generation |
| Data Structures | 2D vector |
| Dynamic Memory | Undo / allocation |
| File I/O | Save/load system |
| Multiple Files | Modular design |
| Difficulty | Multi-level |

---

## 11. 💾 User System (Enhanced)

### User Types

**New User**
- Starts from Easy level
- New record created

**Existing User**
- Load saved progress
- Resume from highest level

### Progress Tracking

- Highest level reached
- Best step count per level

### Save Conditions

Data is saved when:
- Player exits the game
- Player completes a level

---

## 12. 🧠 Optional Advanced Feature (Dynamic Memory)

### Undo System (Optional)

- Stores previous states using stack
- Allows player to revert moves
- Demonstrates dynamic memory usage

---

## 13. 👥 Collaboration

- Divide modules among team members
- Use GitHub for version control
- Maintain meaningful commit history

---

## 14. 📦 Submission Requirements

- GitHub repository
- README.md
- Source code + makefile
- Demo video (≤ 3 min)
- Contribution report

---

## 15. 🎥 Demo Content

- Gameplay
- Restart feature
- Win condition
- Color system
- Save/load system

---

## 16. ⚠️ Notes

- No external libraries required
- Must compile on CS server
- All work must be original
