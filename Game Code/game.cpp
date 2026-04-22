#include "game.h"
#include "file_io.h"
#include <iostream>
#include <cstdlib>

// ============================================================================
// Game base class implementation
// ============================================================================

Game::Game(Difficulty difficulty, Renderer *renderer)
    : difficulty_(difficulty),
      undo_(5),
      renderer_(renderer),
      user_(nullptr),
      game_running_(true),
      level_complete_(false) {
}

Game::~Game() {}

// Get current undo limit based on difficulty (protected helper)
int Game::getCurrentUndoLimit() const {
    return getUndoLimit();
}

void Game::setUserData(UserData *user) {
    user_ = user;
}

bool Game::isRunning() const {
    return game_running_;
}

bool Game::isLevelComplete() const {
    return level_complete_;
}

Difficulty Game::getDifficulty() const {
    return difficulty_;
}

// Polymorphic methods with default implementations
int Game::getUndoLimit() const {
    switch (difficulty_) {
        case EASY:   return 5;
        case MEDIUM: return 3;
        case HARD:   return 0;
        default:     return 5;
    }
}

int Game::getBoxCount() const {
    DifficultySettings settings = getDifficultySettings(difficulty_);
    return settings.num_boxes;
}

void Game::getObstacleRange(int &min, int &max) const {
    DifficultySettings settings = getDifficultySettings(difficulty_);
    min = settings.min_obstacles;
    max = settings.max_obstacles;
}

// Initialize level
void Game::initLevel() {
    int start_row = 1, start_col = 1;
    generateRandomMap(grid_, start_row, start_col, targets_, difficulty_);

    player_.setPosition(start_row, start_col);
    player_.resetSteps();

    undo_.reset(getUndoLimit());

    initial_state_ = saveInitialState(grid_, targets_, player_.row, player_.col);

    game_running_ = true;
    level_complete_ = false;
}

// Generate new map
void Game::generateNewMap() {
    int start_row = 1, start_col = 1;
    generateRandomMap(grid_, start_row, start_col, targets_, difficulty_);

    player_.setPosition(start_row, start_col);
    player_.resetSteps();

    undo_.reset(getUndoLimit());

    initial_state_ = saveInitialState(grid_, targets_, player_.row, player_.col);
}

// Run game main loop
void Game::run() {
    while (game_running_) {
        // 1. Clear screen
        renderer_->clearScreen();

        // 2. Display status bar
        int completed = countCompletedTargets(grid_, targets_);
        int total = getTargetCount(targets_);
        int undo_left = undo_.getUndosLeft();
        int undo_max = undo_.getMaxUndos();
        renderer_->printGameStatus(difficulty_, player_.getSteps(), completed, total, undo_left, undo_max);

        // 3. Print map
        renderer_->printMap(grid_, targets_, player_.row, player_.col);

        // 4. Operation hints
        std::cout << "  [W/A/S/D] Move | [R] Restart | [U] Undo | [H] Help | [Q] Quit\n";

        // 5. Read input
        char input = renderer_->getInput();

        // 6. Process input
        processInput(input);

        // 7. Check win condition
        if (!level_complete_ && checkWinCondition()) {
            level_complete_ = true;
            onLevelComplete();
        }
    }
}

// Start a new game (public method)
void Game::startGame() {
    initLevel();
}

// Process input
void Game::processInput(char input) {
    input = std::tolower(input);

    switch (input) {
        case 'w':
            handleMovement(DIR_UP);
            break;
        case 'a':
            handleMovement(DIR_LEFT);
            break;
        case 's':
            handleMovement(DIR_DOWN);
            break;
        case 'd':
            handleMovement(DIR_RIGHT);
            break;
        case 'r':
            handleRestart();
            break;
        case 'u':
            handleUndo();
            break;
        case 'h':
            handleHelp();
            break;
        case 'q':
        case 27:  // ESC
            handleQuit();
            break;
    }
}

// Handle movement
void Game::handleMovement(char direction) {
    movePlayer(player_, direction, grid_, targets_, &undo_);
}

// Handle restart
void Game::handleRestart() {
    int row = player_.row;
    int col = player_.col;
    restoreInitialState(grid_, targets_, row, col, player_.steps, initial_state_);
    player_.setPosition(row, col);
}

