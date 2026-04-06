# 🧩 Block Tactics — Team Development Workflow

## 👥 Team Role Split

### Person A
- Main game loop
- Player movement
- Box pushing logic
- Restart logic
- Win condition
- Integration work

### Person B
- Map system
- Random generation
- Difficulty system
- File I/O
- Color rendering
- Supporting state structures

---

# 🌐 OVERALL FLOW

START  
→ Stage 0: Project Setup (**Done Together**)  
→ Stage 1: Basic Framework (**Can Split in Parallel**)  
→ Merge 1 (**Done Together**)  
→ Stage 2: Core Gameplay Logic (**Can Split in Parallel**)  
→ Merge 2 (**Done Together**)  
→ Stage 3: Target + Win Logic (**Can Split in Parallel**)  
→ Merge 3 (**Done Together**)  
→ Stage 4: Restart System (**Can Split in Parallel**)  
→ Merge 4 (**Done Together**)  
→ Stage 5: Random Map + Difficulty (**Mostly Parallel, then Done Together**)  
→ Merge 5 (**Done Together**)  
→ Stage 6: User Save/Load System (**Can Split in Parallel**)  
→ Merge 6 (**Done Together**)  
→ Stage 7: Optional Undo / Dynamic Memory Feature (**Can Split in Parallel**)  
→ Merge 7 (**Done Together**)  
→ Stage 8: Color System + Final UI Polish (**Can Split in Parallel**)  
→ Merge 8 (**Done Together**)  
→ Stage 9: Full Testing and Bug Fixing (**Done Together**)  
→ Stage 10: Documentation and Submission (**Can Split in Parallel, Final Check Together**)  
→ END

---

# ✅ STAGE 0 — PROJECT SETUP
## Type: DONE TOGETHER

## Goal
Both members agree on the same structure before coding starts.

## Tasks
- Create GitHub private repo
- Add teammate and instructor
- Upload final design overview
- Confirm file names:
  - main.cpp
  - map.h / map.cpp
  - player.h / player.cpp
  - file_io.h / file_io.cpp
- Agree on symbol system:
  - `@` player
  - `$` box
  - `^` target
  - `#` wall
  - `.` obstacle
- Agree on coding style
- Agree on who owns which module

## Output
- Empty project structure ready
- Everyone knows responsibilities

---

# ✅ STAGE 1 — BASIC FRAMEWORK
## Type: PARALLEL

## Person A
- Set up `main.cpp`
- Create game loop skeleton
- Create input skeleton
- Define `Player` struct
- Prepare movement function prototypes

## Person B
- Set up `map.h` and `map.cpp`
- Build 10x10 map structure
- Write map print function
- Make a fixed sample map with walls, empty cells, one player position placeholder

---

# 🔗 MERGE 1 — BASIC PLAYABLE SHELL
## Type: DONE TOGETHER

## Must be done together because
Person A’s loop must call Person B’s map system correctly.

## Joint tasks
- Connect `main.cpp` with `map.cpp`
- Display map on screen
- Place player on the map
- Make sure project compiles

## Joint checkpoint
- Program runs
- Map displays correctly
- No crash
- Everyone understands current code

## Output
- A basic displayed map with a running loop

---

# ✅ STAGE 2 — CORE GAMEPLAY LOGIC
## Type: PARALLEL

## Person A
- Implement player movement with W/A/S/D
- Add collision checking with walls and obstacles
- Add box pushing logic
- Prevent pushing two boxes
- Prevent walking into blocked cells

## Person B
- Expand map state representation
- Add boxes and targets into map
- Prepare target-position tracking structure
- Clean the print logic for map elements

---

# 🔗 MERGE 2 — CORE GAMEPLAY WORKING
## Type: DONE TOGETHER

## Must be done together because
Movement logic and map state must match exactly.

## Joint tasks
- Connect movement logic to actual map content
- Test player pushing boxes
- Test box movement onto empty cells
- Test blocked pushes

## Joint checkpoint
- Player moves correctly
- Box pushing works
- No illegal overlap
- No broken map state

## Output
- Basic Sokoban gameplay works

---

# ✅ STAGE 3 — TARGET LOGIC + WIN CONDITION
## Type: PARALLEL

## Person A
- Write win condition checking
- Count how many targets are completed
- Update game loop to detect level completion

## Person B
- Implement target state tracking
- Handle box-on-target logic correctly
- Update rendering for completed targets
- Support separate target storage or state flag approach

---

# 🔗 MERGE 3 — LEVEL COMPLETION SYSTEM
## Type: DONE TOGETHER

## Must be done together because
Win logic depends on target state logic being correct.

## Joint tasks
- Test box moved onto target
- Confirm completed target is counted logically
- Confirm level ends only when all targets are filled

## Joint checkpoint
- Visual state and logical state match
- Winning works correctly

## Output
- Level can be finished correctly

---

# ✅ STAGE 4 — RESTART SYSTEM
## Type: PARALLEL

## Person A
- Add `R` input handling
- Reset player position
- Reset step count
- Re-enter loop correctly after restart

## Person B
- Store initial map state
- Store initial box positions
- Store initial target-related state
- Provide restoration function

---

# 🔗 MERGE 4 — RESTART FULLY WORKING
## Type: DONE TOGETHER

