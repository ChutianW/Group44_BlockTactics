#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "map.h"
#include "player.h"
#include "file_io.h"

// ============================================================
// 清屏函数（兼容 Linux 和 Windows）
// ============================================================
void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// ============================================================
// 显示游戏欢迎界面
// ============================================================
void showWelcome() {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════════╗\n";
    std::cout << "  ║                                          ║\n";
    std::cout << "  ║         BLOCK TACTICS                    ║\n";
    std::cout << "  ║                                          ║\n";
    std::cout << "  ║      A Sokoban-Style Puzzle Game         ║\n";
    std::cout << "  ║                                          ║\n";
    std::cout << "  ║         COMP2113 / ENGG1340              ║\n";
    std::cout << "  ║            Group 44                      ║\n";
    std::cout << "  ║                                          ║\n";
    std::cout << "  ╚══════════════════════════════════════════╝\n";
    std::cout << COLOR_RESET << "\n";
    std::cout << "  Press any key to continue...\n";
    getch();
}

// ============================================================
// 用户登录：输入用户名，加载已有数据或创建新用户
// ============================================================
std::string loginUser(UserData &user) {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  ═══════════════ LOGIN ═══════════════\n\n";
    std::cout << COLOR_RESET;

    std::cout << "  Enter your username: ";
    std::string username;
    std::cin >> username;
    std::cin.ignore();

    if (loadUserData(username, user)) {
        std::cout << "\n  Welcome back, " << COLOR_YELLOW << username << COLOR_RESET << "!\n";
        std::cout << "  Your progress has been loaded.\n";
    } else {
        user = createNewUser(username);
        saveUserData(user);
        std::cout << "\n  New user created: " << COLOR_YELLOW << username << COLOR_RESET << "\n";
        std::cout << "  Starting from Easy difficulty.\n";
    }

    std::cout << "\n  Press any key to continue...\n";
    getch();
    return username;
}

// ============================================================
// 显示主菜单
// ============================================================
void showMenu() {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  ═══════════════ MAIN MENU ═══════════════\n\n";
    std::cout << COLOR_RESET;
    std::cout << "  [1] New Game\n";
    std::cout << "  [2] Continue (from saved progress)\n";
    std::cout << "  [3] View Controls\n";
    std::cout << "  [4] View Progress\n";
    std::cout << "  [5] Toggle Colors\n";
    std::cout << "  [Q] Quit\n\n";
    std::cout << "  Enter your choice: ";
}

// ============================================================
// 显示难度选择菜单
// ============================================================
void showDifficultyMenu() {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  ═══════════ SELECT DIFFICULTY ═══════════\n\n";
    std::cout << COLOR_RESET;

    std::cout << "  [1] " << COLOR_GREEN << "Easy" << COLOR_RESET
              << "   - 3 boxes, no obstacles\n";
    std::cout << "  [2] " << COLOR_YELLOW << "Medium" << COLOR_RESET
              << " - 5 boxes, 3-5 obstacles\n";
    std::cout << "  [3] " << COLOR_RED << "Hard" << COLOR_RESET
              << "   - 7 boxes, 6-10 obstacles\n";
    std::cout << "  [B] Back to menu\n\n";
    std::cout << "  Enter your choice: ";
}

// ============================================================
// 显示操作说明
// ============================================================
void showControls() {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  ═══════════════ CONTROLS ═══════════════\n\n";
    std::cout << COLOR_RESET;

    std::cout << "  " << COLOR_BOLD << "Movement:" << COLOR_RESET << "\n";
    std::cout << "    W - Move Up\n";
    std::cout << "    A - Move Left\n";
    std::cout << "    S - Move Down\n";
    std::cout << "    D - Move Right\n\n";

    std::cout << "  " << COLOR_BOLD << "Actions:" << COLOR_RESET << "\n";
    std::cout << "    R - Restart Level\n";
    std::cout << "    U - Undo Last Move\n";
    std::cout << "    H - Show Help\n";
    std::cout << "    Q - Quit to Menu\n\n";

    std::cout << "  " << COLOR_BOLD << "Symbols:" << COLOR_RESET << "\n";
    std::cout << "    " << COLOR_YELLOW << "@" << COLOR_RESET << " - Player\n";
    std::cout << "    " << COLOR_RED << "$" << COLOR_RESET << " - Box\n";
    std::cout << "    " << COLOR_GREEN << "^" << COLOR_RESET << " - Target\n";
    std::cout << "    " << COLOR_BRIGHT_GREEN << "*" << COLOR_RESET << " - Box on Target\n";
    std::cout << "    " << COLOR_GRAY << "#" << COLOR_RESET << " - Wall\n";
    std::cout << "    " << COLOR_DARK_GRAY << "." << COLOR_RESET << " - Obstacle\n\n";

    std::cout << "  " << COLOR_BOLD << "Goal:" << COLOR_RESET << "\n";
    std::cout << "    Push all boxes ($) onto targets (^)\n";
    std::cout << "    When a box is on a target, it shows as (*)\n\n";

    std::cout << "  Press any key to continue...\n";
    getch();
}

