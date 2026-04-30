#include "game.h"
#include "file_io.h"
#include <iostream>
#include <cstdlib>

/*
 * Function: Game::Game (constructor)
 * Purpose:  Initializes a Game object with a given difficulty and renderer.
 *           Sets undo limit to 5 initially (overridden in subclass constructors),
 *           marks the game as running, and clears the level-complete flag.
 * Inputs:   difficulty - the difficulty level (EASY, MEDIUM, or HARD).
 *           renderer   - pointer to the Renderer used for all display output.
 * Output:   A Game object ready to call startGame() and run().
 */
Game::Game(Difficulty difficulty, Renderer *renderer)
    : difficulty_(difficulty),
      undo_(5),
      renderer_(renderer),
      user_(nullptr),
      game_running_(true),
      level_complete_(false) {
}

/*
 * Function: Game::~Game (destructor)
 * Purpose:  Virtual destructor enabling safe deletion of subclass objects.
 * Inputs:   None.
 * Output:   Cleans up base class resources (UndoSystem clears its heap states).
 */
Game::~Game() {}

/*
 * Function: Game::getCurrentUndoLimit
 * Purpose:  Returns the undo limit for the current difficulty by delegating to
 *           the virtual getUndoLimit() so subclass overrides are respected.
 * Inputs:   None.
 * Output:   Returns the integer undo limit appropriate for this game's difficulty.
 */
int Game::getCurrentUndoLimit() const {
    return getUndoLimit();
}

/*
 * Function: Game::setUserData
 * Purpose:  Associates a UserData record with this game session so that progress
 *           (best steps, undo counts) can be saved on level completion.
 * Inputs:   user - pointer to the UserData struct for the currently logged-in user.
 * Output:   Sets the internal user_ pointer; does not copy the struct.
 */
void Game::setUserData(UserData *user) {
    user_ = user;
}

/*
 * Function: Game::isRunning
 * Purpose:  Reports whether the game loop is still active.
 * Inputs:   None.
 * Output:   Returns true while the game is running; false after the player quits.
 */
bool Game::isRunning() const {
    return game_running_;
}

/*
 * Function: Game::isLevelComplete
 * Purpose:  Reports whether all boxes have been placed on targets this level.
 * Inputs:   None.
 * Output:   Returns true if the win condition has been met; false otherwise.
 */
bool Game::isLevelComplete() const {
    return level_complete_;
}

/*
 * Function: Game::getDifficulty
 * Purpose:  Returns the current difficulty level of this game session.
 * Inputs:   None.
 * Output:   Returns EASY, MEDIUM, or HARD.
 */
Difficulty Game::getDifficulty() const {
    return difficulty_;
}

/*
 * Function: Game::getUndoLimit
 * Purpose:  Returns the maximum number of undos allowed based on the current
 *           difficulty. Subclasses can override for custom behavior.
 *           Easy=5, Medium=3, Hard=0.
 * Inputs:   None.
 * Output:   Returns the integer undo limit for the current difficulty.
 */
int Game::getUndoLimit() const {
    switch (difficulty_) {
        case EASY:   return 5;
        case MEDIUM: return 3;
        case HARD:   return 0;
        default:     return 5;
    }
}

/*
 * Function: Game::getBoxCount
 * Purpose:  Returns the number of boxes (and matching targets) for this difficulty.
 *           Delegates to DifficultySettings. Subclasses override to hard-code values.
 * Inputs:   None.
 * Output:   Returns the integer box count (3=Easy, 5=Medium, 7=Hard).
 */
int Game::getBoxCount() const {
    DifficultySettings settings = getDifficultySettings(difficulty_);
    return settings.num_boxes;
}

/*
 * Function: Game::getObstacleRange
 * Purpose:  Returns the minimum and maximum number of obstacles to place for
 *           the current difficulty. Subclasses override to hard-code values.
 * Inputs:   min - output parameter set to the minimum obstacle count.
 *           max - output parameter set to the maximum obstacle count.
 * Output:   Sets min and max via reference (0/0=Easy, 3/5=Medium, 5/7=Hard).
 */
