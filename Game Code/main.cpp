#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "map.h"
#include "player.h"
#include "file_io.h"
#include "renderer.h"
#include "game.h"
#include "terminal.h"

// ===============================================================================
// Windows UTF-8 initialization
// ===============================================================================
void initWindowsUTF8() {
    #if defined(_WIN32) || defined(WIN32)
        system("chcp 65001 > nul");
    #endif
}

// ===============================================================================
// Main - Game entry point (delegates to Game and Renderer classes)
// OOP: game logic in Game, display in Renderer
// ===============================================================================
int main() {
    // Initialize Windows console to UTF-8
    initWindowsUTF8();

    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));

    // Terminal capability detection (conservative on Windows)
    TerminalConfig term_cfg = detectTerminalConfig();

    // Create renderer with safe defaults
    Renderer renderer(term_cfg.color_enabled_by_default,
                      term_cfg.default_render_mode,
                      term_cfg.ansi_color_supported);

    // Show welcome screen
    renderer.printWelcome();

    // 用户登录
    UserData user;
    std::string username;
    {
        // Login logic - use ASCII on non-Unicode terminals
        std::string login_border;
        if (renderer.getRenderMode() == RenderMode::UNICODE) {
            login_border = "  ═══════════════ LOGIN ═══════════════";
        } else {
            login_border = "  =============== LOGIN ===============";
        }

        std::cout << "\n  " << login_border << "\n\n";
        std::cout << "  Enter your username: ";
        std::cin >> username;
        std::cin.ignore();

        if (loadUserData(username, user)) {
            std::cout << "\n  Welcome back, " << COLOR_YELLOW << username << COLOR_RESET << "!\n";
            std::cout << "  Your progress has been loaded.\n";
        } else {
            user = createNewUser(username);
            saveUserData(user);
            std::cout << "\n  New user created: " << COLOR_YELLOW << username << COLOR_RESET << "!\n";
            std::cout << "  Starting from Easy difficulty.\n";
        }
        std::cout << "\n  Press any key to continue...\n";
        renderer.getInput();
    }

    bool quit = false;
    Difficulty diff = EASY;

    // Main menu loop
    while (!quit) {
        renderer.printMenu();
        char choice = renderer.getInput();
        choice = std::tolower(choice);

        switch (choice) {
            case '1': {
                // New game: select difficulty
                renderer.printDifficultyMenu();
                char diff_choice = renderer.getInput();
                diff_choice = std::tolower(diff_choice);

                switch (diff_choice) {
                    case '1': diff = EASY; break;
                    case '2': diff = MEDIUM; break;
                    case '3': diff = HARD; break;
                    case 'b': continue;  // Return to menu
                }

                // Create game object (polymorphism: different class per difficulty)
                Game *game = nullptr;
                switch (diff) {
                    case EASY:
                        game = new EasyGame(&renderer);
                        break;
                    case MEDIUM:
                        game = new MediumGame(&renderer);
                        break;
                    case HARD:
                        game = new HardGame(&renderer);
                        break;
                }

                game->setUserData(&user);
                game->startGame();
                game->run();

                // Save progress after game
                if (game->getDifficulty() != diff) {
                    // 难度可能升级了
                    diff = game->getDifficulty();
                }
                delete game;
                break;
            }
            case '2': {
                // Continue saved game
                Difficulty saved_diff = static_cast<Difficulty>(user.highest_level);

                Game *game = nullptr;
                switch (saved_diff) {
                    case EASY:
                        game = new EasyGame(&renderer);
                        break;
                    case MEDIUM:
                        game = new MediumGame(&renderer);
                        break;
                    case HARD:
                        game = new HardGame(&renderer);
                        break;
                }

                game->setUserData(&user);
                game->startGame();
                game->run();
                delete game;
                break;
            }
            case '3':
                // View controls
                renderer.printControls();
                break;
            case '4':
                // View progress
                renderer.printUserProgress(user, true);
                break;
            case '5':
                // Toggle colors
                renderer.setColorEnabled(!renderer.isColorEnabled());
                break;
            case 'l':
                // Leaderboard
                renderer.printLeaderboard(getLeaderboard());
                break;
            case 'q':
            case 27:  // ESC
                quit = true;
                break;
            default:
                break;
        }
    }

    // Final save
    saveUserData(user);

    renderer.clearScreen();

    // Goodbye screen - use ASCII on non-Unicode terminals
    std::string goodbye_border;
    if (renderer.getRenderMode() == RenderMode::UNICODE) {
        goodbye_border = "  ═══════════════════════════════════════════";
    } else {
        goodbye_border = "  ===========================================";
    }

    if (renderer.isColorEnabled()) {
        std::cout << COLOR_CYAN << COLOR_BOLD;
    }
    std::cout << "\n  " << goodbye_border << "\n";
    std::cout << "       Thanks for playing Block Tactics!\n";
    std::cout << "  " << goodbye_border << "\n";
    if (renderer.isColorEnabled()) {
        std::cout << COLOR_RESET;
    }
    std::cout << "\n";

    return 0;
}
