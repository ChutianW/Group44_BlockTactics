# Block Tactics - Playing Guideline

## Bug Fixes Applied

### Bug 1: termios.h Not Found

**Error:**
```
player.cpp:3:10: fatal error: termios.h: No such file or directory
```

**Root Cause:**
- `<termios.h>` and `<unistd.h>` are **Linux/Unix-only headers**
- Windows uses `<conio.h>` with `_getch()` instead

**Fix Applied:**
Modified `player.cpp` lines 1-10 to use cross-platform preprocessor directives:
```cpp
#ifdef _WIN32
    #include <conio.h>  // Windows
#else
    #include <termios.h>  // Linux/Mac
    #include <unistd.h>
#endif
```

---

### Bug 2: getch() Name Conflict

**Error:**
```
conio.h:272:15: error: ambiguating new declaration of 'int getch()'
player.h:78:6: note: old declaration 'char getch()'
```

**Root Cause:**
- Windows `<conio.h>` already defines `getch()` function
- Our custom `getch()` function conflicts with it

**Fix Applied:**
- Renamed our function from `getch()` to `getKeyInput()` in:
  - `player.h` (line 78)
  - `player.cpp` (line 319)
  - `main.cpp` (all occurrences)

---

## Step-by-Step Instructions to Play

### Step 1: Open PowerShell

Press `Win + X` and select **Windows PowerShell**

---

### Step 2: Navigate to Project Folder

Type this command (copy and paste):
```powershell
cd "d:\港大教材\y2\comp 2113\Group44_BlockTactics_VS code\Group44_BlockTactics\4\6 Skelenton(Cc)"
```

Press **Enter**.

---

### Step 3: Compile the Game

Type this command (copy and paste):
```powershell
g++ -std=c++17 -Wall -Wextra -o blocktactics main.cpp map.cpp player.cpp file_io.cpp
```

Press **Enter**.

**Expected:** No output means success.

---

### Step 4: Run the Game

Type this command:
```powershell
.\blocktactics.exe
```

Press **Enter**.

---

## Quick Copy-Paste (All Commands)

```powershell
cd "d:\港大教材\y2\comp 2113\Group44_BlockTactics_VS code\Group44_BlockTactics\4\6 Skelenton(Cc)"
g++ -std=c++17 -Wall -Wextra -o blocktactics main.cpp map.cpp player.cpp file_io.cpp
.\blocktactics.exe
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

| Symbol | Meaning |
|--------|---------|
| `@` | Player (you) |
| `$` | Box (push these) |
| `^` | Target (push boxes here) |
| `*` | Box on Target (success!) |
| `#` | Wall |
| `.` | Obstacle |

---

## Goal

Push all boxes (`$`) onto all targets (`^`). When complete, you win!

---

## Troubleshooting

| Error | Solution |
|-------|----------|
| `termios.h: No such file` | Apply the cross-platform fix (already done) |
| `g++ not found` | Install MinGW and add to PATH |
| `cd` fails | Make sure path is in quotes |
| Colors not showing | Use Windows Terminal instead of CMD |