void Game::getObstacleRange(int &min, int &max) const {
    DifficultySettings settings = getDifficultySettings(difficulty_);
    min = settings.min_obstacles;
    max = settings.max_obstacles;
}

/*
 * Function: Game::initLevel
 * Purpose:  Generates a new random map and sets up all game state for a fresh level.
 *           Resets player position, step count, undo system, and saves initial state
 *           for level restart.
 * Inputs:   None.
 * Output:   Populates grid_, targets_, player_, undo_, and initial_state_.
 *           Sets game_running_=true and level_complete_=false.
 */
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

/*
 * Function: Game::generateNewMap
 * Purpose:  Generates a brand-new random map and resets position, steps, and undo
 *           history without returning to the main menu. Used by handleRegenerate().
 * Inputs:   None.
 * Output:   Updates grid_, targets_, player_, undo_, and initial_state_ with
 *           a newly generated map at the current difficulty.
 */
void Game::generateNewMap() {
    int start_row = 1, start_col = 1;
    generateRandomMap(grid_, start_row, start_col, targets_, difficulty_);

    player_.setPosition(start_row, start_col);
    player_.resetSteps();

    undo_.reset(getUndoLimit());

    initial_state_ = saveInitialState(grid_, targets_, player_.row, player_.col);
}

/*
 * Function: Game::run
 * Purpose:  Main game loop. Continuously clears screen, renders status bar and map,
 *           reads a keypress, processes input, and checks the win condition until
 *           game_running_ becomes false.
 * Inputs:   None.
 * Output:   Drives the entire in-game experience; terminates when the player quits
 *           or returns to the menu.
 */
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
        std::cout << "  [W/A/S/D] Move | [R] Restart | [U] Undo | [G] New Map | [H] Help | [Q] Quit\n";

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

/*
 * Function: Game::startGame
 * Purpose:  Public entry point that triggers level initialization before run().
 *           Called from main.cpp after creating a Game subclass instance.
 * Inputs:   None.
 * Output:   Calls initLevel() to set up the grid and player for the first level.
 */
void Game::startGame() {
    initLevel();
}

/*
 * Function: Game::processInput
 * Purpose:  Dispatches a keypress to the appropriate handler method.
 *           W/A/S/D -> handleMovement, R -> handleRestart, U -> handleUndo,
 *           G -> handleRegenerate, H -> handleHelp, Q/ESC -> handleQuit.
 * Inputs:   input - the character read from the keyboard.
 * Output:   Calls the corresponding handler; modifies game state accordingly.
 */
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
        case 'g':
            handleRegenerate();
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

/*
 * Function: Game::handleMovement
 * Purpose:  Delegates a movement action to the player movement system.
 *           Passes the undo system so a state snapshot is saved before moving.
 * Inputs:   direction - a direction constant (DIR_UP/DOWN/LEFT/RIGHT).
 * Output:   Calls movePlayer() which updates grid_ and player_ if the move is valid.
 */
void Game::handleMovement(char direction) {
    movePlayer(player_, direction, grid_, targets_, &undo_);
}

/*
 * Function: Game::handleRestart
 * Purpose:  Restores the level to its initial state (grid, player position, steps).
 *           Used when the player presses R or chooses to replay after winning.
 * Inputs:   None.
 * Output:   Overwrites grid_, targets_, and player_ from initial_state_.
 *           Step count is reset to the initial snapshot value (0).
 */
void Game::handleRestart() {
    int row = player_.row;
    int col = player_.col;
    restoreInitialState(grid_, targets_, row, col, player_.steps, initial_state_);
    player_.setPosition(row, col);
}

/*
 * Function: Game::handleUndo
 * Purpose:  Reverts the game one step if undos are available for the difficulty.
 *           Has no effect on Hard difficulty (undo limit = 0).
 * Inputs:   None.
 * Output:   Calls undo_.undo() which restores grid_ and player_ from history
 *           and decrements the remaining undo count.
 */