// ============================================================
// 显示游戏中状态栏（难度、步数、完成进度）
// ============================================================
void showGameStatus(Difficulty diff, int steps, int completed, int total) {
    std::cout << COLOR_CYAN;
    std::cout << "  ╔════════════════════════════════════════╗\n";
    std::cout << "  ║  ";

    // 难度名称
    std::cout << "Difficulty: ";
    switch (diff) {
        case EASY:   std::cout << COLOR_GREEN << "Easy  " << COLOR_CYAN; break;
        case MEDIUM: std::cout << COLOR_YELLOW << "Medium" << COLOR_CYAN; break;
        case HARD:   std::cout << COLOR_RED << "Hard  " << COLOR_CYAN; break;
    }

    std::cout << " | Steps: " << COLOR_YELLOW << steps << COLOR_CYAN;
    std::cout << " | " << completed << "/" << total;
    std::cout << "  ║\n";
    std::cout << "  ╚════════════════════════════════════════╝\n";
    std::cout << COLOR_RESET;
}

// ============================================================
// 显示用户进度信息
// ============================================================
void showUserProgress(const UserData &user, bool logged_in) {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  ═══════════════ PROGRESS ═══════════════\n\n";
    std::cout << COLOR_RESET;

    if (!logged_in) {
        std::cout << "  No user logged in.\n\n";
    } else {
        std::cout << "  User: " << COLOR_YELLOW << user.username << COLOR_RESET << "\n\n";
        std::cout << "  Highest Level: ";
        switch (user.highest_level) {
            case 1: std::cout << COLOR_GREEN << "Easy" << COLOR_RESET; break;
            case 2: std::cout << COLOR_YELLOW << "Medium" << COLOR_RESET; break;
            case 3: std::cout << COLOR_RED << "Hard" << COLOR_RESET; break;
        }
        std::cout << "\n\n";

        std::cout << "  Best Steps:\n";
        std::cout << "    Easy:   " << (user.best_steps_easy > 0 ?
                                   std::to_string(user.best_steps_easy) : "Not completed") << "\n";
        std::cout << "    Medium: " << (user.best_steps_medium > 0 ?
                                   std::to_string(user.best_steps_medium) : "Not completed") << "\n";
        std::cout << "    Hard:   " << (user.best_steps_hard > 0 ?
                                   std::to_string(user.best_steps_hard) : "Not completed") << "\n";
    }

    std::cout << "\n  Press any key to continue...\n";
    getch();
}

// ============================================================
// 显示胜利界面和后续选择
// ============================================================
void showWinScreen(int steps, const UserData &user, Difficulty diff) {
    clearScreen();
    std::cout << COLOR_BRIGHT_GREEN << COLOR_BOLD;
    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════════╗\n";
    std::cout << "  ║                                          ║\n";
    std::cout << "  ║      CONGRATULATIONS!                    ║\n";
    std::cout << "  ║                                          ║\n";
    std::cout << "  ║         Level Complete!                  ║\n";
    std::cout << "  ║                                          ║\n";
    std::cout << "  ╚══════════════════════════════════════════╝\n";
    std::cout << COLOR_RESET << "\n";

    std::cout << "  Steps taken: " << COLOR_YELLOW << steps << COLOR_RESET << "\n\n";

    // 检查是否是新最佳纪录
    int current_best = 0;
    switch (diff) {
        case EASY:   current_best = user.best_steps_easy; break;
        case MEDIUM: current_best = user.best_steps_medium; break;
        case HARD:   current_best = user.best_steps_hard; break;
    }

    if (current_best == 0 || steps < current_best) {
        std::cout << "  " << COLOR_BRIGHT_GREEN << "NEW BEST SCORE!" << COLOR_RESET << "\n\n";
    }

    std::cout << "  [N] Next Level\n";
    std::cout << "  [R] Replay Same Level\n";
    std::cout << "  [M] Return to Menu\n\n";
    std::cout << "  Enter your choice: ";
}

// ============================================================
// 显示游戏内帮助提示
// ============================================================
void showHelp() {
    std::cout << "\n";
    std::cout << COLOR_CYAN;
    std::cout << "  ═══════════════ QUICK HELP ═══════════════\n";
    std::cout << COLOR_RESET;
    std::cout << "  W/A/S/D = Move | R = Restart | U = Undo\n";
    std::cout << "  Q = Quit to Menu | H = This help\n";
    std::cout << "  Goal: Push all " << COLOR_RED << "$" << COLOR_RESET
              << " onto " << COLOR_GREEN << "^" << COLOR_RESET << "\n";
    std::cout << COLOR_CYAN;
    std::cout << "  ═══════════════════════════════════════════\n";
    std::cout << COLOR_RESET;
}

