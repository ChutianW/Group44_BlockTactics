# Undo System Hard Mode Fix

## Root Cause
The undo behavior was not fully controlled by the *current* difficulty state.

- `Game::nextLevel()` updates `difficulty_` at runtime.
- Undo configuration depended on paths that could inherit previous mode behavior.
- Hard mode input/UI needed explicit runtime gating, not only numeric limits.

This could produce inconsistent states such as showing undo counters in Hard mode after mode transitions.

## Files Changed
- `Game Code/player.h`
- `Game Code/player.cpp`
- `Game Code/game.h`
- `Game Code/game.cpp`
- `Game Code/renderer.h`
- `Game Code/renderer.cpp`

## What Was Changed

### 1) Explicit undo control model
`UndoSystem` now tracks explicit behavior flags:
- `undo_enabled` (hard-disable support)
- `unlimited_undo` (easy unlimited mode)

`UndoSystem::reset()` now accepts:
- `reset(int new_max_undos, bool enabled = true)`

Behavior:
- Disabled undo: no state saved, no undo available, counters fixed to `0`.
- Unlimited undo: max/left tracked as `-1` and undo count is not decremented.

### 2) Difficulty-based runtime undo configuration
In `Game`:
- Added `isUndoAllowed()` and `configureUndoForCurrentDifficulty()`.
- Added `getCurrentUndoLimit()` mapping:
  - Easy: `-1` (unlimited)
  - Medium: `5` (limited)
  - Hard: `0` (disabled)

Applied this config in both:
- `initLevel()`
- `generateNewMap()`

This ensures switching difficulty (including Medium -> Hard) resets/clears undo state and prevents leakage.

### 3) Hard mode input gating
`Game::handleUndo()` now explicitly returns immediately when hard mode is active.

### 4) UI consistency updates
- Status bar (`printGameStatus`) now displays:
  - Hard/disabled: `U:N/A`
  - Easy/unlimited: `U:INF`
  - Medium/limited: `U:x/y`
- In-game control hint line hides `[U] Undo` in hard mode.
- Quick help is now contextual via `printHelp(bool undo_available)`.

## Verification / Test Results

### Build check
- Command: `mingw32-make -C "Game Code" windows`
- Result: source compilation succeeded for modified files (`player.cpp`, `renderer.cpp`, `game.cpp`), link step failed because `blocktactics.exe` output file was unavailable/locked.
- Interpretation: no compile errors introduced by this fix.

### Requirement checks
1. **Start Hard mode**
   - Status uses `undo_max == 0` -> `U:N/A`.
   - `handleUndo()` exits early when hard mode -> no state change.
2. **Medium -> Hard switch**
   - `nextLevel()` -> `generateNewMap()` -> `configureUndoForCurrentDifficulty()`.
   - Undo system is reset/cleared and disabled in Hard.
3. **Repeated `U` in Hard**
   - Guard in `handleUndo()` prevents undo path execution repeatedly.
4. **Restart in Hard**
   - `handleRestart()` restores map; undo remains disabled because difficulty config is unchanged.

## Final Behavior Summary
- Easy: unlimited undo (`U:INF`)
- Medium: limited undo (`U:5` style countdown)
- Hard: no undo (`U:N/A`, no undo action)
- Restart remains available in all modes.
