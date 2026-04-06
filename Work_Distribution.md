# 🧩 Block Tactics — Development Workflow (Flowchart Style)

## 👥 Team Setup
- You → Player Logic + Game Loop Integration
- Teammate → Map System + File I/O

Goal: Minimal interference, clear module ownership

---

# 🚀 STAGE 0 — Project Setup (SEQUENTIAL)

Start
→ Create GitHub repo
→ Add team members + instructor
→ Upload Final Design Overview
→ Create file structure:

    main.cpp
    map.cpp / map.h
    player.cpp / player.h
    file_io.cpp / file_io.h

→ Initial commit

---

# 🧱 STAGE 1 — Core Foundation (PARALLEL SPLIT)

                 ┌──────────────────────────────┐
                 │      PARALLEL DEVELOPMENT    │
                 └──────────────────────────────┘

🔵 YOU (Player + Game Loop)           🔴 TEAMMATE (Map System)

- Basic main loop                     - Create 10x10 grid
- Input system (WASD)                - Print map function
- Player struct                      - Place walls (#)
- Player movement logic              - Place empty spaces
- Boundary checking                  - Hardcode sample map

---

# 🔗 MERGE POINT 1 — BASIC PLAYABLE MAP

→ Integrate:
    - main.cpp calls map display
    - player moves on map

✅ RESULT:
- Player can move on map
- No box logic yet

⚠️ CHECKPOINT:
- Code compiles
- No crashes

---

# 📦 STAGE 2 — BOX MECHANICS (PARALLEL)

🔵 YOU                              🔴 TEAMMATE

- Implement push logic              - Add boxes ($)
- Collision handling                - Add targets (^)
- Prevent double push              - Store target positions
- Movement validation               - Map representation cleanup

---

# 🔗 MERGE POINT 2 — CORE GAMEPLAY

→ Integrate:
    - Player pushes boxes
    - Boxes interact with targets

✅ RESULT:
- Basic Sokoban gameplay works

⚠️ CHECKPOINT:
- No illegal moves
- No overlapping objects

---

# 🎯 STAGE 3 — WIN CONDITION + TARGET LOGIC

🔵 YOU                              🔴 TEAMMATE

- Win condition check               - Track target positions
- Count completed boxes             - Implement box-on-target state
- Game loop update                  - Update map rendering

---

# 🔗 MERGE POINT 3 — WIN SYSTEM

→ Integrate:
    - Game detects victory

✅ RESULT:
- Level can be completed

---

# 🔁 STAGE 4 — RESTART SYSTEM

🔵 YOU                              🔴 TEAMMATE

- Add 'R' input handling            - Store initial map state
- Reset player + steps              - Copy original map
- Restart logic                     - Reset box positions

---

# 🔗 MERGE POINT 4 — RESTART WORKING

✅ RESULT:
- Player can restart level
- Same map restored

⚠️ CHECKPOINT:
- No memory issues
- State fully resets

---

# 🎲 STAGE 5 — RANDOM MAP GENERATION (HIGH IMPORTANCE)

🔴 TEAMMATE (MAIN OWNER)            🔵 YOU (SUPPORT)

- Generate random layout            - Test generated maps
- Place boxes/targets               - Validate playability
- Apply constraints                 - Provide feedback
- Box mobility check
- Obstacle placement rules

---

# 🔗 MERGE POINT 5 — RANDOM SYSTEM

✅ RESULT:
- Random playable maps
- No immediate deadlocks

⚠️ CRITICAL CHECKPOINT:
- Maps must be solvable
- No blocked paths

---

# 🎚️ STAGE 6 — DIFFICULTY SYSTEM

🔴 TEAMMATE                         🔵 YOU

- Difficulty parameters             - Difficulty selection UI
- Box/obstacle scaling              - Integrate into game loop

---

# 🔗 MERGE POINT 6 — DIFFICULTY WORKING

✅ RESULT:
- Easy / Medium / Hard selectable

---

# 💾 STAGE 7 — FILE I/O SYSTEM

🔴 TEAMMATE (MAIN OWNER)            🔵 YOU

- Read/write user_data.txt          - Call save/load in main
- Store username                    - Display user info
- Save progress                     - Handle game exit

---

# 🔗 MERGE POINT 7 — USER SYSTEM

✅ RESULT:
- Login works
- Progress saved
- Resume works

⚠️ CHECKPOINT:
- File not corrupted
- Works across runs

---

# 🧠 STAGE 8 — DYNAMIC MEMORY (OPTIONAL: UNDO)

🔵 YOU                              🔴 TEAMMATE

- Implement undo stack              - Store game states
- Push/pop states                   - Optimize memory usage

---

# 🔗 MERGE POINT 8 — ADVANCED FEATURE

✅ RESULT:
- Undo works (optional bonus)

---

# 🎨 STAGE 9 — COLOR SYSTEM

🔴 TEAMMATE                         🔵 YOU

- ANSI color rendering              - Integrate in game loop
- Color mapping                     - Test visual clarity

---

# 🔗 MERGE POINT 9 — VISUAL SYSTEM

✅ RESULT:
- Colored terminal output
- Box changes color on target

---

# 🧪 STAGE 10 — TESTING & DEBUGGING (SEQUENTIAL)

→ Test all features:
    - Movement
    - Box pushing
    - Win condition
    - Restart
    - Random maps
    - File saving

→ Fix bugs

⚠️ FINAL CHECKPOINT:
- No crashes
- All requirements satisfied

---

# 📄 STAGE 11 — DOCUMENTATION (PARALLEL)

🔵 YOU                              🔴 TEAMMATE

- README.md                         - Demo video
- Final design doc                  - Code comments
- Requirement mapping               - GitHub cleanup

---

# 🔗 FINAL MERGE — SUBMISSION READY

→ Push final code to GitHub
→ Submit repo link
→ Submit video

---

# 🏁 END

Project Complete 🎯
