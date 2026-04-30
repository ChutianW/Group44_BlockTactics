# Project Requirements Checklist 2

Re-evaluation of `Group44_BlockTactics` after in-code documentation was added.

- **Branch evaluated:** `final_evaluation`
- **Latest commit:** `0875052` — *Update Checklist.md with in-code documentation summary*
- **Date:** May 1, 2026

> **Scope of this checklist:** Same requirements as `Checklist.md` (Image 1 general requirements + Image 2 Stage 2 deliverables), re-evaluated against the current `final_evaluation` branch state.  The only change since `Checklist.md` was created is the addition of formal `Function / Purpose / Inputs / Output` docstrings to **every function** in all 7 `.cpp` files and `terminal.h` (commit `045637e`).  No game logic was altered.

---

# Part A — General Requirements

## 1. Code Requirements

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 1.1 | Generation of random events | ✅ Pass | `map.cpp` uses `srand(time(nullptr))` in `main.cpp` and `rand() %` inside `generateRandomMap()`, `placeBoxesAndTargets()`, `placeObstacles()`, `placePlayer()` to produce a unique puzzle each session. Unchanged from previous evaluation. |
| 1.2 | Data structures for storing data | ✅ Pass | `std::vector<std::vector<char>>` (grid), `std::vector<std::pair<int,int>>` (targets), `std::deque<UndoState*>` (undo history in `UndoSystem`), `std::vector<UserData>` / `std::vector<LeaderboardEntry>` (file I/O). README §"Coding Elements 2" maps these. |
| 1.3 | Dynamic memory management | ✅ Pass | `UndoSystem::saveState()` allocates `UndoState` with `new`; `UndoSystem::undo()`, `clear()`, and `~UndoSystem()` free with `delete`. `Renderer*` heap-allocated in `main.cpp` via `new RendererWithEffects(...)` and deleted at exit. No memory leaks detected. |
| 1.4 | File input/output | ✅ Pass | `file_io.cpp` implements `saveUserData()`, `loadUserData()`, `userExists()`, `loadAllUsers()`, `saveGame()`, `loadGame()`, `getLeaderboard()` against `data/user_data.txt` using `<fstream>` and `<sstream>`. |
| 1.5 | Program code in multiple files | ✅ Pass | 7 `.cpp` + 7 `.h` source files in `Game Code/`: `main`, `game`, `player`, `map`, `renderer`, `file_io`, `terminal`. Cross-platform `Makefile` included. |
| 1.6 | Multiple difficulty levels | ✅ Pass | `EasyGame`, `MediumGame`, `HardGame` are polymorphic subclasses of `Game`. Each overrides `getBoxCount()`, `getObstacleRange()`, and sets a distinct undo limit (5 / 3 / 0) via `getUndoLimit()`. |

**Section verdict:** ✅ All 6 code requirements satisfied. No change from Checklist.md.

---

## 2. Documentation and Code Style

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 2.1 | In-code documentation (what / inputs / outputs per function) | ✅ **Pass** | **Updated since Checklist.md.** Every function in all source files now has a formal block comment in the format `Function / Purpose / Inputs / Output`. 142 functions documented across `terminal.cpp/h`, `player.cpp`, `file_io.cpp`, `map.cpp`, `game.cpp`, `renderer.cpp`, `main.cpp`. Build confirms 0 errors / 0 warnings. See commit `045637e`. |
| 2.2 | Proper indentation and naming styles | ✅ Pass | Consistent 4-space indentation throughout. `snake_case` for variables/parameters (`box_row`, `undos_left`, `level_complete_`), `PascalCase` for classes (`Game`, `UndoSystem`, `Renderer`), `camelCase` for methods (`getUndoLimit`, `saveState`, `printWinScreen`). Trailing-underscore for private members (`game_running_`, `renderer_`). |
| 2.3 | All project files committed to private GitHub repo with sensible commit messages | ✅ Pass | Repo: `https://github.com/ChutianW/Group44_BlockTactics`. All source files present on `final_evaluation`. Recent commits are descriptive: `Add function documentation for project requirements`, `Update Checklist.md with in-code documentation summary`, `Fix undo on Hard after nextLevel`, `Remove unused parameter 'unicode_ch' from Renderer::line()`. |
| 2.4 | README.MD with team members, app description, features, non-standard libraries, compilation/execution | ✅ Pass | `README.md` contains: Team Members table, Game Description, How to Play (with demo video link), Features Implemented (8 sub-sections), Coding Elements 1–6 mapping, Non-Standard Libraries ("None" + standard header table), Compilation & Execution instructions for macOS/Linux and Windows, Project Structure tree. |

**Section verdict:** ✅ All 4 documentation requirements now satisfied.
- **Previous:** ⚠️ 3 pass, 1 partial (2.1 was partial)
- **Current:** ✅ All 4 pass — formal docstrings added to every function in commit `045637e`.

---

## 3. Contributions

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 3.1 | Group of 2 → each contributes ≥ 25% (per Contributors Graph) | ✅ Pass | `git shortlog -sne --all`: ChutianW = 44+ commits; Enjun3636/Enjun36 = 47+ commits. Both contributors exceed the 25% minimum. |

**Section verdict:** ✅ Satisfied.

---

