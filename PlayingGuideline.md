# Block Tactics - Playing Guideline

## Step-by-Step Instructions to Play the Game

---

## Step 1: Open PowerShell

- Press `Win + X` and select **Windows PowerShell**
- Or search "PowerShell" in the Start menu

---

## Step 2: Navigate to Project Folder

Type this command (copy and paste):
```powershell
cd "d:\港大教材\y2\comp 2113\Group44_BlockTactics-main"
```

Press **Enter**.

**Expected:** The prompt changes to show the folder path.

---

## Step 3: Compile the Game

Type this command (copy and paste):
```powershell
g++ -std=c++11 -Wall -o block_tactics "src/main.cpp" "src/map.cpp" "src/player.cpp" "src/file_io.cpp" "src/game.cpp"
```

Press **Enter**.

**Expected:** No output means success. If you see errors, check that g++ is installed.

---

## Step 4: Run the Game

Type this command:
```powershell
.\block_tactics.exe
```

Press **Enter**.

---

## Step 5: Welcome Screen

**You will see:**
```
  ╔══════════════════════════════════════════╗
  ║         🧩 BLOCK TACTICS 🧩              ║
  ║      A Sokoban-Style Puzzle Game         ║
  ╚══════════════════════════════════════════╝

  Press any key to continue...
```

**Action:** Press any key to continue.

---

## Step 6: Enter Username

**You will see:**
```
  ═══════════════ LOGIN ═══════════════

  Enter your username:
```

**Action:** Type your name (e.g., `player1`) and press **Enter**.

---

## Step 7: Main Menu

**You will see:**
```
  ═══════════════ MAIN MENU ═══════════════

  Welcome back, player1!

  [1] New Game
  [2] Continue (from saved progress)
  [3] View Controls
  [4] View Progress
  [5] Toggle Colors
  [Q] Quit

  Enter your choice:
```

**Action:** Press `1` for New Game.

---

## Step 8: Select Difficulty

**You will see:**
```
  ═══════════ SELECT DIFFICULTY ═══════════

  [1] Easy   - 3 boxes, no obstacles
  [2] Medium - 5 boxes, 3-5 obstacles
  [3] Hard   - 7 boxes, 6-10 obstacles
  [B] Back to menu

  Enter your choice:
```

**Action:** Press `1` for Easy (recommended for first time).

---

## Step 9: Play the Game!

**You will see the game board:**
```
  ╔════════════════════════════════════════╗
  ║  Difficulty: Easy │ Steps: 0 │ 0/3    ║
  ╚════════════════════════════════════════╝

  ##########
  #        #
  #  @     #
  #    $   #
  #      ^ #
  #  $     #
  #  ^  $  #
  #       ^#
  #        #
  ##########

  [W/A/S/D] Move | [R] Restart | [U] Undo | [H] Help | [Q] Quit
```

---

## Game Controls

| Key | Action |
|-----|--------|
| **W** | Move Up |
| **A** | Move Left |
| **S** | Move Down |
| **D** | Move Right |
| **R** | Restart Level |
| **U** | Undo Last Move |
| **H** | Show Help |
| **Q** | Quit to Menu |

---

## Game Symbols

| Symbol | Meaning | Color |
|--------|---------|-------|
| `@` | You (the player) | Yellow |
| `$` | Box (push these) | Red |
| `^` | Target (push boxes here) | Green |
| `*` | Box on Target (success!) | Bright Green |
| `#` | Wall (cannot pass) | Gray |
| `.` | Obstacle (cannot pass) | Gray |

---

## How to Win

1. **Goal:** Push all boxes (`$`) onto all targets (`^`)
2. When a box is on a target, it shows as `*`
3. When ALL targets have boxes, you win! 🎉

---

## Tips

- **Plan ahead** - Think before you push
- **Avoid corners** - Boxes stuck in corners cannot be moved
- **Use Undo (U)** - Made a mistake? Press U to go back
- **Restart (R)** - Completely stuck? Press R to try again

---

## Quick Start Commands (Copy All)

```powershell
cd "d:\港大教材\y2\comp 2113\Group44_BlockTactics-main"
g++ -std=c++11 -Wall -o block_tactics "src/main.cpp" "src/map.cpp" "src/player.cpp" "src/file_io.cpp" "src/game.cpp"
.\block_tactics.exe
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| `g++ not found` | Install MinGW and add to PATH |
| `cd` fails | Make sure path is in quotes |
| Compilation errors | Check all source files exist |
| Colors not showing | Try a different terminal (Windows Terminal recommended) |

---

## Have Fun! 🎮