// ============================================================
// 检查胜利条件：所有目标点上都有箱子
// ============================================================
bool checkWinCondition(const std::vector<std::vector<char>> &grid,
                       const std::vector<std::pair<int, int>> &target_positions) {
    return countCompletedTargets(grid, target_positions) == getTargetCount(target_positions);
}

// ============================================================
// 保存用户进度（更新最佳步数和最高难度）
// ============================================================
void saveProgress(UserData &user, Difficulty diff, int steps, bool level_complete) {
    if (level_complete && steps > 0) {
        switch (diff) {
            case EASY:
                if (user.best_steps_easy == 0 || steps < user.best_steps_easy) {
                    user.best_steps_easy = steps;
                }
                break;
            case MEDIUM:
                if (user.best_steps_medium == 0 || steps < user.best_steps_medium) {
                    user.best_steps_medium = steps;
                }
                break;
            case HARD:
                if (user.best_steps_hard == 0 || steps < user.best_steps_hard) {
                    user.best_steps_hard = steps;
                }
                break;
        }
    }

    // 更新最高难度
    if (static_cast<int>(diff) > user.highest_level) {
        user.highest_level = static_cast<int>(diff);
    }

    saveUserData(user);
}

// ============================================================
// 开始新游戏：选择难度，生成地图，初始化玩家
// ============================================================
void startNewGame(Difficulty &diff,
                  std::vector<std::vector<char>> &grid,
                  std::vector<std::pair<int, int>> &target_positions,
                  Player &player,
                  UndoSystem &undo) {
    // 选择难度
    while (true) {
        showDifficultyMenu();
        char choice = getch();
        choice = std::tolower(choice);

        switch (choice) {
            case '1': diff = EASY; break;
            case '2': diff = MEDIUM; break;
            case '3': diff = HARD; break;
            case 'b': return;  // 返回主菜单
        }
        if (choice == '1' || choice == '2' || choice == '3') break;
    }

    // 生成随机地图
    int start_row = 1, start_col = 1;
    generateRandomMap(grid, start_row, start_col, target_positions, diff);

    // 初始化玩家
    player.setPosition(start_row, start_col);
    player.resetSteps();

    // 清空撤销历史
    undo.clear();
}

// ============================================================
// 重新开始当前关卡
// ============================================================
void restartLevel(std::vector<std::vector<char>> &grid,
                  std::vector<std::pair<int, int>> &target_positions,
                  Player &player,
                  const GameState &initial_state) {
    int row = player.row;
    int col = player.col;
    restoreInitialState(grid, target_positions, row, col, player.steps, initial_state);
    player.setPosition(row, col);
}

// ============================================================
// 进入下一关（提升难度）
// ============================================================
void nextLevel(Difficulty &diff,
               std::vector<std::vector<char>> &grid,
               std::vector<std::pair<int, int>> &target_positions,
               Player &player,
               UndoSystem &undo) {
    // 提升难度
    if (diff < HARD) {
        diff = static_cast<Difficulty>(static_cast<int>(diff) + 1);
    }

    int start_row = 1, start_col = 1;
    generateRandomMap(grid, start_row, start_col, target_positions, diff);
    player.setPosition(start_row, start_col);
    player.resetSteps();
    undo.clear();
}

// ============================================================
// 处理游戏中的按键输入
// ============================================================
void processInput(char input,
                  Player &player,
                  std::vector<std::vector<char>> &grid,
                  std::vector<std::pair<int, int>> &target_positions,
                  UndoSystem &undo,
                  const GameState &initial_state,
                  bool &game_running) {
    input = std::tolower(input);

    switch (input) {
        case 'w':
            movePlayer(player, DIR_UP, grid, target_positions, &undo);
            break;
        case 'a':
            movePlayer(player, DIR_LEFT, grid, target_positions, &undo);
            break;
        case 's':
            movePlayer(player, DIR_DOWN, grid, target_positions, &undo);
            break;
        case 'd':
            movePlayer(player, DIR_RIGHT, grid, target_positions, &undo);
            break;
        case 'r':
            restartLevel(grid, target_positions, player, initial_state);
            break;
        case 'u':
            if (undo.canUndo()) {
                undo.undo(grid, player);
            }
            break;
        case 'h':
            showHelp();
            std::cout << "  Press any key to continue...\n";
            getch();
            break;
        case 'q':
        case 27:  // ESC
            game_running = false;
            break;
    }
}