void Game::handleUndo() {
    if (undo_.canUndo()) {
        undo_.undo(grid_, player_);
    }
}

/*
 * Function: Game::handleRegenerate
 * Purpose:  Generates a completely new map at the same difficulty when the player
 *           presses G. Resets player position, steps, and undo history.
 * Inputs:   None.
 * Output:   Calls generateNewMap(); prints a confirmation message; clears
 *           level_complete_ flag.
 */
void Game::handleRegenerate() {
    generateNewMap();
    level_complete_ = false;
    std::cout << "\n  New map generated.\n";
}

/*
 * Function: Game::handleHelp
 * Purpose:  Displays the quick-help overlay and waits for a keypress to dismiss it.
 * Inputs:   None.
 * Output:   Prints help text via renderer_->printHelp(); blocks until key press.
 */
void Game::handleHelp() {
    renderer_->printHelp();
    std::cout << "  Press any key to continue...\n";
    renderer_->getInput();
}

/*
 * Function: Game::handleQuit
 * Purpose:  Ends the game loop by setting the running flag to false.
 *           Triggered by Q or ESC. Returns control to the main menu in main.cpp.
 * Inputs:   None.
 * Output:   Sets game_running_ = false, ending the run() loop.
 */
void Game::handleQuit() {
    game_running_ = false;
}

/*
 * Function: Game::checkWinCondition
 * Purpose:  Checks whether all boxes have been pushed onto their target positions.
 * Inputs:   None.
 * Output:   Returns true if countCompletedTargets == getTargetCount; false otherwise.
 */
bool Game::checkWinCondition() {
    return countCompletedTargets(grid_, targets_) == getTargetCount(targets_);
}

/*
 * Function: Game::onLevelComplete
 * Purpose:  Called immediately after a win is detected. Saves progress to file
 *           and shows the post-win prompt for the player's next action.
 * Inputs:   None.
 * Output:   Calls saveProgress() then promptNextAction().
 */
void Game::onLevelComplete() {
    saveProgress();
    promptNextAction();
}

/*
 * Function: Game::saveProgress
 * Purpose:  Updates the UserData record with the best step count and total undos
 *           used for the completed difficulty level, then persists it to file.
 *           Best steps are only updated if the new score is lower than the previous.
 * Inputs:   None.
 * Output:   Modifies user_->best_steps_* and user_->total_undos_* fields, then
 *           calls saveUserData() to write changes to data/user_data.txt.
 */
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

/*
 * Function: Game::promptNextAction
 * Purpose:  Displays the win screen and waits for the player to choose
 *           N (Next Level), R (Replay), or M (Return to Menu).
 * Inputs:   None.
 * Output:   Calls nextLevel(), restart(), or sets game_running_=false depending
 *           on the player's choice.
 */
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

/*
 * Function: Game::restart
 * Purpose:  Restores the level to its initial state and clears the level-complete
 *           flag. Delegates position/grid restore to handleRestart().
 * Inputs:   None.
 * Output:   Resets grid_, player_, and step count; sets level_complete_=false.
 */
void Game::restart() {
    handleRestart();
    level_complete_ = false;
}

/*
 * Function: Game::nextLevel
 * Purpose:  Advances the difficulty by one step (Easy->Medium->Hard) and generates
 *           a new map. If already at Hard, stays at Hard with a new map.
 * Inputs:   None.
 * Output:   Increments difficulty_ (if below HARD), calls generateNewMap(),
 *           and sets level_complete_=false.
 */
void Game::nextLevel() {
    if (difficulty_ < HARD) {
        difficulty_ = static_cast<Difficulty>(static_cast<int>(difficulty_) + 1);
    }
    generateNewMap();
    level_complete_ = false;
}

/*
 * Function: EasyGame::EasyGame (constructor)
 * Purpose:  Constructs an Easy difficulty game, setting undo limit to 5.
 * Inputs:   renderer - pointer to the Renderer used for all display output.
 * Output:   An EasyGame ready to play with 3 boxes, no obstacles, 5 undos.
 */
