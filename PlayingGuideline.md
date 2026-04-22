# Block Tactics - Playing Guideline

## Step-by-Step Instructions to Play the Game

---

## Step 1: Open Terminal

- **macOS:** Open `Terminal` from Applications > Utilities
- **Linux:** Press `Ctrl+Alt+T` or search "Terminal" in your launcher

---

## Step 2: Navigate to Project Folder

```bash
cd /path/to/Group44_BlockTactics
```

Or if the project is in your home directory:

```bash
cd ~/Group44_BlockTactics
```

Press **Enter**.

---

## Step 3: Compile the Game

```bash
cd "Game Code"
make
```

Press **Enter**.

**Expected:** No output means success. If you see errors, make sure `g++` is installed.

---

## Step 4: Run the Game

```bash
make run
```

Or directly:

```bash
./blocktactics
```

---

## Step 5: Welcome Screen

**You will see:**
```
  ╔══════════════════════════════════════════╗
  ║         BLOCK TACTICS                    ║
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

  [1] New Game
  [2] Continue (from saved progress)
  [3] View Controls
  [4] View Progress
  [5] Toggle Colors
  [L] Leaderboard
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
| `%` | Obstacle (cannot pass) | Dark Gray |

---

## How to Win

1. **Goal:** Push all boxes (`$`) onto all targets (`^`)
2. When a box is on a target, it shows as `*`
3. When ALL targets have boxes, you win!

---

## Difficulty Levels

| Level | Boxes | Obstacles | Undo Limit |
|-------|-------|-----------|------------|
| Easy | 3 | 0 | Unlimited (50 moves) |
| Medium | 5 | 3-5 | 5 moves |
| Hard | 7 | 6-10 | Disabled |

---

## Leaderboard

Press **[L]** from the Main Menu to view the leaderboard.

**Ranking Rules:**
- Priority: Difficulty > (Steps + Undos)
- Higher difficulty ranks first
- Among same difficulty, fewer steps + undos ranks higher
- If still tied, earlier record ranks first

**Rank Colors:**
| Rank | Color |
|------|-------|
| 1st | Gold |
| 2nd | Silver |
| 3rd | Orange |
| 4th+ | White |

**Displayed Info:** Username, Difficulty, Best Steps, Total Undos, Record Date

---

## Tips

- **Plan ahead** - Think before you push
- **Avoid corners** - Boxes stuck in corners cannot be moved
- **Use Undo (U)** - Made a mistake? Press U to go back
- **Restart (R)** - Completely stuck? Press R to try again

---

## Quick Start Commands (Copy All)

```bash
cd /path/to/Group44_BlockTactics/Game\ Code
make
./blocktactics
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| `g++ not found` | Install g++: `sudo apt install g++` (Linux) or check Xcode (macOS) |
| `cd` fails | Make sure path is correct with `ls` |
| Compilation errors | Check all source files exist with `ls` |
| Colors not showing | Use a modern terminal (iTerm2 on macOS, GNOME Terminal on Linux) |
| Game seems stuck | Press `Q` to quit and restart |

---

## Game Features

- **Random Maps** - Every game generates a new puzzle
- **Progress Saving** - Your best scores are saved automatically
- **Undo System** - Made a mistake? Go back up to 50 moves (Easy mode)
- **Multiple Difficulties** - Easy, Medium, and Hard modes

---

Enjoy playing Block Tactics!