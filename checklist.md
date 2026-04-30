# Project Requirements Checklist

Evaluation of the `main` branch of `Group44_BlockTactics` against the COMP2113 Group Project requirements.

**Branch evaluated:** `main` (commit `357a86b`)
**Date:** Apr 30, 2026

---

## 1. Code Requirements

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 1.1 | Generation of random events | ✅ Pass | `map.cpp` uses `rand()` / `srand()` to generate a unique map every game (random box, target, and obstacle placement). `main.cpp` seeds the RNG with `time(nullptr)`. |
| 1.2 | Data structures for storing data | ✅ Pass | `std::vector<std::vector<char>>` for the grid; `std::vector<std::pair<int,int>>` for targets; `std::deque<UndoState*>` for undo history; `std::vector<LeaderboardEntry>` for rankings. |
| 1.3 | Dynamic memory management | ✅ Pass | `UndoSystem` in `player.cpp` uses `new UndoState(...)` and `delete` for each undo state; destructor and `clear()` ensure no leaks. `Renderer*` in `main.cpp` is also heap-allocated and deleted on exit. |
| 1.4 | File input/output | ✅ Pass | `file_io.cpp` implements `saveUserData()`, `loadUserData()`, `userExists()`, `getLeaderboard()` using `<fstream>` and `<sstream>` against `data/user_data.txt`. |
| 1.5 | Program codes in multiple files | ✅ Pass | 7 source files + 6 headers: `main.cpp`, `game.cpp/h`, `map.cpp/h`, `player.cpp/h`, `renderer.cpp/h`, `file_io.cpp/h`, `terminal.cpp/h`. |
| 1.6 | Multiple difficulty levels | ✅ Pass | Three difficulties — Easy / Medium / Hard — implemented as polymorphic subclasses (`EasyGame`, `MediumGame`, `HardGame`) of base `Game`. Each varies box count, obstacle range, and undo limit. |

**Section verdict:** ✅ All 6 code requirements satisfied.

---

## 2. Documentation and Code Style

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 2.1 | In-code documentation (what / inputs / outputs per function) | ⚠️ Partial | Most functions in `game.cpp`, `map.cpp`, `player.cpp`, `renderer.cpp` have brief one-line comments (e.g. `// Run game main loop`, `// Handle undo`). However, formal "what / inputs / outputs" docstrings are not consistently applied to every function. |
| 2.2 | Proper indentation and naming styles | ✅ Pass | Consistent 4-space indentation; `snake_case` for variables/members (e.g. `box_row`, `undos_left`, `level_complete_`), `PascalCase` for classes (`Game`, `UndoSystem`, `Renderer`), `camelCase` for methods (`getUndoLimit`, `saveState`). Trailing-underscore convention for private members. |
| 2.3 | All project files committed to private GitHub repo with sensible commit messages | ✅ Pass | Repo: `https://github.com/ChutianW/Group44_BlockTactics.git`. Recent commit messages are descriptive (e.g. `Fix undo on Hard after nextLevel; dynamic status bar frame alignment; add Target label`, `Remove unused parameter 'unicode_ch' from Renderer::line() function`). No empty or trivial messages observed in recent history. |
| 2.4 | README.MD with team members, description, features, non-standard libraries, compilation/execution | ✅ Pass | `README.md` contains: Team Members table, Game Description, How to play (with demo video), Features Implemented (8 sub-sections), Coding Elements section, Non-Standard Libraries (states "None" + table of standard headers), Compilation and Execution Instructions for macOS/Linux and Windows, Project Structure tree. |

**Section verdict:** ⚠️ Mostly satisfied — recommend strengthening per-function docstrings (2.1) for full marks.

---

## 3. Contributions

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 3.1 | Group of 2 → each contributes ≥ 25% (per Contributors Graph) | ✅ Pass | `git shortlog -sne --all`: ChutianW = 42 commits, Enjun3636 (incl. alt `Enjun36`) = 46 commits. Both members are substantial contributors well above the 25% threshold. |

**Section verdict:** ✅ Satisfied. Recommend confirming via the GitHub Contributors Graph UI (Insights → Contributors) before submission.

---

## 4. Library Restrictions

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 4.1 | Only standard C/C++ libraries (no external dependencies) | ✅ Pass | `README.md` lists only standard headers: `<vector>`, `<string>`, `<fstream>`, `<sstream>`, `<ctime>`, `<algorithm>`, `<stack>`, `<cstdlib>`. Plus platform headers `<conio.h>` (Windows) / `<termios.h>` (POSIX) which ship with the OS toolchain. No third-party packages. |

**Section verdict:** ✅ Satisfied.

---

## 5. Compilation on CS Department Academy Server

| # | Requirement | Status | Evidence |
|---|-------------|--------|----------|
| 5.1 | Code compiles successfully with `g++ -std=c++17` | ✅ Pass (locally) | Clean rebuild with `g++ -std=c++17 -Wall -Wextra` produces `blocktactics.exe` with **zero warnings**. The `Makefile` is cross-platform (Windows / Linux / macOS). |
| 5.2 | Verified on the academy Linux server | ⏳ Not yet verified | Has not been tested on the actual CS dept academy server from this evaluation. Recommend SSH-ing in and running `make` before submission. |

**Section verdict:** ⚠️ Local compilation clean. **Action item:** test on the academy server.

---

## Overall Summary

| Category | Status |
|----------|--------|
| 1. Code Requirements (6 items) | ✅ All pass |
| 2. Documentation & Code Style (4 items) | ⚠️ 3 pass, 1 partial |
| 3. Contributions | ✅ Pass |
| 4. Library Restrictions | ✅ Pass |
| 5. Compilation | ⚠️ Local pass, server pending |

**Overall:** **Project largely satisfies the stated requirements.** Two follow-up items recommended before final submission:

1. **(2.1)** Add fuller per-function docstrings (what it does / inputs / outputs) to every function — currently many have only a brief one-line summary.
2. **(5.2)** Verify the build on the CS department academy Linux server via `make`.

---

## Notes on Items Outside the Requirement List (Bonus)

- **OOP Encapsulation** — private members in `Game`, `Player`, `UndoSystem`.
- **OOP Polymorphism** — virtual `getBoxCount()`, `getObstacleRange()` overridden in difficulty subclasses; `Renderer` / `RendererWithEffects` hierarchy.
- **Composition** — `Game` owns `Renderer*`, `Player`, `UndoSystem`.
- **Leaderboard, undo system, color toggle, restart, map regeneration `[G]`** are all implemented and exceed the baseline requirements.