EasyGame::EasyGame(Renderer *renderer) : Game(EASY, renderer) {
    undo_.reset(getUndoLimit());
}

/*
 * Function: EasyGame::~EasyGame (destructor)
 * Purpose:  Virtual destructor for safe polymorphic deletion of EasyGame.
 * Inputs:   None.
 * Output:   Delegates cleanup to base class destructor.
 */
EasyGame::~EasyGame() {}

/*
 * Function: EasyGame::getBoxCount
 * Purpose:  Returns the number of boxes for Easy difficulty.
 * Inputs:   None.
 * Output:   Returns 3.
 */
int EasyGame::getBoxCount() const {
    return 3;  // Easy mode has 3 boxes
}

/*
 * Function: EasyGame::getObstacleRange
 * Purpose:  Returns the obstacle count range for Easy difficulty (none).
 * Inputs:   min - output set to 0.
 *           max - output set to 0.
 * Output:   Sets min=0 and max=0 (no obstacles on Easy).
 */
void EasyGame::getObstacleRange(int &min, int &max) const {
    min = 0;
    max = 0;  // Easy mode has no obstacles
}

/*
 * Function: MediumGame::MediumGame (constructor)
 * Purpose:  Constructs a Medium difficulty game, setting undo limit to 3.
 * Inputs:   renderer - pointer to the Renderer used for all display output.
 * Output:   A MediumGame ready to play with 5 boxes, 3-5 obstacles, 3 undos.
 */
MediumGame::MediumGame(Renderer *renderer) : Game(MEDIUM, renderer) {
    undo_.reset(getUndoLimit());
}

/*
 * Function: MediumGame::~MediumGame (destructor)
 * Purpose:  Virtual destructor for safe polymorphic deletion of MediumGame.
 * Inputs:   None.
 * Output:   Delegates cleanup to base class destructor.
 */
MediumGame::~MediumGame() {}

/*
 * Function: MediumGame::getBoxCount
 * Purpose:  Returns the number of boxes for Medium difficulty.
 * Inputs:   None.
 * Output:   Returns 5.
 */
int MediumGame::getBoxCount() const {
    return 5;  // Medium mode has 5 boxes
}

/*
 * Function: MediumGame::getObstacleRange
 * Purpose:  Returns the obstacle count range for Medium difficulty.
 * Inputs:   min - output set to 3.
 *           max - output set to 5.
 * Output:   Sets min=3 and max=5.
 */
void MediumGame::getObstacleRange(int &min, int &max) const {
    min = 3;
    max = 5;  // Medium mode has 3-5 obstacles
}

/*
 * Function: HardGame::HardGame (constructor)
 * Purpose:  Constructs a Hard difficulty game, setting undo limit to 0 (disabled).
 * Inputs:   renderer - pointer to the Renderer used for all display output.
 * Output:   A HardGame ready to play with 7 boxes, 5-7 obstacles, no undos.
 */
HardGame::HardGame(Renderer *renderer) : Game(HARD, renderer) {
    undo_.reset(getUndoLimit());
}

/*
 * Function: HardGame::~HardGame (destructor)
 * Purpose:  Virtual destructor for safe polymorphic deletion of HardGame.
 * Inputs:   None.
 * Output:   Delegates cleanup to base class destructor.
 */
HardGame::~HardGame() {}

/*
 * Function: HardGame::getBoxCount
 * Purpose:  Returns the number of boxes for Hard difficulty.
 * Inputs:   None.
 * Output:   Returns 7.
 */
int HardGame::getBoxCount() const {
    return 7;  // Hard mode has 7 boxes
}

/*
 * Function: HardGame::getObstacleRange
 * Purpose:  Returns the obstacle count range for Hard difficulty.
 * Inputs:   min - output set to 5.
 *           max - output set to 7.
 * Output:   Sets min=5 and max=7.
 */
void HardGame::getObstacleRange(int &min, int &max) const {
    min = 5;
    max = 7;  // Hard mode has 5-7 obstacles (reduced from 6-10 for better solvability)
}
