# Windows Terminal Mojibake Fix (2026-04-22)

## Problem Summary
On Windows terminals, the game could show garbled mojibake-like characters and broken UI borders.

This happened because rendering and color output were too optimistic:
- Unicode box-drawing was printed in environments that were not reliably Unicode-safe.
- ANSI color sequences could be printed in terminals without confirmed ANSI support.
- Some UI output paths used direct Unicode/ANSI strings instead of conservative capability-gated rendering.

## Root Cause
1. No conservative terminal capability layer for Windows.
2. Unicode rendering was not restricted enough when terminal confidence was uncertain.
3. Color output had unsafe direct usage in several UI paths.

## What Was Changed

### 1) Added terminal capability detection
**Files added:**
- `Game Code/terminal.h`
- `Game Code/terminal.cpp`

**Behavior:**
- Detect ANSI color support safely.
- On Windows, default render mode is now **ASCII** unless Unicode confidence is explicit.
- Linux/macOS behavior remains compatible (Unicode allowed by default, NO_COLOR respected).
- Environment override: `BLOCKTACTICS_FORCE_ASCII=1` forces ASCII mode.

### 2) Integrated safe render/color behavior into renderer
**Files changed:**
- `Game Code/renderer.h`
- `Game Code/renderer.cpp`

**Changes:**
- `Renderer` now accepts terminal capability configuration:
  - `color_enabled`
  - `render_mode` (ASCII/UNICODE)
  - `ansi_color_supported`
- Added mode-aware UI helpers (box/top/bottom/row/line).
- UI sections (welcome/menu/difficulty/controls/progress/leaderboard/status/help/win screen) now render via ASCII/Unicode-safe helpers.
- Color output is gated using safe helper functions so raw escape sequences do not leak when unsupported.

### 3) Main flow now uses capability config
**File changed:**
- `Game Code/main.cpp`

**Changes:**
- Initializes renderer using `detectTerminalConfig()`.
- Login and exit banners now choose ASCII vs Unicode according to render mode.
- Login/exit color output now respects safe color state.

### 4) Build and docs updated
**Files changed:**
- `Game Code/Makefile` (includes `terminal.cpp` in source list)
- `PlayingGuideline.md` (compile/run updates including PowerShell and ASCII fallback)
- `Journal.md` (work log update)

## Why ASCII Fallback Is Safer on Windows
Windows terminals vary across:
- classic console host
- Windows Terminal
- integrated IDE terminals
- code pages and font support

When capability confidence is uncertain, ASCII guarantees readable layout and avoids mojibake.

## Validation Performed
1. Built on Windows with:
   - `g++ -std=c++17 -Wall -Wextra -o blocktactics_mojibake_fix.exe main.cpp map.cpp player.cpp file_io.cpp renderer.cpp game.cpp terminal.cpp`
2. Verified compile success after capability integration and renderer refactor.
3. Confirmed docs now include:
   - PowerShell-safe launch (`.\\blocktactics.exe`)
   - forced ASCII fallback command.

## Run Commands (Windows)
```powershell
cd "D:\港大教材\y2\comp 2113\Group44_BlockTactics_VS code\Game Code"
.\blocktactics.exe
```

If you still see rendering issues:
```powershell
$env:BLOCKTACTICS_FORCE_ASCII="1"; .\blocktactics.exe
```