## Must be done together because
Restart touches both control flow and map state restoration.

## Joint tasks
- Press `R` during a level
- Confirm same map returns
- Confirm box positions reset
- Confirm player resets
- Confirm step count resets

## Joint checkpoint
- Restart does not generate a new map
- Restart fully restores the original level

## Output
- Stable restart feature

---

# ✅ STAGE 5 — RANDOM MAP + DIFFICULTY
## Type: MOSTLY PARALLEL, THEN TOGETHER

## Person B (Main Owner)
- Implement random map generation
- Place boxes, targets, obstacles
- Apply constraints:
  - no corner placement
  - targets not next to outer walls
  - obstacles not blocking paths
  - box mobility validation

## Person A
- Build difficulty selection menu
- Pass difficulty parameters into game startup
- Help test generated maps
- Help detect impossible or broken maps

---

# 🔗 MERGE 5 — RANDOM PLAYABLE LEVELS
## Type: DONE TOGETHER

## Must be done together because
Difficulty menu, generation rules, and actual gameplay must connect correctly.

## Joint tasks
- Select Easy / Medium / Hard
- Confirm correct box and obstacle counts
- Test several generated maps
- Reject obviously broken layouts

## Joint checkpoint
- Difficulty changes content correctly
- Maps are playable
- No instant deadlocks

## Output
- Playable random levels with difficulties

---

# ✅ STAGE 6 — USER SYSTEM (SAVE / LOAD)
## Type: PARALLEL

## Person B
- Implement `file_io.cpp`
- Read existing user data
- Create new user record if not found
- Save highest level
- Save best step count
- Save on exit and on win

## Person A
- Add username input in game start
- Display existing user progress
- Call load/save functions from main loop
- Handle continue-from-progress behavior in flow

---

# 🔗 MERGE 6 — USER PROGRESSION WORKING
## Type: DONE TOGETHER

## Must be done together because
Game flow and file system must behave as one system.

## Joint tasks
- Test new user login
- Test existing user login
- Test save after win
- Test save after quit
- Test resume from saved progress

## Joint checkpoint
- File format stable
- User records correct
- No lost progress

## Output
- Full user progression system

---

# ✅ STAGE 7 — OPTIONAL UNDO / DYNAMIC MEMORY
## Type: PARALLEL

## Person A
- Add undo input
- Connect undo action to loop
- Restore previous step

## Person B
- Store game states in stack
- Manage state snapshots
- Ensure memory logic is safe

---

# 🔗 MERGE 7 — ADVANCED FEATURE INTEGRATION
## Type: DONE TOGETHER

## Must be done together because
Undo affects loop behavior and stored state structure.

## Joint tasks
- Test several moves
- Undo repeatedly
- Confirm map/player/box states return correctly

## Joint checkpoint
- No corrupted state
- Undo behaves predictably

## Output
- Optional advanced feature complete

---

# ✅ STAGE 8 — COLOR SYSTEM + FINAL UI POLISH
## Type: PARALLEL

## Person B
- Add ANSI color codes
- Update rendering rules:
  - player yellow
  - box red
  - target green
  - box on target bright green
  - wall gray
  - obstacle dark gray

## Person A
- Integrate color display into game loop flow
- Check restart, win, and movement still display properly
- Improve prompts and messages for player

---

# 🔗 MERGE 8 — FINAL PLAYABLE VERSION
## Type: DONE TOGETHER

## Must be done together because
Color rendering changes the displayed game and may affect readability.

## Joint tasks
- Test all symbols visually
- Test color change when box reaches target
- Confirm output remains readable

## Joint checkpoint
- UI looks clean
- Logic still correct
- Terminal display stable

## Output
- Final polished playable game

---

# ✅ STAGE 9 — FULL TESTING + BUG FIXING
## Type: DONE TOGETHER

## Must be done together because
This stage checks the whole system, not separate modules.

## Joint tasks
- Test movement
- Test pushing
- Test win detection
- Test restart
- Test random generation
- Test difficulty switching
- Test save/load
- Test optional undo
- Test color rendering
- Test edge cases

## Joint checkpoint
- No crashes
- No broken logic
- All required features present

## Output
- Submission-safe codebase

---

# ✅ STAGE 10 — DOCUMENTATION + SUBMISSION
## Type: PARTLY PARALLEL, FINAL CHECK TOGETHER

## Person A
- Finalize README
- Finalize design overview
- Finalize requirement mapping
- Help prepare demo script

## Person B
- Add function comments
- Clean code formatting
- Help record demo video
- Check GitHub commit history

---

# 🔗 FINAL MERGE — SUBMISSION PACKAGE
## Type: DONE TOGETHER

## Joint tasks
- Final code review
- Final README review
- Final compile test
- Final demo video check
- Push all files to GitHub
- Submit repo and video

## Final checkpoint
- README is complete
- Code compiles
- Video under 3 minutes
- GitHub looks clean
- Both members understand the final product

## Output
- Project fully complete and ready to submit

---

# 🏁 END STATE

You are done only when all of these are true:

- Gameplay works
- Restart works
- Random levels work
- Save/load works
- Difficulty works
- Required coding elements are present
- Code is documented
- Repo is clean
- Demo video is ready
- Submission is uploaded
