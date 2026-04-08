# Stage 6 — User Save/Load System Output

## Date
2026-04-08

## Type
Feature Implementation

---

## Goal
Implement user progress saving and loading with file I/O.

---

## Tasks Completed

### Person B Tasks

#### 1. File I/O Implementation (file_io.cpp)

**User Data Structure:**
```cpp
struct UserData {
    string username;
    int highest_level;      // 1=Easy, 2=Medium, 3=Hard
    int best_steps_easy;
    int best_steps_medium;
    int best_steps_hard;
};
```

**Load User Data:**
```cpp
bool loadUserData(const string& username, UserData& data) {
    ifstream file(USER_DATA_FILE);
    // Read line by line, find matching username
    // Parse: username highest_level best_easy best_medium best_hard
}
```

**Save User Data:**
```cpp
bool saveUserData(const UserData& data) {
    // Load all users
    vector<UserData> all_users = loadAllUsers();
    // Update or add user
    // Write all back to file
}
```

#### 2. File Format
```
username1 2 45 67 0
username2 1 32 0 0
```
Format: `username highest_level best_easy best_medium best_hard`

#### 3. Save Conditions
- On level completion (win)
- On game exit
- When quitting to menu

---

### Person A Tasks

#### 1. Username Input
```cpp
void Game::loginUser() {
    cout << "Enter your username: ";
    string username;
    cin >> username;
    
    if (loadUserData(username, current_user)) {
        cout << "Welcome back!\n";
    } else {
        current_user = UserData(username);
        saveUserData(current_user);
        cout << "New user created!\n";
    }
}
```

#### 2. Display User Progress
```cpp
void Game::displayUserProgress() {
    cout << "User: " << current_user.username << "\n";
    cout << "Highest Level: " << levelName << "\n";
    cout << "Best Steps:\n";
    cout << "  Easy:   " << current_user.best_steps_easy << "\n";
    cout << "  Medium: " << current_user.best_steps_medium << "\n";
    cout << "  Hard:   " << current_user.best_steps_hard << "\n";
}
```

#### 3. Save Progress Function
```cpp
void Game::saveProgress() {
    // Update best steps if better
    switch (current_difficulty) {
        case EASY:
            if (steps < current_user.best_steps_easy || best == 0)
                current_user.best_steps_easy = steps;
            break;
        // ... similar for MEDIUM, HARD
    }
    
    // Update highest level
    if (current_difficulty > current_user.highest_level)
        current_user.highest_level = current_difficulty;
    
    saveUserData(current_user);
}
```

#### 4. Continue From Progress
```cpp
void Game::continueGame() {
    current_difficulty = static_cast<Difficulty>(current_user.highest_level);
    game_map.generateRandomMap(current_difficulty);
    // ...
}
```

---

## Merge 6 Checkpoint

### Verified Behavior
- ✅ New user login creates record
- ✅ Existing user loads saved progress
- ✅ Save after win updates best steps
- ✅ Save after quit preserves progress
- ✅ Continue resumes from highest level
- ✅ File format is stable

---

## Data Flow

```
Login → Load/Create User → Play → Win/Quit → Save Progress
                              ↓
                         Continue → Load Progress → Resume
```

---

## Files Modified

| File | Changes |
|------|---------|
| src/file_io.h | UserData struct, function declarations |
| src/file_io.cpp | Load/save implementations |
| src/game.cpp | Login, save, continue functions |
| data/user_data.txt | User data storage file |

---

## Output
- Full user progression system
- Progress persists between sessions
