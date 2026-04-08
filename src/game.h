#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "player.h"
#include "file_io.h"
#include <string>

// Game class - manages the entire game flow
class Game {
private:
    Map game_map;
    Player player;
    UndoSystem undo_system;
    UserData current_user;
    Difficulty current_difficulty;
    bool game_running;
    bool level_complete;
    bool user_logged_in;

    // UI functions
    void clearScreen();
    void displayWelcome();
    void displayMainMenu();
    void displayDifficultyMenu();
    void displayControls();
    void displayGameStatus();
    void displayUserProgress();
    void displayWinScreen();
    void displayHelp();

    // Game flow functions
    void loginUser();
    void selectDifficulty();
    void startNewGame();
    void continueGame();
    void processInput(char input);
    void restartLevel();
    void nextLevel();

    // Win/Save functions
    bool checkWinCondition();
    void saveProgress();
    int countCompletedTargets();

public:
    Game();
    ~Game();

    // Main game entry point
    void run();
};

#endif // GAME_H
