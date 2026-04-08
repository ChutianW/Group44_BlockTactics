#include "game.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constructor
Game::Game() : current_difficulty(EASY), game_running(false), 
               level_complete(false), user_logged_in(false) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

// Destructor
Game::~Game() {
    if (user_logged_in) {
        saveProgress();
    }
}

// Clear screen
void Game::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Display welcome screen
void Game::displayWelcome() {
    clearScreen();
    cout << COLOR_CYAN << COLOR_BOLD;
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════╗\n";
    cout << "  ║                                          ║\n";
    cout << "  ║         🧩 BLOCK TACTICS 🧩              ║\n";
    cout << "  ║                                          ║\n";
    cout << "  ║      A Sokoban-Style Puzzle Game         ║\n";
    cout << "  ║                                          ║\n";
    cout << "  ║         COMP2113 / ENGG1340              ║\n";
    cout << "  ║            Group 44                      ║\n";
    cout << "  ║                                          ║\n";
    cout << "  ╚══════════════════════════════════════════╝\n";
    cout << COLOR_RESET << "\n";
    cout << "  Press any key to continue...\n";
    _getch();
}

// Display main menu
void Game::displayMainMenu() {
    clearScreen();
    cout << COLOR_CYAN << COLOR_BOLD;
    cout << "\n  ═══════════════ MAIN MENU ═══════════════\n\n";
    cout << COLOR_RESET;
    
    if (user_logged_in) {
        cout << "  Welcome back, " << COLOR_YELLOW << current_user.username << COLOR_RESET << "!\n\n";
    }
    
    cout << "  [1] New Game\n";
    cout << "  [2] Continue (from saved progress)\n";
    cout << "  [3] View Controls\n";
    cout << "  [4] View Progress\n";
    cout << "  [5] Toggle Colors (currently: " 
         << (game_map.isColorEnabled() ? "ON" : "OFF") << ")\n";
    cout << "  [Q] Quit\n\n";
    cout << "  Enter your choice: ";
}

// Display difficulty menu
void Game::displayDifficultyMenu() {
    clearScreen();
    cout << COLOR_CYAN << COLOR_BOLD;
    cout << "\n  ═══════════ SELECT DIFFICULTY ═══════════\n\n";
    cout << COLOR_RESET;
    
    cout << "  [1] " << COLOR_GREEN << "Easy" << COLOR_RESET 
         << "   - 3 boxes, no obstacles\n";
    cout << "  [2] " << COLOR_YELLOW << "Medium" << COLOR_RESET 
         << " - 5 boxes, 3-5 obstacles\n";
    cout << "  [3] " << COLOR_RED << "Hard" << COLOR_RESET 
         << "   - 7 boxes, 6-10 obstacles\n";
    cout << "  [B] Back to menu\n\n";
    cout << "  Enter your choice: ";
}

// Display controls
void Game::displayControls() {
    clearScreen();
    cout << COLOR_CYAN << COLOR_BOLD;
    cout << "\n  ═══════════════ CONTROLS ═══════════════\n\n";
    cout << COLOR_RESET;
    
    cout << "  " << COLOR_BOLD << "Movement:" << COLOR_RESET << "\n";
    cout << "    W - Move Up\n";
    cout << "    A - Move Left\n";
    cout << "    S - Move Down\n";
    cout << "    D - Move Right\n\n";
    
    cout << "  " << COLOR_BOLD << "Actions:" << COLOR_RESET << "\n";
    cout << "    R - Restart Level\n";
    cout << "    U - Undo Last Move\n";
    cout << "    H - Show Help\n";
    cout << "    Q - Quit to Menu\n\n";
    
    cout << "  " << COLOR_BOLD << "Symbols:" << COLOR_RESET << "\n";
    cout << "    " << COLOR_YELLOW << "@" << COLOR_RESET << " - Player\n";
    cout << "    " << COLOR_RED << "$" << COLOR_RESET << " - Box\n";
    cout << "    " << COLOR_GREEN << "^" << COLOR_RESET << " - Target\n";
    cout << "    " << COLOR_BRIGHT_GREEN << "*" << COLOR_RESET << " - Box on Target\n";
    cout << "    " << COLOR_GRAY << "#" << COLOR_RESET << " - Wall\n";
    cout << "    " << COLOR_DARK_GRAY << "." << COLOR_RESET << " - Obstacle\n\n";
    
    cout << "  " << COLOR_BOLD << "Goal:" << COLOR_RESET << "\n";
    cout << "    Push all boxes ($) onto targets (^)\n";
    cout << "    When a box is on a target, it shows as (*)\n\n";
    
    cout << "  Press any key to continue...\n";
    _getch();
}