// Handle undo
void Game::handleUndo() {
    if (undo_.canUndo()) {
        undo_.undo(grid_, player_);
    }
}

// Handle help
void Game::handleHelp() {
    renderer_->printHelp();
    std::cout << "  Press any key to continue...\n";
    renderer_->getInput();
}

// Handle quit
void Game::handleQuit() {
    game_running_ = false;
}

// Check win condition
bool Game::checkWinCondition() {
    return countCompletedTargets(grid_, targets_) == getTargetCount(targets_);
}

// On level complete
void Game::onLevelComplete() {
    saveProgress();
    promptNextAction();
}

// Save progress
void Game::saveProgress() {
    if (user_ != nullptr && player_.getSteps() > 0) {
        switch (difficulty_) {
            case EASY:
                if (user_->best_steps_easy == 0 || player_.getSteps() < user_->best_steps_easy) {
                    user_->best_steps_easy = player_.getSteps();
                }
                user_->total_undos_easy += (undo_.getMaxUndos() - undo_.getUndosLeft());
                break;
            case MEDIUM:
                if (user_->best_steps_medium == 0 || player_.getSteps() < user_->best_steps_medium) {
                    user_->best_steps_medium = player_.getSteps();
                }
                user_->total_undos_medium += (undo_.getMaxUndos() - undo_.getUndosLeft());
                break;
            case HARD:
                if (user_->best_steps_hard == 0 || player_.getSteps() < user_->best_steps_hard) {
                    user_->best_steps_hard = player_.getSteps();
                }
                user_->total_undos_hard += (undo_.getMaxUndos() - undo_.getUndosLeft());
                break;
        }

        if (static_cast<int>(difficulty_) > user_->highest_level) {
            user_->highest_level = static_cast<int>(difficulty_);
        }

        saveUserData(*user_);
    }
}

// Prompt next action after win
void Game::promptNextAction() {
    while (true) {
        renderer_->printWinScreen(player_.getSteps(), *user_, difficulty_);
        char choice = renderer_->getInput();
        choice = std::tolower(choice);

        if (choice == 'n') {
            nextLevel();
            break;
        } else if (choice == 'r') {
            restart();
            break;
        } else if (choice == 'm') {
            game_running_ = false;
            break;
        }
    }
}

// Restart current level
void Game::restart() {
    handleRestart();
    level_complete_ = false;
}

// Go to next level
void Game::nextLevel() {
    if (difficulty_ < HARD) {
        difficulty_ = static_cast<Difficulty>(static_cast<int>(difficulty_) + 1);
    }
    generateNewMap();
    level_complete_ = false;
}

// ============================================================================
// EasyGame subclass - polymorphism
// ============================================================================

EasyGame::EasyGame(Renderer *renderer) : Game(EASY, renderer) {
    // Reinitialize undo system with correct limit for Easy mode
    undo_.reset(5);
}

EasyGame::~EasyGame() {}

int EasyGame::getUndoLimit() const {
    return 5;  // Easy mode has 5 undos
}

int EasyGame::getBoxCount() const {
    return 3;  // Easy mode has 3 boxes
}

void EasyGame::getObstacleRange(int &min, int &max) const {
    min = 0;
    max = 0;  // Easy mode has no obstacles
}

// ============================================================================
// MediumGame subclass - polymorphism
// ============================================================================

MediumGame::MediumGame(Renderer *renderer) : Game(MEDIUM, renderer) {
    undo_.reset(3);
}

MediumGame::~MediumGame() {}

int MediumGame::getUndoLimit() const {
    return 3;  // Medium mode has 3 undos
}

int MediumGame::getBoxCount() const {
    return 5;  // Medium mode has 5 boxes
}

void MediumGame::getObstacleRange(int &min, int &max) const {
    min = 3;
    max = 5;  // Medium mode has 3-5 obstacles
}

// ============================================================================
// HardGame subclass - polymorphism
// ============================================================================

HardGame::HardGame(Renderer *renderer) : Game(HARD, renderer) {
    undo_.reset(0);
}

HardGame::~HardGame() {}

int HardGame::getUndoLimit() const {
    return 0;  // Hard mode has no undos
}

int HardGame::getBoxCount() const {
    return 7;  // Hard mode has 7 boxes
}

void HardGame::getObstacleRange(int &min, int &max) const {
    min = 6;
    max = 10;  // Hard mode has 6-10 obstacles
}
