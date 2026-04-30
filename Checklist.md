# Project Requirements Checklist

Evaluation of the `main` branch of `Group44_BlockTactics` against both:
- **Image 1:** General project Requirements
- **Image 2:** Stage 2 Project Product Submission deliverables

**Branch evaluated:** `main` (commit `fcdbd78` — *Merge upstream/main to sync with latest changes*)
**Date:** Apr 30, 2026

---

# Part A — General Requirements (Image 1)

## 1. Code Requirements

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 1.1 | Generation of random events | ✅ Pass | `map.cpp` uses `srand(time(nullptr))` and `rand() %` to randomly place obstacles, boxes, targets, and the player. Each new game produces a unique puzzle. README §"Coding Elements 1" documents this. |
| 1.2 | Data structures for storing data | ✅ Pass | Uses `std::vector<std::vector<char>>` (grid), `std::vector<std::pair<int,int>>` (targets), `std::stack<UndoState*>` (undo history), `std::vector<LeaderboardEntry>` (leaderboard). README §"Coding Elements 2" documents this. |
| 1.3 | Dynamic memory management | ✅ Pass | `UndoSystem` in `player.cpp` allocates `UndoState` with `new` and frees with `delete`; destructor + `clear()` prevent leaks. `Renderer*` is also heap-allocated in `main.cpp`. README §"Coding Elements 3" documents this. |
| 1.4 | File input/output | ✅ Pass | `file_io.cpp` implements `saveUserData()`, `loadUserData()`, `userExists()`, `getLeaderboard()` against `data/user_data.txt` using `<fstream>` + `<sstream>`. README §"Coding Elements 4" documents this. |
| 1.5 | Program codes in multiple files | ✅ Pass | 7 `.cpp` + 7 `.h` files in `Game Code/`: `main`, `game`, `player`, `map`, `renderer`, `file_io`, `terminal`. README §"Coding Elements 5" documents this. |
| 1.6 | Multiple difficulty levels | ✅ Pass | Easy / Medium / Hard implemented as polymorphic subclasses (`EasyGame`, `MediumGame`, `HardGame`) of base `Game`. Each varies box count, obstacle range, and undo limit. README §"Coding Elements 6" documents this. |

**Section verdict:** ✅ All 6 code requirements satisfied.

---

## 2. Documentation and Code Style

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 2.1 | In-code documentation (what / inputs / outputs per function) | ⚠️ Partial | Most functions have brief one-line comments (e.g. `// Run game main loop`, `// Handle undo`). However the rubric asks specifically for **what it does / inputs / outputs** per function — formal docstrings are not consistently applied across every function. |
| 2.2 | Proper indentation and naming styles | ✅ Pass | Consistent 4-space indentation; `snake_case` for variables/members (`box_row`, `undos_left`, `level_complete_`), `PascalCase` for classes (`Game`, `UndoSystem`, `Renderer`), `camelCase` for methods (`getUndoLimit`, `saveState`). Trailing-underscore convention for private members. |
| 2.3 | All project files committed to private GitHub repo with sensible commit messages | ✅ Pass | Repo: `https://github.com/ChutianW/Group44_BlockTactics`. Recent commits are descriptive (e.g. `Fix undo on Hard after nextLevel; dynamic status bar frame alignment; add Target label`, `Remove unused parameter 'unicode_ch' from Renderer::line()`). No empty/trivial messages observed. |
| 2.4 | README.MD with team members, app description, features, non-standard libraries, compilation/execution | ✅ Pass | `README.md` covers: Team Members table, Game Description, How to play (with demo video link), Features Implemented (8 sub-sections), Coding Elements 1-6 mapping, Non-Standard Libraries ("None" + standard headers table), Compilation and Execution Instructions for macOS/Linux + Windows, Project Structure tree. |

**Section verdict:** ⚠️ 3 pass, 1 partial — recommend strengthening per-function docstrings for full marks on 2.1.

---

## 3. Contributions

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 3.1 | Group of 2 → each contributes ≥ 25% (per Contributors Graph) | ✅ Pass | `git shortlog -sne --all`: ChutianW = 44 commits; Enjun3636 + alt `Enjun36` = 47 commits. Both well above 25%. |

**Section verdict:** ✅ Satisfied. Recommend confirming with the GitHub Contributors Graph UI (Insights → Contributors) before final submission.

---

## 4. Library Restrictions

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 4.1 | Only standard C/C++ libraries (no external installation) | ✅ Pass | README lists only standard headers: `<vector>`, `<string>`, `<fstream>`, `<sstream>`, `<ctime>`, `<algorithm>`, `<stack>`, `<cstdlib>`. Plus OS-bundled `<conio.h>` (Windows) / `<termios.h>` (POSIX). No third-party packages. |

