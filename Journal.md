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

## Apr 27
- Hard difficulty rebalance:
  1. Reduced Hard obstacle range from 6-10 to 5-7 in `map.cpp` `getDifficultySettings()` and `game.cpp` `HardGame::getObstacleRange()`.
  2. Reason: 10x10 map with 7 boxes + 7 targets + up to 10 obstacles produced too many unsolvable Hard maps; undo is disabled in Hard, so unsolvable starts were unrecoverable.
  3. Result: more varied Hard maps, faster generation (less retry on dead-zone rejection), still challenging but reliably solvable.
  4. UI / docs synced: difficulty menu (`renderer.cpp`), `README.md`, `PlayingGuideline.md`, `FeatureChecklist.md`.
  5. Hard still shows U:N/A (undo disabled) — unchanged.

## Apr 28
- Fixed critical undo bug on Hard difficulty:
  1. Bug: advancing from Easy/Medium to Hard via "Next Level" kept undo enabled (showed U:3/5 or U:5/5 instead of U:N/A).
  2. Root cause: subclass overrides of `getUndoLimit()` returned hardcoded values (e.g. `MediumGame::getUndoLimit()` always returned 3), ignoring the updated `difficulty_` member after `nextLevel()` changed it to HARD.
  3. Fix: removed `getUndoLimit()` overrides from `EasyGame`, `MediumGame`, `HardGame`. The base `Game::getUndoLimit()` already uses a switch on `difficulty_` and returns the correct limit (Easy=5, Medium=3, Hard=0). Polymorphism still demonstrated via `getBoxCount()` and `getObstacleRange()`.
  4. Also fixed undo history data structure (stack→deque) so undo goes back one step instead of jumping to earliest state.

