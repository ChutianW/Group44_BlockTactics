# 🧠 Questions to Finalize Development (Based on Latest Design Overview)

Now that your **final design overview is refined and aligned with requirements**, the next step is to resolve **key implementation decisions** before coding.

These questions are designed to:
- Prevent confusion during development  
- Avoid GitHub conflicts  
- Ensure all systems integrate smoothly  

---

# 🔵 1. Map & Random Generation

### Q1.1 — Map Representation
- Will the map store **only characters**, or will you track extra states (e.g., box on target)?
  - Option A: `vector<vector<char>>` only  
  - Option B: Add a separate structure (recommended)

---

### Q1.2 — Box on Target Detection
- How will you detect when a box is on a target?
  - Option A: Check underlying original map  
  - Option B: Use a special symbol (e.g., different char)

👉 This affects your **color system + win condition**

---

### Q1.3 — Regeneration Strategy
- When constraints fail (e.g., box not movable), how will you regenerate?
  - Regenerate **only that object**
  - Regenerate **entire map**

---

### Q1.4 — Path Validation (Important)
- Do you want to:
  - Only enforce local rules (current design)
  - OR attempt **full solvability checking** (advanced, optional)

---

# 🟡 2. Player & Movement Logic

### Q2.1 — Player Position Tracking
- Will player position be:
  - Stored in struct (recommended)
  - OR searched in map every move

---

### Q2.2 — Movement Handling
- When moving:
  - Update map directly?
  - OR use a temporary state system?

---

### Q2.3 — Box Push Logic
- When pushing a box:
  - How do you ensure:
    - Only one box is pushed
    - Next cell is valid

---

### Q2.4 — Invalid Move Feedback
- Do you want to:
  - Ignore invalid moves silently
  - OR display a message (better UX)

---

# 🟢 3. Restart & State Management

### Q3.1 — Initial State Storage
- How will you store the original map?
  - Copy of map
  - Separate structure

---

### Q3.2 — Reset Scope
When restarting, reset:
- Player position ✔  
- Box positions ✔  
- Step count ✔  

👉 Confirm all of these are implemented

---

### Q3.3 — Step Counter
- Do you want to:
  - Track number of moves
  - Display it to player
  - Save best score

---

# 🔵 4. Dynamic Memory (IMPORTANT FOR MARKS)

### Q4.1 — Undo System
- Will you implement undo?
  - Yes → use stack of game states  
  - No → use dynamic allocation elsewhere

👉 You already chose undo → confirm full implementation

---

### Q4.2 — GameState Design
What will GameState include?

- Map  
- Player position  
- Step count  

---

# 🟣 5. File I/O System

### Q5.1 — Data Format
How will you store data in `user_data.txt`?

Example:

---

### Q5.2 — Save Timing
When should saving occur?

- On exit  
- On level completion  
- Both (recommended)

---

### Q5.3 — Level Progression
- What defines progression?
  - Difficulty-based?
  - Level number-based?

---

# 🔴 6. Game Flow & UI

### Q6.1 — Menu Structure
Choose one:

- Auto-start after login  
- OR main menu:
  - Continue  
  - New Game  
  - Quit  

---

### Q6.2 — Input Handling
- Will input be:
  - Single character input
  - OR require Enter key

---

### Q6.3 — End Game Behavior
After winning:

- Go to next level?  
- Restart same difficulty?  
- Return to menu?

---

# 🟠 7. Color System (Already Designed, Needs Implementation)

### Q7.1 — Rendering Method
- Will you:
  - Print colors directly in `printMap()`
  - OR create a helper function

---

### Q7.2 — Box on Target Logic
- How exactly will you render:
  - Red `$` vs Green `$`

---

# ⚪ 8. GitHub Workflow

### Q8.1 — Branch Strategy
- Will you use:
  - feature-map  
  - feature-player  

---

### Q8.2 — Integration Timing
- When will you merge?
  - After each feature
  - Or only after full module completion

---

# ⚫ 9. Minimum Viable Version (VERY IMPORTANT)

Before everything else:

### Q9.1 — What is your FIRST working version?

Must include:
- Fixed map  
- Movement  
- Box pushing  
- Win condition  

👉 Confirm you build this FIRST before advanced features

---

# 🧭 Final Guidance

You do NOT need to answer everything at once.

👉 Priority order:

1. Map + Movement decisions  
2. Restart + Game state  
3. File I/O  
4. Undo system  
5. UI + polish  

---

# 🚀 Next Step

Reply with your answers to:

- Section 1 (Map)
- Section 2 (Movement)
- Section 3 (Restart)

Then I will:

👉 Turn your answers into a **clean coding blueprint (actual C++ structure)**  
👉 So you can start coding immediately without confusion