// Display game status during play
void Game::displayGameStatus() {
    int completed = countCompletedTargets();
    int total = game_map.getTargetCount();
    
    cout << COLOR_CYAN;
    cout << "  ╔════════════════════════════════════════╗\n";
    cout << "  ║  ";
    
    // Difficulty
    cout << "Difficulty: ";
    switch (current_difficulty) {
        case EASY:   cout << COLOR_GREEN << "Easy  " << COLOR_CYAN; break;
        case MEDIUM: cout << COLOR_YELLOW << "Medium" << COLOR_CYAN; break;
        case HARD:   cout << COLOR_RED << "Hard  " << COLOR_CYAN; break;
    }
    
    cout << " │ Steps: " << COLOR_YELLOW << player.getSteps() << COLOR_CYAN;
    cout << " │ " << completed << "/" << total;
    cout << "  ║\n";
    
    cout << "  ╚════════════════════════════════════════╝\n";
    cout << COLOR_RESET;
}

// Display user progress
void Game::displayUserProgress() {
    clearScreen();
    cout << COLOR_CYAN << COLOR_BOLD;
    cout << "\n  ═══════════════ PROGRESS ═══════════════\n\n";
    cout << COLOR_RESET;
    
    if (!user_logged_in) {
        cout << "  No user logged in.\n\n";
    } else {
        cout << "  User: " << COLOR_YELLOW << current_user.username << COLOR_RESET << "\n\n";
        cout << "  Highest Level: ";
        switch (current_user.highest_level) {
            case 1: cout << COLOR_GREEN << "Easy" << COLOR_RESET; break;
            case 2: cout << COLOR_YELLOW << "Medium" << COLOR_RESET; break;
            case 3: cout << COLOR_RED << "Hard" << COLOR_RESET; break;
        }
        cout << "\n\n";
        
        cout << "  Best Steps:\n";
        cout << "    Easy:   " << (current_user.best_steps_easy > 0 ? 
                                   to_string(current_user.best_steps_easy) : "Not completed") << "\n";
        cout << "    Medium: " << (current_user.best_steps_medium > 0 ? 
                                   to_string(current_user.best_steps_medium) : "Not completed") << "\n";
        cout << "    Hard:   " << (current_user.best_steps_hard > 0 ? 
                                   to_string(current_user.best_steps_hard) : "Not completed") << "\n";
    }
    
    cout << "\n  Press any key to continue...\n";
    _getch();
}

// Display win screen
void Game::displayWinScreen() {
    clearScreen();
    cout << COLOR_BRIGHT_GREEN << COLOR_BOLD;
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════╗\n";
    cout << "  ║                                          ║\n";
    cout << "  ║      🎉 CONGRATULATIONS! 🎉              ║\n";
    cout << "  ║                                          ║\n";
    cout << "  ║         Level Complete!                  ║\n";
    cout << "  ║                                          ║\n";
    cout << "  ╚══════════════════════════════════════════╝\n";
    cout << COLOR_RESET << "\n";
    
    cout << "  Steps taken: " << COLOR_YELLOW << player.getSteps() << COLOR_RESET << "\n\n";
    
    // Check for best score
    int* best_steps = nullptr;
    switch (current_difficulty) {
        case EASY:   best_steps = &current_user.best_steps_easy; break;
        case MEDIUM: best_steps = &current_user.best_steps_medium; break;
        case HARD:   best_steps = &current_user.best_steps_hard; break;
    }
    
    if (best_steps && (*best_steps == 0 || player.getSteps() < *best_steps)) {
        cout << "  " << COLOR_BRIGHT_GREEN << "NEW BEST SCORE!" << COLOR_RESET << "\n\n";
    }
    
    cout << "  [N] Next Level\n";
    cout << "  [R] Replay Same Level\n";
    cout << "  [M] Return to Menu\n\n";
    cout << "  Enter your choice: ";
}

