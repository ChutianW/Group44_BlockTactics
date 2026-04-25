#include "player.h"
#include <iostream>

// ============================================================
// Player struct member functions
// ============================================================

Player::Player() : row(0), col(0), steps(0) {}

Player::Player(int start_row, int start_col)
    : row(start_row), col(start_col), steps(0) {}

void Player::resetSteps() {
    steps = 0;
}

void Player::incrementSteps() {
    steps++;
}

int Player::getSteps() const {
    return steps;
}

void Player::setPosition(int new_row, int new_col) {
    row = new_row;
    col = new_col;
}

// ============================================================
// Undo System (Dynamic Memory) - Uses new/delete for state snapshots
// Limits: Easy=5, Medium=3, Hard=0
// Undo restores position but not step count, consumes one undo
// ============================================================

UndoSystem::UndoSystem(int max_size)
    : undos_left(max_size),
      max_undos(max_size),
      used_undo(false),
      undo_enabled(max_size != 0),
      unlimited_undo(max_size < 0) {
    if (!undo_enabled) {
        undos_left = 0;
        max_undos = 0;
    } else if (unlimited_undo) {
        undos_left = -1;
    }
}

UndoSystem::~UndoSystem() {
    clear();
}

// Save current state to history (allocates with new)
void UndoSystem::saveState(const std::vector<std::vector<char>> &grid, const Player &player) {
    if (!undo_enabled) return;

    UndoState *state = new UndoState(grid, player.row, player.col, player.getSteps());
    history.push(state);

    while (!unlimited_undo && (int)history.size() > max_undos) {
        std::stack<UndoState *> temp;
        while (history.size() > 1) {
            temp.push(history.top());
            history.pop();
        }

        UndoState *oldest = history.top();
        history.pop();
        delete oldest;

        while (!temp.empty()) {
            history.push(temp.top());
            temp.pop();
        }
    }
}

// Undo last move, restore previous state
bool UndoSystem::undo(std::vector<std::vector<char>> &grid, Player &player) {
    if (!canUndo()) return false;

    UndoState *state = history.top();
    history.pop();

    grid = state->grid;
    player.row = state->player_row;
    player.col = state->player_col;

    if (!unlimited_undo) {
        undos_left--;
    }
    used_undo = true;

    delete state;
    return true;
}

// Clear all history and free memory
void UndoSystem::clear() {
    while (!history.empty()) {
        UndoState *state = history.top();
        history.pop();
        delete state;
    }
    history = std::stack<UndoState *>();
}

bool UndoSystem::canUndo() const {
    if (!undo_enabled || history.empty()) {
        return false;
    }
    if (unlimited_undo) {
        return true;
    }
    return undos_left > 0;
}

int UndoSystem::getUndosLeft() const {
    return undos_left;
}

int UndoSystem::getMaxUndos() const {
    return max_undos;
}

bool UndoSystem::hasUsedUndo() const {
    return used_undo;
}

bool UndoSystem::isEnabled() const {
    return undo_enabled;
}

bool UndoSystem::isUnlimited() const {
    return unlimited_undo;
}

void UndoSystem::reset(int new_max_undos, bool enabled) {
    clear();
    undo_enabled = enabled;
    unlimited_undo = enabled && new_max_undos < 0;
    if (!undo_enabled) {
        max_undos = 0;
        undos_left = 0;
    } else {
        max_undos = new_max_undos;
        undos_left = unlimited_undo ? -1 : new_max_undos;
    }
    used_undo = false;
}

// ============================================================
// Create player with initial position
// ============================================================
Player createPlayer(int row, int col) {
    Player p;
    p.row = row;
    p.col = col;
    p.steps = 0;
    return p;
}

// ============================================================
// Get row/col offset for direction
//   DIR_UP:    -1, 0
//   DIR_DOWN:  +1, 0
//   DIR_LEFT:   0, -1
//   DIR_RIGHT:  0, +1
// ============================================================
void getDirectionOffset(int direction, int &drow, int &dcol) {
    drow = 0;
    dcol = 0;
    switch (direction) {
        case DIR_UP:    drow = -1; break;
        case DIR_DOWN:  drow = 1;  break;
        case DIR_LEFT:  dcol = -1; break;
        case DIR_RIGHT: dcol = 1;  break;
    }
}

// ============================================================
// Check if box can be pushed
// Next cell must be empty or target (not wall/obstacle/another box)
// ============================================================
bool canPushBox(int box_row, int box_col, int direction,
                const std::vector<std::vector<char>> &grid,
                const std::vector<std::pair<int, int>> &target_positions) {
    (void)target_positions;
    int drow, dcol;
    getDirectionOffset(direction, drow, dcol);

    int behind_row = box_row + drow;
    int behind_col = box_col + dcol;

    if (behind_row < 0 || behind_row >= MAP_ROWS || behind_col < 0 || behind_col >= MAP_COLS) {
        return false;
    }

    char behind_cell = grid[behind_row][behind_col];
    return behind_cell == SYMBOL_EMPTY || behind_cell == SYMBOL_TARGET;
}

