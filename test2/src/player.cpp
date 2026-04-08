#include "player.h"

// Default constructor
Player::Player() : x(0), y(0), step_count(0) {}

// Constructor with starting position
Player::Player(int start_x, int start_y) : x(start_x), y(start_y), step_count(0) {}

// Reset step count
void Player::resetSteps() {
    step_count = 0;
}

// Increment step count
void Player::incrementSteps() {
    step_count++;
}

// Get current step count
int Player::getSteps() const {
    return step_count;
}

// Set position
void Player::setPosition(int new_x, int new_y) {
    x = new_x;
    y = new_y;
}

// ============== UNDO SYSTEM (Dynamic Memory) ==============

// Constructor
UndoSystem::UndoSystem(int max_size) : max_history(max_size) {}

// Destructor - clean up dynamic memory
UndoSystem::~UndoSystem() {
    clear();
}

// Save current state
void UndoSystem::saveState(const Map& game_map, const Player& player) {
    // Create new state using dynamic memory
    GameState* state = new GameState(
        game_map.getGrid(),
        player.x,
        player.y,
        player.getSteps()
    );
    
    history.push(state);
    
    // Limit history size
    if ((int)history.size() > max_history) {
        stack<GameState*> temp;
        while ((int)history.size() > max_history) {
            GameState* old = history.top();
            history.pop();
            delete old;
        }
    }
}

// Undo last move
bool UndoSystem::undo(Map& game_map, Player& player) {
    if (history.empty()) {
        return false;
    }
    
    GameState* state = history.top();
    history.pop();
    
    // Restore state
    game_map.setGrid(state->grid);
    player.x = state->player_x;
    player.y = state->player_y;
    player.step_count = state->step_count;
    
    // Free memory
    delete state;
    
    return true;
}

// Clear all history
void UndoSystem::clear() {
    while (!history.empty()) {
        GameState* state = history.top();
        history.pop();
        delete state;
    }
}

// Check if undo is available
bool UndoSystem::canUndo() const {
    return !history.empty();
}

// Get history size
int UndoSystem::getHistorySize() const {
    return history.size();
}

// ============== MOVEMENT FUNCTIONS ==============

// Get direction offsets
void getDirectionOffset(Direction dir, int& dx, int& dy) {
    dx = 0;
    dy = 0;
    switch (dir) {
        case UP:
            dy = -1;
            break;
        case DOWN:
            dy = 1;
            break;
        case LEFT:
            dx = -1;
            break;
        case RIGHT:
            dx = 1;
            break;
    }
}

// Check if player can move in a direction
bool canMove(const Player& player, const Map& game_map, Direction dir) {
    int dx, dy;
    getDirectionOffset(dir, dx, dy);

    int new_x = player.x + dx;
    int new_y = player.y + dy;

    // Check bounds
    if (!game_map.isValidPosition(new_x, new_y)) {
        return false;
    }

    char target_cell = game_map.getCell(new_x, new_y);

    // Can move to empty space or target
    if (target_cell == EMPTY || target_cell == TARGET) {
        return true;
    }

    // Can push box if space behind it is free
    if (target_cell == BOX || target_cell == BOX_ON_TARGET) {
        return canPushBox(game_map, new_x, new_y, dir);
    }

    return false;
}

// Check if a box can be pushed
bool canPushBox(const Map& game_map, int box_x, int box_y, Direction dir) {
    int dx, dy;
    getDirectionOffset(dir, dx, dy);

    int behind_x = box_x + dx;
    int behind_y = box_y + dy;

    // Check bounds
    if (!game_map.isValidPosition(behind_x, behind_y)) {
        return false;
    }

    char behind_cell = game_map.getCell(behind_x, behind_y);

    // Can push if space behind is empty or target
    return behind_cell == EMPTY || behind_cell == TARGET;
}

// Move player in a direction
bool movePlayer(Player& player, Map& game_map, Direction dir, UndoSystem* undo) {
    if (!canMove(player, game_map, dir)) {
        return false;
    }

    // Save state before move (for undo)
    if (undo != nullptr) {
        undo->saveState(game_map, player);
    }

    int dx, dy;
    getDirectionOffset(dir, dx, dy);

    int new_x = player.x + dx;
    int new_y = player.y + dy;

    char target_cell = game_map.getCell(new_x, new_y);

    // Handle box pushing
    if (target_cell == BOX || target_cell == BOX_ON_TARGET) {
        int box_new_x = new_x + dx;
        int box_new_y = new_y + dy;

        // Move box to new position
        if (game_map.isTarget(box_new_x, box_new_y)) {
            game_map.setCell(box_new_x, box_new_y, BOX_ON_TARGET);
        } else {
            game_map.setCell(box_new_x, box_new_y, BOX);
        }

        // Restore target if box was on target
        if (target_cell == BOX_ON_TARGET) {
            game_map.setCell(new_x, new_y, TARGET);
        } else {
            game_map.setCell(new_x, new_y, EMPTY);
        }
    }

    // Clear old player position
    if (game_map.isTarget(player.x, player.y)) {
        game_map.setCell(player.x, player.y, TARGET);
    } else {
        game_map.setCell(player.x, player.y, EMPTY);
    }

    // Move player to new position
    game_map.setCell(new_x, new_y, PLAYER);
    player.x = new_x;
    player.y = new_y;

    // Increment step count
    player.incrementSteps();

    return true;
}