// Display help during game
void Game::displayHelp() {
    cout << "\n";
    cout << COLOR_CYAN;
    cout << "  ═══════════════ QUICK HELP ═══════════════\n";
    cout << COLOR_RESET;
    cout << "  W/A/S/D = Move | R = Restart | U = Undo\n";
    cout << "  Q = Quit to Menu | H = This help\n";
    cout << "  Goal: Push all " << COLOR_RED << "$" << COLOR_RESET 
         << " onto " << COLOR_GREEN << "^" << COLOR_RESET << "\n";
    cout << COLOR_CYAN;
    cout << "  ═══════════════════════════════════════════\n";
    cout << COLOR_RESET;
}

// Login user
void Game::loginUser() {
    clearScreen();
    cout << COLOR_CYAN << COLOR_BOLD;
    cout << "\n  ═══════════════ LOGIN ═══════════════\n\n";
    cout << COLOR_RESET;
    
    cout << "  Enter your username: ";
    string username;
    cin >> username;
    cin.ignore();
    
    if (loadUserData(username, current_user)) {
        cout << "\n  Welcome back, " << COLOR_YELLOW << username << COLOR_RESET << "!\n";
        cout << "  Your progress has been loaded.\n";
    } else {
        current_user = UserData(username);
        saveUserData(current_user);
        cout << "\n  New user created: " << COLOR_YELLOW << username << COLOR_RESET << "\n";
        cout << "  Starting from Easy difficulty.\n";
    }
    
    user_logged_in = true;
    cout << "\n  Press any key to continue...\n";
    _getch();
}

// Select difficulty
void Game::selectDifficulty() {
    while (true) {
        displayDifficultyMenu();
        char choice = _getch();
        
        switch (choice) {
            case '1':
                current_difficulty = EASY;
                return;
            case '2':
                current_difficulty = MEDIUM;
                return;
            case '3':
                current_difficulty = HARD;
                return;
            case 'b':
            case 'B':
                return;
        }
    }
}

// Start new game
void Game::startNewGame() {
    selectDifficulty();
    
    // Generate random map
    game_map.generateRandomMap(current_difficulty);
    
    // Initialize player
    player.setPosition(game_map.getPlayerStartX(), game_map.getPlayerStartY());
    player.resetSteps();
    
    // Clear undo history
    undo_system.clear();
    
    level_complete = false;
    game_running = true;
}

// Continue from saved progress
void Game::continueGame() {
    if (!user_logged_in) {
        cout << "\n  Please login first!\n";
        cout << "  Press any key to continue...\n";
        _getch();
        return;
    }
    
    // Set difficulty based on highest level
    current_difficulty = static_cast<Difficulty>(current_user.highest_level);
    
    // Generate map for that difficulty
    game_map.generateRandomMap(current_difficulty);
    
    // Initialize player
    player.setPosition(game_map.getPlayerStartX(), game_map.getPlayerStartY());
    player.resetSteps();
    
    // Clear undo history
    undo_system.clear();
    
    level_complete = false;
    game_running = true;
}

// Process input during game
void Game::processInput(char input) {
    input = tolower(input);
    
    switch (input) {
        case 'w':
            movePlayer(player, game_map, UP, &undo_system);
            break;
        case 'a':
            movePlayer(player, game_map, LEFT, &undo_system);
            break;
        case 's':
            movePlayer(player, game_map, DOWN, &undo_system);
            break;
        case 'd':
            movePlayer(player, game_map, RIGHT, &undo_system);
            break;
        case 'r':
            restartLevel();
            break;
        case 'u':
            if (undo_system.canUndo()) {
                undo_system.undo(game_map, player);
            }
            break;
        case 'h':
            displayHelp();
            cout << "  Press any key to continue...\n";
            _getch();
            break;
        case 'q':
        case 27:  // ESC
            game_running = false;
            break;
    }
}