// ============================================================
// 主游戏循环
//
// 流程：
//   while (游戏运行中) {
//     1. 清屏
//     2. 显示状态栏（难度、步数、完成进度）
//     3. 打印地图
//     4. 显示操作提示
//     5. 读取输入
//     6. 处理输入（移动/重启/撤销/帮助/退出）
//     7. 检查胜利条件 → 胜利则进入胜利界面
//   }
// ============================================================
void runGameLoop(std::vector<std::vector<char>> grid,
                 std::vector<std::pair<int, int>> target_positions,
                 Player &player,
                 Difficulty diff,
                 UndoSystem &undo,
                 GameState &initial_state,
                 UserData &user) {
    bool game_running = true;
    bool level_complete = false;

    while (game_running) {
        // 1. 清屏
        clearScreen();

        // 2. 显示状态栏
        int completed = countCompletedTargets(grid, target_positions);
        int total = getTargetCount(target_positions);
        showGameStatus(diff, player.getSteps(), completed, total);

        // 3. 打印地图
        printMap(grid, target_positions, player.row, player.col, true);

        // 4. 操作提示
        std::cout << "  [W/A/S/D] Move | [R] Restart | [U] Undo | [H] Help | [Q] Quit\n";

        // 5. 读取输入
        char input = getch();

        // 6. 处理输入
        processInput(input, player, grid, target_positions, undo,
                     initial_state, game_running);

        // 7. 检查胜利条件
        if (!level_complete && checkWinCondition(grid, target_positions)) {
            level_complete = true;
            saveProgress(user, diff, player.getSteps(), true);

            // 胜利界面
            while (true) {
                showWinScreen(player.getSteps(), user, diff);
                char win_choice = getch();
                win_choice = std::tolower(win_choice);

                if (win_choice == 'n') {
                    // 下一关
                    nextLevel(diff, grid, target_positions, player, undo);
                    initial_state = saveInitialState(grid, target_positions, player.row, player.col);
                    level_complete = false;
                    break;
                } else if (win_choice == 'r') {
                    // 重玩
                    restartLevel(grid, target_positions, player, initial_state);
                    level_complete = false;
                    break;
                } else if (win_choice == 'm') {
                    // 返回主菜单
                    game_running = false;
                    break;
                }
            }
        }
    }
}

// ============================================================
// 主函数 —— 游戏入口
//
// 整体流程：
//   1. 显示欢迎界面
//   2. 用户登录
//   3. 主菜单循环：
//      - 新游戏 → 选难度 → 生成地图 → 进入游戏循环
//      - 继续 → 加载难度 → 生成地图 → 进入游戏循环
//      - 操作说明 → 显示后返回菜单
//      - 查看进度 → 显示后返回菜单
//      - 切换颜色 → 切换后返回菜单
//      - 退出 → 保存进度 → 退出
// ============================================================
int main() {
    // 初始化随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));

    // 显示欢迎界面
    showWelcome();

    // 用户登录
    UserData user;
    loginUser(user);

    bool quit = false;
    Difficulty diff = EASY;

    while (!quit) {
        showMenu();
        char choice = getch();
        choice = std::tolower(choice);

        switch (choice) {
            case '1': {
                // 新游戏
                std::vector<std::vector<char>> grid;
                std::vector<std::pair<int, int>> target_positions;
                Player player;
                UndoSystem undo(100);
                GameState initial_state;

                startNewGame(diff, grid, target_positions, player, undo);
                if (grid.empty()) continue;  // 用户返回了

                initial_state = saveInitialState(grid, target_positions, player.row, player.col);
                runGameLoop(grid, target_positions, player, diff, undo, initial_state, user);
                saveProgress(user, diff, player.getSteps(), false);
                break;
            }
            case '2': {
                // 继续游戏
                Difficulty saved_diff = static_cast<Difficulty>(user.highest_level);

                std::vector<std::vector<char>> grid;
                std::vector<std::pair<int, int>> target_positions;
                Player player;
                UndoSystem undo(100);
                GameState initial_state;

                int start_row = 1, start_col = 1;
                generateRandomMap(grid, start_row, start_col, target_positions, saved_diff);
                player.setPosition(start_row, start_col);
                player.resetSteps();
                undo.clear();

                initial_state = saveInitialState(grid, target_positions, player.row, player.col);
                runGameLoop(grid, target_positions, player, saved_diff, undo, initial_state, user);
                saveProgress(user, saved_diff, player.getSteps(), false);
                break;
            }
            case '3':
                // 操作说明
                showControls();
                break;
            case '4':
                // 查看进度
                showUserProgress(user, true);
                break;
            case 'q':
            case 27:  // ESC
                quit = true;
                break;
            default:
                break;
        }
    }

    // 最终保存
    saveUserData(user);

    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  ═══════════════════════════════════════════\n";
    std::cout << "       Thanks for playing Block Tactics!\n";
    std::cout << "  ═══════════════════════════════════════════\n";
    std::cout << COLOR_RESET << "\n";

    return 0;
}
