# Work Log

## Mar 21
- First team meeting
- Conducted brainstorming
- Confirmed the core content of the game

## Apr 3
- Started project construction
- Created GitHub repository
- Wrote the project agenda
- Finalized detailed game design and task assignment

## Apr 8
- Completed the program framework
- Finished the initial version of the program
- Carried out game trial and found problems:
  1. Defect in undo function: unlimited number of uses
  2. Sudden flashing/glitching phenomenon in the game
- Items pending testing:
  1. Game record (File I/O) function
  2. Perfection and robustness of the game algorithm

## Apr 22
- Bug Fixing by Person B:
  1. ~~Boxes on targets now locked (cannot be pushed out)~~ **Reverted on Apr 22: Boxes on targets can now be pushed out again**
  2. Undo limit per difficulty: Easy=5, Medium=3, Hard=0
  3. Undo no longer affects step count
  4. Added undo tracking to UserData; status bar shows "U: X/Y"
  5. Changed obstacle symbol from `.` to `%` since the previous `.` was too small can causes confusion.
  6. Added Leaderboard feature [L] in the Main Menu.

- Periodic Group Meeting 2
  1. Go through the game together again
  2. Reverted Changes: Boxes on targets can now be pushed out again (boxes on targets are no longer locked)
  3. IIP Refactoring: Add `Game` and `Renderer` Class, add subclass `EasyGame`, `MediumGame`, `HardGame` and `RendererWithEffects`
  4. Add Windows user guideline in PlayerGuiline.md
  5. Add gitignore to avoid rebundant files in the repository.
  6. Problem fixed:Chinese characters in `cout` displayed as garbled text on Windows (Windows console uses GBK encoding by default, not UTF-8)
  7. Add FeatureChecklist.md. Plan out for feature bug detection.





## Apr 22 (Windows Build Fix)
- Fixed Windows compile failure (`termios.h: No such file or directory`):
  - Updated `player.cpp` to use `_getch()` on Windows and `termios` on Linux/macOS
  - Updated `renderer.cpp` to use `_getch()` on Windows and `termios` on Linux/macOS
- Added `windows` target in `Game Code/Makefile` to build `blocktactics.exe`
- Updated `PlayingGuideline.md` with Windows-specific compile/run instructions:
  - `make windows` flow
  - fallback direct `g++ ... -o blocktactics.exe ...` command when `make` is unavailable
  - troubleshooting note for `termios.h` error on Windows

## Apr 22 (Windows Terminal Rendering Stability Fix)
- Fixed mojibake/garbled terminal output risk by making Windows rendering conservative by default.
- Added `Game Code/terminal.h` and `Game Code/terminal.cpp`:
  - Detect ANSI color support safely
  - Default to ASCII render mode on Windows unless Unicode confidence is explicit
  - Keep Linux/macOS behavior unchanged
- Updated renderer integration:
  - `Renderer` now uses terminal capability config (`color + render mode + ANSI support`)
  - UI boxes/headers/status now render via mode-aware helpers (ASCII/Unicode)
  - Color output is gated to avoid leaking raw escape codes when unsupported
- Updated `main.cpp`:
  - Initializes renderer using detected terminal config
  - Login/exit banner now respects ASCII/Unicode mode and safe color state
- Updated `PlayingGuideline.md`:
  - Windows compile command includes `terminal.cpp`
  - Added PowerShell-safe run command (`.\\blocktactics.exe`)
  - Added forced ASCII fallback command (`$env:BLOCKTACTICS_FORCE_ASCII="1"; .\\blocktactics.exe`)

## Apr 25 (Undo Behavior Consistency Fix)
- Fixed undo snapshot retention bug so one undo now correctly returns to exactly one previous move (instead of jumping back too far).
- Unified difficulty undo behavior to match design:
  - Easy: unlimited undo
  - Medium: 5 undos
  - Hard: no undo
- Fixed cross-level transition behavior (Medium -> Hard):
  - Undo state is reconfigured and reset by current difficulty when levels/maps are initialized.
  - Hard mode now consistently shows `U:N/A` and disables undo input after transitioning from earlier levels.