## 4. Library Restrictions

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 4.1 | Only standard C/C++ libraries (no external installation required) | ✅ Pass | All includes are standard C++17 headers (`<vector>`, `<string>`, `<fstream>`, `<sstream>`, `<ctime>`, `<algorithm>`, `<deque>`, `<cstdlib>`) plus OS-bundled `<conio.h>` (Windows) / `<termios.h>` (POSIX). No third-party packages. README §"Non-Standard Libraries Used" explicitly states **"None"**. |

**Section verdict:** ✅ Satisfied.

---

## 5. Compilation

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 5.1 | Code compiles with `g++ -std=c++17` | ✅ Pass | Clean rebuild with `g++ -std=c++17 -Wall -Wextra` produces `blocktactics.exe` with **0 errors, 0 warnings** after documentation was added (verified May 1, 2026). `Makefile` is cross-platform. |
| 5.2 | Verified on CS dept academy Linux server | ⏳ Not yet verified | Has not been tested on the HKUCS academy Linux server. **Action item:** SSH in and run `make` from `Game Code/` before final submission. |

**Section verdict:** ⚠️ Local build passes cleanly. Academy server test still pending.

---

# Part B — Stage 2 Submission Deliverables

## 6. README.MD Required Contents

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 6.1 | Identification of team members | ✅ Pass | README §"Team Members" lists Wang Chutian (3036481779) and Long Enjun (3036279102). |
| 6.2 | Description of the game | ✅ Pass | README §"Game Description" explains Sokoban-style gameplay, symbols, and win objective. |
| 6.3 | Features implemented + coding elements 1–5 explained | ✅ Pass | README §"Features Implemented" (8 groups) and §"Coding Elements and Feature Support" explicitly maps elements 1–6 to source files with example code. |
| 6.4 | Non-standard C/C++ libraries (if any) | ✅ Pass | README §"Non-Standard Libraries Used" explicitly states **"None"** with a table of standard C++17 headers used. |
| 6.5 | Compilation and execution instructions | ✅ Pass | README §"Compilation and Execution Instructions" gives macOS/Linux (`make` + `./blocktactics`) and Windows (`g++ ... -o blocktactics.exe`) commands. |

**Section verdict:** ✅ All README requirements satisfied.

---

## 7. Source Code & Build Files

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 7.1 | All source files (`.h` / `.cpp`) included in repo | ✅ Pass | `Game Code/` contains all 14 source files: `main.cpp`, `game.cpp/h`, `player.cpp/h`, `map.cpp/h`, `renderer.cpp/h`, `file_io.cpp/h`, `terminal.cpp/h`. |
| 7.2 | Makefile included | ✅ Pass | `Game Code/Makefile` present. Cross-platform (auto-detects Windows vs POSIX for `clean` and `run` targets). |

**Section verdict:** ✅ Satisfied.

---

## 8. Submission & Video

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 8.1 | Submit Moodle link to GitHub repo | ⏳ Action required | Must be submitted manually by the team on Moodle — outside scope of this evaluation. |
| 8.2 | Demo video ≤ 3 minutes | ⚠️ Verify | README links to `https://youtu.be/8Kya-oxEXeA`. Please confirm the video is ≤ 3 minutes and demonstrates the implemented features. |

**Section verdict:** ⚠️ Manual action required.

---

# Overall Summary

| Category | Previous (Checklist.md) | Current (Checklist2.md) | Change |
|----------|-------------------------|-------------------------|--------|
| 1. Code Requirements (6 items) | ✅ All pass | ✅ All pass | — |
| 2. Documentation & Code Style (4 items) | ⚠️ 3 pass, 1 partial | ✅ **All 4 pass** | ✅ Upgraded |
| 3. Contributions | ✅ Pass | ✅ Pass | — |
| 4. Library Restrictions | ✅ Pass | ✅ Pass | — |
| 5. Compilation | ⚠️ Local pass, server pending | ⚠️ Local pass, server pending | — |
| 6. README contents (5 items) | ✅ All pass | ✅ All pass | — |
| 7. Source & Makefile | ✅ Pass | ✅ Pass | — |
| 8. Moodle + Video | ⏳ Manual verification | ⏳ Manual verification | — |

**Overall verdict:** **Project satisfies all evaluable requirements.** The only previously failing item (2.1 — per-function docstrings) is now fully resolved.

### Remaining action items before final submission

1. **(5.2)** Test compilation on the CS department academy Linux server — run `make` from `Game Code/`.
2. **(8.1)** Submit the GitHub repo URL to Moodle.
3. **(8.2)** Confirm `https://youtu.be/8Kya-oxEXeA` is ≤ 3 minutes and covers all major features.

---

## What Changed Since Checklist.md

| Item | Before | After |
|------|--------|-------|
| Requirement 2.1 (in-code documentation) | ⚠️ Partial — most functions had only one-line section divider comments | ✅ Pass — every function now has a `Function / Purpose / Inputs / Output` block comment |
| Functions with formal docstrings | ~0 (section headers only) | **142** across all 7 `.cpp` files and `terminal.h` |
| Build result after documentation added | N/A | ✅ `g++ -std=c++17 -Wall -Wextra` → 0 errors, 0 warnings |
| Commit | — | `045637e` *Add function documentation for project requirements* |

All other source code, game logic, and features are **unchanged**.
