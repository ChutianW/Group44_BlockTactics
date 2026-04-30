#include "player.h"
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

/*
 * Function: Player::Player (default constructor)
 * Purpose:  Initializes the player at position (0,0) with zero steps.
 * Inputs:   None.
 * Output:   A Player object with row=0, col=0, steps=0.
 */
Player::Player() : row(0), col(0), steps(0) {}

/*
 * Function: Player::Player (parameterized constructor)
 * Purpose:  Initializes the player at a given grid position with zero steps.
 * Inputs:   start_row - the initial row index on the grid.
 *           start_col - the initial column index on the grid.
 * Output:   A Player object at (start_row, start_col) with steps=0.
 */
Player::Player(int start_row, int start_col)
    : row(start_row), col(start_col), steps(0) {}

/*
 * Function: Player::resetSteps
 * Purpose:  Resets the player's step counter to zero (used on level restart).
 * Inputs:   None.
 * Output:   Sets the steps member to 0.
 */
void Player::resetSteps() {
    steps = 0;
}

/*
 * Function: Player::incrementSteps
 * Purpose:  Increments the player's step counter by one after each valid move.
 * Inputs:   None.
 * Output:   Increases the steps member by 1.
 */
void Player::incrementSteps() {
    steps++;
}

/*
 * Function: Player::getSteps
 * Purpose:  Returns the current step count for the player this level.
 * Inputs:   None.
 * Output:   Returns the integer step count.
 */
int Player::getSteps() const {
    return steps;
}

/*
 * Function: Player::setPosition
 * Purpose:  Moves the player to a new grid position directly (used on init/restart).
 * Inputs:   new_row - the target row index.
 *           new_col - the target column index.
 * Output:   Updates row and col members to the given values.
 */
void Player::setPosition(int new_row, int new_col) {
    row = new_row;
    col = new_col;
}

/*
 * Function: UndoSystem::UndoSystem (constructor)
 * Purpose:  Initializes the undo system with a given maximum number of undos.
 *           Limits: Easy=5, Medium=3, Hard=0 (0 disables undo entirely).
 * Inputs:   max_size - the maximum number of undo steps permitted.
 * Output:   An UndoSystem with empty history and undos_left = max_size.
 */
UndoSystem::UndoSystem(int max_size) : undos_left(max_size), max_undos(max_size), used_undo(false) {}

/*
 * Function: UndoSystem::~UndoSystem (destructor)
 * Purpose:  Frees all heap-allocated UndoState objects still in the history deque.
 * Inputs:   None.
 * Output:   All dynamically allocated undo states are deleted; no memory leaks.
 */
UndoSystem::~UndoSystem() {
    clear();
}

/*
 * Function: UndoSystem::saveState
 * Purpose:  Saves a snapshot of the current game state to the undo history.
 *           Allocates a new UndoState on the heap. If history exceeds max_undos,
 *           the oldest state is trimmed and deleted.
 * Inputs:   grid   - the current 2D grid representing the game map.
 *           player - the current Player object (position and step count).
 * Output:   Appends a new UndoState to the back of the history deque.
 *           Does nothing if max_undos == 0 (undo disabled).
 */
void UndoSystem::saveState(const std::vector<std::vector<char>> &grid, const Player &player) {
    if (max_undos == 0) return;

    UndoState *state = new UndoState(grid, player.row, player.col, player.getSteps());
    history.push_back(state);

    while ((int)history.size() > max_undos) {
        UndoState *old = history.front();
        history.pop_front();
        delete old;
    }
}

/*
 * Function: UndoSystem::undo
 * Purpose:  Reverts the game to the most recently saved state (one step back).
 *           Restores grid layout and player position. Does NOT restore step count.
 *           Consumes one undo from the remaining undo budget.
 * Inputs:   grid   - the 2D grid to overwrite with the saved state.
 *           player - the Player object whose row and col will be restored.
 * Output:   Returns true and modifies grid and player if undo succeeds.
 *           Returns false if no undos remain or history is empty.
 */
bool UndoSystem::undo(std::vector<std::vector<char>> &grid, Player &player) {
    if (!canUndo()) return false;
    if (history.empty()) return false;

    UndoState *state = history.back();
    history.pop_back();

    grid = state->grid;
    player.row = state->player_row;
    player.col = state->player_col;

    undos_left--;
    used_undo = true;

    delete state;
    return true;
}

/*
 * Function: UndoSystem::clear
 * Purpose:  Deletes all saved undo states and clears the history deque.
 *           Called by the destructor and on level reset.
 * Inputs:   None.
 * Output:   All heap-allocated UndoState objects are deleted; history is empty.
 */
void UndoSystem::clear() {
    for (UndoState *state : history) {
        delete state;
    }
    history.clear();
}

/*
 * Function: UndoSystem::canUndo
 * Purpose:  Checks whether an undo operation is currently possible.
 * Inputs:   None.
 * Output:   Returns true if undos_left > 0 and there is at least one saved state.
 */
bool UndoSystem::canUndo() const {
    return undos_left > 0 && !history.empty();
}

/*
 * Function: UndoSystem::getUndosLeft
 * Purpose:  Returns the number of undo operations still available this level.
 * Inputs:   None.
 * Output:   Returns the remaining undo count as an integer.
 */