**Section verdict:** ✅ Satisfied.

---

## 5. Compilation on CS Department Academy Server

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 5.1 | Code compiles with `g++ -std=c++17` | ✅ Pass (locally) | Clean rebuild with `g++ -std=c++17 -Wall -Wextra` produces `blocktactics.exe` with **zero warnings** on Windows. `Makefile` is cross-platform (auto-detects OS for clean/run targets). |
| 5.2 | Verified on CS dept academy Linux server | ⏳ Not yet verified | Has not been tested on the actual academy server. Recommend SSH-ing in and running `make` before final submission. |

**Section verdict:** ⚠️ Local pass; **Action item:** test on academy server.

---

# Part B — Stage 2 Submission Deliverables (Image 2)

## 6. README.MD Required Contents

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 6.1 | Identification of team members | ✅ Pass | README §"Team Members" lists Wang Chutian (3036481779) and Long Enjun (3036279102). |
| 6.2 | Description of the game | ✅ Pass | README §"Game Description" explains Sokoban-style gameplay, symbols, and objective. |
| 6.3 | List of features implemented + explain how coding elements 1-5 support them | ✅ Pass | README §"Features Implemented" lists 8 feature groups; §"Coding Elements and Feature Support" explicitly maps elements 1-6 (random events, data structures, dynamic memory, file I/O, multiple files, difficulty levels) to source files and example code. |
| 6.4 | List of non-standard C/C++ libraries (if any) and what features they support | ✅ Pass | README §"Non-Standard Libraries Used" explicitly states **"None"** and provides a table of standard C++17 headers used. |
| 6.5 | Compilation and execution instructions ("Quick start") | ✅ Pass | README §"Compilation and Execution Instructions" provides macOS/Linux (`make` + `./blocktactics`) and Windows (`g++ ... -o blocktactics.exe`) commands. |

**Section verdict:** ✅ All README requirements satisfied.

---

## 7. Source Code & Build Files

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 7.1 | All source files (`.h` / `.cpp` / `.c`) included | ✅ Pass | `Game Code/` contains: `main.cpp`, `game.cpp/h`, `player.cpp/h`, `map.cpp/h`, `renderer.cpp/h`, `file_io.cpp/h`, `terminal.cpp/h` (7 cpp + 7 h). |
| 7.2 | Makefile included | ✅ Pass | `Game Code/Makefile` present (728 bytes). Cross-platform: auto-detects Windows vs POSIX for clean/run commands. |

**Section verdict:** ✅ Satisfied.

---

## 8. Submission & Video

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 8.1 | Submit Moodle link to GitHub repo | ⏳ Action required | Outside this evaluation — must be done by the team on Moodle. |
| 8.2 | Demo video ≤ 3 minutes | ⚠️ Verify | README links to `https://youtu.be/8Kya-oxEXeA` — please verify the video is ≤ 3 min and demonstrates implemented features. |

**Section verdict:** ⚠️ Manual verification needed for Moodle submission and video length.

---

# Overall Summary

| Category | Status |
|----------|--------|
| 1. Code Requirements (6 items) | ✅ All pass |
| 2. Documentation & Code Style (4 items) | ⚠️ 3 pass, 1 partial |
| 3. Contributions | ✅ Pass |
| 4. Library Restrictions | ✅ Pass |
| 5. Compilation | ⚠️ Local pass, server pending |
| 6. README contents (5 items) | ✅ All pass |
| 7. Source & Makefile | ✅ Pass |
| 8. Moodle + Video | ⏳ Manual verification |

**Overall verdict:** **Project largely satisfies the stated requirements.** Outstanding action items before final submission:

1. **(2.1)** Add fuller per-function docstrings — *what it does / inputs / outputs* — to each function. Most currently have only a one-line summary.
2. **(5.2)** Test compilation on the CS department academy Linux server (`make` from `Game Code/`).
3. **(8.1)** Submit the GitHub repo link to Moodle.
4. **(8.2)** Confirm the demo video at `https://youtu.be/8Kya-oxEXeA` is ≤ 3 minutes and showcases implemented features.

---

## Notes — Bonus Implementation Beyond Requirements

The project goes beyond baseline requirements with:
- **OOP Encapsulation** — private members in `Game`, `Player`, `UndoSystem`.
- **Composition** — `Game` owns `Renderer*`, `Player`, `UndoSystem`.
- **Polymorphism with overrides** — `getBoxCount()`, `getObstacleRange()` per difficulty; `Renderer` / `RendererWithEffects` hierarchy.
- **Leaderboard with ranking & color highlights** for top 3.
- **Color toggle**, **help screen**, **restart**, **map regeneration `[G]`**, **per-user progress tracking**.