// ============================================================
// Push box in direction
// Restore old cell to empty or target based on position
// ============================================================
void pushBox(int &box_row, int &box_col, int direction,
             std::vector<std::vector<char>> &grid,
             const std::vector<std::pair<int, int>> &target_positions) {
    int drow, dcol;
    getDirectionOffset(direction, drow, dcol);

    int new_row = box_row + drow;
    int new_col = box_col + dcol;

    grid[new_row][new_col] = SYMBOL_BOX;

    bool was_on_target = false;
    for (const auto &pos : target_positions) {
        if (pos.first == box_col && pos.second == box_row) {
            was_on_target = true;
            break;
        }
    }
    grid[box_row][box_col] = was_on_target ? SYMBOL_TARGET : SYMBOL_EMPTY;

    box_row = new_row;
    box_col = new_col;
}

// ============================================================
// Move player - Core game logic
//
// Flow:
//   1. Calculate target cell (new_row, new_col)
//   2. Check bounds -> return false
//   3. Check target cell:
//      a) Wall(#) or Obstacle(%) -> blocked, return false
//      b) Box($) -> try push:
//         - canPushBox() returns false -> can't push, return false
//         - canPushBox() returns true -> push box, player moves
//      c) Empty(' ') or Target(^) -> move directly
//   4. Update grid: clear old, set new
//   5. Update player position, increment steps
//   6. Save to undo system (if undo != nullptr)
// ============================================================
bool movePlayer(Player &player, int direction,
                std::vector<std::vector<char>> &grid,
                const std::vector<std::pair<int, int>> &target_positions,
                UndoSystem *undo) {
    int drow, dcol;
    getDirectionOffset(direction, drow, dcol);

    int new_row = player.row + drow;
    int new_col = player.col + dcol;

    if (new_row < 0 || new_row >= MAP_ROWS || new_col < 0 || new_col >= MAP_COLS) {
        return false;
    }

    char target_cell = grid[new_row][new_col];

    if (target_cell == SYMBOL_WALL || target_cell == SYMBOL_OBSTACLE) {
        return false;
    }

    if (target_cell == SYMBOL_BOX) {
        if (!canPushBox(new_row, new_col, direction, grid, target_positions)) {
            return false;
        }

        if (undo != nullptr) {
            undo->saveState(grid, player);
        }

        int box_row = new_row;
        int box_col = new_col;
        pushBox(box_row, box_col, direction, grid, target_positions);

        bool was_on_target = false;
        for (const auto &pos : target_positions) {
            if (pos.first == player.col && pos.second == player.row) {
                was_on_target = true;
                break;
            }
        }
        grid[player.row][player.col] = was_on_target ? SYMBOL_TARGET : SYMBOL_EMPTY;

        grid[new_row][new_col] = SYMBOL_PLAYER;
        player.row = new_row;
        player.col = new_col;

    } else if (target_cell == SYMBOL_EMPTY || target_cell == SYMBOL_TARGET) {
        if (undo != nullptr) {
            undo->saveState(grid, player);
        }

        bool was_on_target = false;
        for (const auto &pos : target_positions) {
            if (pos.first == player.col && pos.second == player.row) {
                was_on_target = true;
                break;
            }
        }
        grid[player.row][player.col] = was_on_target ? SYMBOL_TARGET : SYMBOL_EMPTY;

        grid[new_row][new_col] = SYMBOL_PLAYER;
        player.row = new_row;
        player.col = new_col;

    } else {
        return false;
    }

    player.incrementSteps();
    return true;
}

// ============================================================
// Parse keyboard input to direction
//
//   'W'/'w' -> DIR_UP
//   'S'/'s' -> DIR_DOWN
//   'A'/'a' -> DIR_LEFT
//   'D'/'d' -> DIR_RIGHT
//   others   -> -1 (handled by caller: R/Q/U/H)
// ============================================================
int parseDirection(char input) {
    switch (input) {
        case 'W': case 'w': return DIR_UP;
        case 'S': case 's': return DIR_DOWN;
        case 'A': case 'a': return DIR_LEFT;
        case 'D': case 'd': return DIR_RIGHT;
        default: return -1;
    }
}

// Reset player to initial position
void resetPlayer(Player &player, int start_row, int start_col) {
    player.row = start_row;
    player.col = start_col;
}