int UndoSystem::getUndosLeft() const {
    return undos_left;
}

/*
 * Function: UndoSystem::getMaxUndos
 * Purpose:  Returns the maximum number of undos allowed for the current difficulty.
 * Inputs:   None.
 * Output:   Returns the max undo limit (5=Easy, 3=Medium, 0=Hard).
 */
int UndoSystem::getMaxUndos() const {
    return max_undos;
}

/*
 * Function: UndoSystem::hasUsedUndo
 * Purpose:  Indicates whether the player has used at least one undo this level.
 * Inputs:   None.
 * Output:   Returns true if undo has been used at least once; false otherwise.
 */
bool UndoSystem::hasUsedUndo() const {
    return used_undo;
}

/*
 * Function: UndoSystem::reset
 * Purpose:  Clears all saved states and resets undo limits to a new maximum.
 *           Called when starting a new level or restarting.
 * Inputs:   new_max_undos - the new undo limit (replaces the current max).
 * Output:   History is cleared; undos_left and max_undos set to new_max_undos;
 *           used_undo reset to false.
 */
void UndoSystem::reset(int new_max_undos) {
    clear();
    max_undos = new_max_undos;
    undos_left = new_max_undos;
    used_undo = false;
}

/*
 * Function: createPlayer
 * Purpose:  Creates and returns a Player object initialized at the given position.
 * Inputs:   row - the starting row index on the grid.
 *           col - the starting column index on the grid.
 * Output:   Returns a Player with the specified position and steps set to 0.
 */
Player createPlayer(int row, int col) {
    Player p;
    p.row = row;
    p.col = col;
    p.steps = 0;
    return p;
}

/*
 * Function: getDirectionOffset
 * Purpose:  Converts a direction constant into row/column delta values.
 *           DIR_UP=(-1,0), DIR_DOWN=(+1,0), DIR_LEFT=(0,-1), DIR_RIGHT=(0,+1).
 * Inputs:   direction - one of DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT.
 *           drow      - output parameter set to the row delta.
 *           dcol      - output parameter set to the column delta.
 * Output:   Sets drow and dcol to the appropriate offsets for the given direction.
 */
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

/*
 * Function: canPushBox
 * Purpose:  Checks whether a box at the given position can be pushed in a direction.
 *           A push is valid if the cell behind the box is empty or a target,
 *           and within the map bounds.
 * Inputs:   box_row          - row index of the box to push.
 *           box_col          - column index of the box to push.
 *           direction        - direction of the push (DIR_UP/DOWN/LEFT/RIGHT).
 *           grid             - the current 2D game grid.
 *           target_positions - list of target positions (unused, reserved).
 * Output:   Returns true if the push is valid; false if blocked or out of bounds.
 */
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

/*
 * Function: pushBox
 * Purpose:  Moves a box one cell in the given direction, updating the grid.
 *           Restores the box's old cell to EMPTY or TARGET as appropriate.
 * Inputs:   box_row          - row index of the box (updated to new position).
 *           box_col          - column index of the box (updated to new position).
 *           direction        - direction to push the box.
 *           grid             - the 2D game grid (modified in place).
 *           target_positions - list of target positions to determine cell restoration.
 * Output:   Updates grid; box_row and box_col are set to the new box position.
 */
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

/*
 * Function: movePlayer
 * Purpose:  Attempts to move the player one step in the given direction.
 *           Handles box pushing, collision with walls/obstacles, and undo saving.
 *           Flow: compute target cell -> check bounds -> handle wall/box/empty cases
 *           -> update grid and player position -> increment steps.
 * Inputs:   player           - the Player object to move (modified in place).
 *           direction        - one of DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT.
 *           grid             - the 2D game grid (modified in place).
 *           target_positions - list of target positions for cell restoration.
 *           undo             - pointer to UndoSystem for saving state; nullptr skips.
 * Output:   Returns true if the move succeeded and grid/player were updated.
 *           Returns false if the move is blocked (wall, obstacle, immovable box).
 */
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

/*
 * Function: parseDirection
 * Purpose:  Maps a keyboard character to a direction constant.
 *           'W'/'w'=DIR_UP, 'S'/'s'=DIR_DOWN, 'A'/'a'=DIR_LEFT, 'D'/'d'=DIR_RIGHT.
 *           Non-movement keys (R, U, H, Q, etc.) return -1 for the caller to handle.
 * Inputs:   input - the character read from the keyboard.
 * Output:   Returns one of DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT, or -1.
 */
int parseDirection(char input) {
    switch (input) {
        case 'W': case 'w': return DIR_UP;
        case 'S': case 's': return DIR_DOWN;
        case 'A': case 'a': return DIR_LEFT;
        case 'D': case 'd': return DIR_RIGHT;
        default: return -1;
    }
}

/*
 * Function: resetPlayer
 * Purpose:  Resets the player's position to the level's starting coordinates.
 *           Does not reset step count (caller must call resetSteps separately).
 * Inputs:   player    - the Player object to reposition.
 *           start_row - the original starting row for this level.
 *           start_col - the original starting column for this level.
 * Output:   Updates player.row and player.col to the given starting position.
 */
void resetPlayer(Player &player, int start_row, int start_col) {
    player.row = start_row;
    player.col = start_col;
}