// Restart level
void Game::restartLevel() {
    game_map.restoreInitialState();
    player.setPosition(game_map.getPlayerStartX(), game_map.getPlayerStartY());
    player.resetSteps();
    undo_system.clear();
}

// Go to next level
void Game::nextLevel() {
    // Increase difficulty if possible
    if (current_difficulty < HARD) {
        current_difficulty = static_cast<Difficulty>(static_cast<int>(current_difficulty) + 1);
    }
    
    // Update user progress
    if (static_cast<int>(current_difficulty) > current_user.highest_level) {
        current_user.highest_level = static_cast<int>(current_difficulty);
    }
    
    // Generate new map
    game_map.generateRandomMap(current_difficulty);
    player.setPosition(game_map.getPlayerStartX(), game_map.getPlayerStartY());
    player.resetSteps();
    undo_system.clear();
    
    level_complete = false;
}

// Check win condition
bool Game::checkWinCondition() {
    return countCompletedTargets() == game_map.getTargetCount();
}

// Count completed targets
int Game::countCompletedTargets() {
    int count = 0;
    for (int y = 0; y < game_map.getHeight(); y++) {
        for (int x = 0; x < game_map.getWidth(); x++) {
            if (game_map.getCell(x, y) == BOX_ON_TARGET) {
                count++;
            }
        }
    }
    return count;
}

// Save progress
void Game::saveProgress() {
    if (!user_logged_in) return;
    
    // Update best steps if applicable
    int steps = player.getSteps();
    if (level_complete && steps > 0) {
        switch (current_difficulty) {
            case EASY:
                if (current_user.best_steps_easy == 0 || steps < current_user.best_steps_easy) {
                    current_user.best_steps_easy = steps;
                }
                break;
            case MEDIUM:
                if (current_user.best_steps_medium == 0 || steps < current_user.best_steps_medium) {
                    current_user.best_steps_medium = steps;
                }
                break;
            case HARD:
                if (current_user.best_steps_hard == 0 || steps < current_user.best_steps_hard) {
                    current_user.best_steps_hard = steps;
                }
                break;
        }
    }
    
    // Update highest level
    if (static_cast<int>(current_difficulty) > current_user.highest_level) {
        current_user.highest_level = static_cast<int>(current_difficulty);
    }
    
    saveUserData(current_user);
}

// Main game loop
void Game::run() {
    displayWelcome();
    loginUser();
    
    bool quit = false;
    
    while (!quit) {
        displayMainMenu();
        char choice = _getch();
        
        switch (tolower(choice)) {
            case '1':
                startNewGame();
                break;
            case '2':
                continueGame();
                break;
            case '3':
                displayControls();
                continue;
            case '4':
                displayUserProgress();
                continue;
            case '5':
                game_map.setColorEnabled(!game_map.isColorEnabled());
                continue;
            case 'q':
                quit = true;
                continue;
            default:
                continue;
        }
        
        // Game play loop
        while (game_running) {
            clearScreen();
            displayGameStatus();
            game_map.printMap();
            
            cout << "  [W/A/S/D] Move | [R] Restart | [U] Undo | [H] Help | [Q] Quit\n";
            
            char input = _getch();
            processInput(input);
            
            // Check win
            if (checkWinCondition()) {
                level_complete = true;
                saveProgress();
                
                // Win screen
                while (true) {
                    displayWinScreen();
                    char win_choice = _getch();
                    win_choice = tolower(win_choice);
                    
                    if (win_choice == 'n') {
                        nextLevel();
                        break;
                    } else if (win_choice == 'r') {
                        restartLevel();
                        level_complete = false;
                        break;
                    } else if (win_choice == 'm') {
                        game_running = false;
                        break;
                    }
                }
            }
        }
        
        // Save on exit from game
        saveProgress();
    }
    
    // Final save
    saveProgress();
    
    clearScreen();
    cout << COLOR_CYAN << COLOR_BOLD;
    cout << "\n  ═══════════════════════════════════════════\n";
    cout << "       Thanks for playing Block Tactics!\n";
    cout << "  ═══════════════════════════════════════════\n";
    cout << COLOR_RESET << "\n";
}
