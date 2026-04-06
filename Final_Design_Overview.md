# 🧩 Block Tactics — Final Design Overview

## 📌 Course Project: COMP2113 / ENGG1340  
**Type:** Text-based terminal game (C++)

---

## 🧩 1. Introduction

Block Tactics is a text-based Sokoban-style puzzle game implemented in C++.  
The player pushes boxes onto target positions within a 10×10 grid.

This project fulfills all required coding elements:
- Random events  
- Data structures  
- Dynamic memory management  
- File I/O  
- Multi-file structure  
- Multiple difficulty levels  

---

## 🎮 2. Game Overview

The player controls a character to push all boxes onto their targets.

The game emphasizes:
- Logical thinking  
- Planning ahead  
- Avoiding deadlocks  

---

## 🧱 3. Game Elements

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
- This is handled by **game state logic**, not just visual color

### Implementation Concept

- Track targets separately or maintain a box-on-target state
- Win condition:
  - All targets are occupied by boxes

---

## 🎨 4. Visual Design & Color System

The game uses ANSI color codes to enhance clarity.

### Color Mapping

- Player → Yellow  
- Box → Red  
- Target → Green  
- Box on Target → Bright Green  
- Wall → Gray  
- Obstacle → Dark Gray  

### Dynamic Behavior

- When a box reaches a target:
  - Color changes from Red → Green  
  - Provides visual feedback to the player  

---

## 📋 5. Game Rules

- Use W / A / S / D to move  
- Push one box at a time  
- Cannot push multiple boxes  
- Cannot move through walls or obstacles  
- Cannot pull boxes  

---

## ⚠️ Deadlocks

- Box stuck in a corner (not on target)  
- Blocking important paths  
- Losing access to targets  

---

## 🎲 6. Difficulty & Random Generation

### Difficulty Levels

| Level | Boxes | Obstacles |
|------|------|----------|
| Easy | 3 | 0 |
| Medium | 5 | 3–5 |
| Hard | 7 | 6–10 |

---

### Random Generation Constraints

#### 🔒 Hard Constraints

- No boxes or targets placed in corners  
- Targets cannot be adjacent to outer walls  
- Obstacles cannot overlap targets  
- Obstacles must not block essential paths  

---

### 🔄 Box Mobility Validation

- Each box must have at least **two adjacent free cells**
- Prevents immediate deadlocks  

---

### 🚧 Obstacle Placement Rules

- Obstacles must not:
  - Block narrow corridors completely  
  - Isolate targets  
  - Prevent player navigation  

---

## 🔄 7. Game Flow

Start  
→ Enter Username  
→ Load / Create User  
→ Select Difficulty  
→ Generate Map  
→ Save Initial State  

### Game Loop:

- Display map  
- Input movement  
- Update state  
- Check win condition  
- Restart option  

---

## 🔁 8. Restart System

- Press `R` to restart current level  
- Restores the original map state  
- Resets:
  - Player position  
  - Box positions  
  - Step count  
- Does NOT generate a new map  

---

## 🧠 9. System Architecture

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

## 🛠️ 10. Implementation Plan

- Map rendering  
- Player movement  
- Box pushing logic  
- Win condition detection  
- Restart system  
- Random map generation  
- Difficulty system  
- File I/O system  
- Dynamic memory usage  
- Multi-file structure  

---

## 🧾 11. Requirement Mapping

| Requirement        | Implementation                     |
|------------------|----------------------------------|
| Random Events     | Random map generation            |
| Data Structures   | 2D vector grid                   |
| Dynamic Memory    | Undo stack / dynamic allocation  |
| File I/O          | Save/load system                 |
| Multiple Files    | Modular `.cpp / .h` structure    |
| Difficulty Levels | Easy / Medium / Hard             |

---

## 💾 12. User System

### 👤 User Types

#### New User
- Starts from Easy level  
- New record created  

#### Existing User
- Loads saved progress  
- Continues from highest level  

---

### 📊 Progress Tracking

- Highest level reached  
- Best step count per level  

---

### 💾 Save Conditions

Data is saved when:
- Player exits the game  
- Player completes a level  

---

## 🧠 13. Optional Advanced Feature

### 🔁 Undo System (Optional)

- Stores previous game states using a stack  
- Allows player to revert moves  
- Demonstrates dynamic memory usage  

---

## 👥 14. Collaboration & GitHub

- Divide work by modules  
- Use GitHub for version control  
- Maintain meaningful commit messages  

---

## 📦 15. Submission Requirements

- GitHub repository  
- README.md  
- Source code + makefile  
- Demo video (≤ 3 minutes)  
- Contribution report  

---

## 🎥 16. Demo Content

- Gameplay demonstration  
- Restart feature  
- Win condition  
- Color system  
- Save/load system  

---

## 📌 17. Project Constraints

- No external libraries requiring installation  
- Must compile on the CS department server  
- All work must be original (no plagiarism)  

---
