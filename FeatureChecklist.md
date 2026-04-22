# Block Tactics - Feature Checklist

## Game Environment

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 1 | Terminal UI renders correctly | Run game, observe colored symbols | [ ] |
| 2 | Colors toggle on/off | Press [5] in main menu, verify color change | [ ] |

---

## Player & Movement

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 3 | Player `@` moves up with W | Press W, player moves up | [ ] |
| 4 | Player `@` moves down with S | Press S, player moves down | [ ] |
| 5 | Player `@` moves left with A | Press A, player moves left | [ ] |
| 6 | Player `@` moves right with D | Press D, player moves right | [ ] |
| 7 | Player cannot walk through walls `#` | Walk into wall, player stays | [ ] |
| 8 | Player cannot walk through obstacles `%` | Walk into obstacle, player stays | [ ] |
| 9 | Player cannot push two boxes at once | Push box into another box, both stay | [ ] |
| 10 | Step counter increments on move | Move, check step count increases | [ ] |

---

## Boxes & Targets

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 11 | Box `$` can be pushed onto empty space | Push box to empty area | [ ] |
| 12 | Box `$` turns to `*` (bright green) when on target `^` | Push box onto target | [ ] |
| 13 | Box on target (`*`) can be pushed off | Push `*` off target, reverts to `$` | [ ] |
| 14 | Box cannot be pushed into wall | Push box into wall, box stays | [ ] |
| 15 | Box cannot be pushed into obstacle `%` | Push box into obstacle, box stays | [ ] |
| 16 | Box cannot be pushed into another box | Push box into another box, both stay | [ ] |
| 17 | Win when ALL boxes on ALL targets | Place all boxes on targets, check win screen | [ ] |

---

## Undo System

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 18 | Undo restores previous position | Move, press U, player returns | [ ] |
| 19 | Undo reverts box position | Push box, undo, box returns | [ ] |
| 20 | Undo does NOT reduce step count | Move 5 times, undo, step count stays | [ ] |
| 21 | Undo available in Easy mode (5 max) | Play Easy, undo up to 5 times | [ ] |
| 22 | Undo available in Medium mode (3 max) | Play Medium, undo up to 3 times | [ ] |
| 23 | Undo disabled in Hard mode (0 allowed) | Play Hard, press U, "undo disabled" | [ ] |
| 24 | Status bar shows "U: X/Y" correctly | Check top status bar format | [ ] |
| 25 | Undo count resets on level restart | Use undo, press R, undos restored | [ ] |

---

## Restart & Reset

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 26 | Press R restarts current level | Press R, map resets, player at start | [ ] |
| 27 | Map layout is identical after restart | Restart, compare initial map | [ ] |
| 28 | Step count resets to 0 on restart | Move, restart, step count = 0 | [ ] |
| 29 | Undo count restores to max on restart | Use undo, restart, undo count restored | [ ] |

---

## Three Difficulty Levels

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 30 | Easy: 3 boxes, 0 obstacles | Start Easy, count boxes and obstacles | [ ] |
| 31 | Medium: 5 boxes, 3-5 obstacles | Start Medium, count boxes and obstacles | [ ] |
| 32 | Hard: 7 boxes, 6-10 obstacles | Start Hard, count boxes and obstacles | [ ] |
| 33 | Each difficulty generates different maps | Play each twice, maps differ | [ ] |
| 34 | Difficulty persists after level completion | Complete Easy, still Easy | [ ] |

---

## User System

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 35 | New username can be created | Enter new name, user created | [ ] |
| 36 | Existing username loads correctly | Login with existing name, data loads | [ ] |
| 37 | Progress saved after level complete | Complete level, check user_data.txt | [ ] |
| 38 | Best steps recorded per difficulty | Complete Easy once with 20 steps, again with 15 steps, best saved | [ ] |
| 39 | Continue loads saved level and steps | Save mid-game, exit, continue, state restored | [ ] |

---

## Leaderboard

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 40 | Leaderboard accessible from main menu | Press L, leaderboard displays | [ ] |
| 41 | Higher difficulty ranks first | Complete Hard and Easy, Hard appears first | [ ] |
| 42 | Fewer (steps + undos) ranks higher in same difficulty | Complete same difficulty twice, better score first | [ ] |
| 43 | 1st/2nd/3rd shows gold/silver/orange | Visual check of rank colors | [ ] |
| 44 | Leaderboard shows username, difficulty, steps, undos, date | Compare displayed info | [ ] |

---

## Map Generation

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 45 | Player `@` placed on empty space | Start new game, player visible | [ ] |
| 46 | Boxes `$` not in corners | Check boxes are not in map corners | [ ] |
| 47 | Targets `^` not in corners | Check targets are not in map corners | [ ] |
| 48 | Boxes have at least 2 adjacent free cells | Check box mobility | [ ] |
| 49 | Obstacles `%` avoid blocking paths | Check obstacles don't create dead ends | [ ] |
| 50 | Fallback map loads if generation fails | (Tested via 50 failed retries) | [ ] |

---

## Help & UI

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 51 | Press H shows help screen | Press H, help menu appears | [ ] |
| 52 | Help screen shows all controls | Verify W/A/S/D/R/U/H/Q listed | [ ] |
| 53 | Press Q quits current game to menu | Press Q, return to main menu | [ ] |
| 54 | Welcome screen shows on startup | Run game, ASCII logo visible | [ ] |
| 55 | Status bar shows difficulty, steps, undo count | Compare header to game state | [ ] |

---

## Win Condition

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 56 | Win screen appears when all targets covered | Place last box on last target | [ ] |
| 57 | Win screen shows final step count | Check steps displayed on win | [ ] |
| 58 | Can proceed to next level after win | Press key after win, next level loads | [ ] |
| 59 | Next level increases difficulty (Easy→Medium→Hard) | Complete all Easy levels, check Medium | [ ] |

---

## OOP Features (Code Quality)

| # | Feature | Test Steps | Pass |
|---|---------|-----------|------|
| 60 | Encapsulation: private members not directly accessible | (Review code) private members have no direct access | [ ] |
| 61 | Polymorphism: virtual functions overridden in subclasses | (Review code) getUndoLimit() differs per class | [ ] |
| 62 | Composition: Game holds Renderer, Player, UndoSystem | (Review code) Game class contains these objects | [ ] |

---

**Test Summary:** `___` / 62 features passed