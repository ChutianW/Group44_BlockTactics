#include <iostream>
#include <string>
#include "map.h"
#include "player.h"
#include "file_io.h"

// 显示游戏欢迎界面
void showWelcome() {
    std::cout << "========================\n";
    std::cout << "    BLOCK TACTICS\n";
    std::cout << "========================\n\n";
}

// ============================================================
// 【下一步工作】优化用户名输入
// 当前：简单读取用户名
// 待做：检查该用户是否有历史进度，如果有则显示欢迎 + 进度信息
// 提示：调用 userExists() 检查，调用 loadUserData() 读取
// ============================================================
std::string getUsername() {
    std::string name;
    std::cout << "Enter your username: ";
    std::cin >> name;
    return name;
}

// 显示主菜单（已完成，无需修改）
void showMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "  1. New Game\n";
    std::cout << "  2. Continue\n";
    std::cout << "  3. Quit\n";
    std::cout << "Choice: ";
}

// 显示难度选择菜单（已完成，无需修改）
void showDifficultyMenu() {
    std::cout << "\nSelect Difficulty:\n";
    std::cout << "  1. Easy\n";
    std::cout << "  2. Medium\n";
    std::cout << "  3. Hard\n";
    std::cout << "Choice: ";
}

// ============================================================
// 【下一步工作】检查胜利条件（第一步：最小可行版本）
//
// 实现方法：
//   遍历整个地图 grid，统计：
//     - 目标点 '^' 的数量
//     - 箱子在目标点上 '*' 的数量（或检查箱子位置是否覆盖所有目标点）
//   如果所有目标点都被箱子覆盖 → 返回 true
//   否则 → 返回 false
// ============================================================
bool checkWinCondition(const std::vector<std::vector<char>> &grid) {
    // TODO: 遍历地图检查胜利条件
    // 方法一：统计 '*' 的数量是否等于目标点总数
    // 方法二：单独维护目标点位置列表，检查每个目标点上是否有箱子
    (void)grid;
    return false;
}

// ============================================================
// 【下一步工作】实现主游戏循环（第一步：最小可行版本）
//
// 函数签名建议改为：
//   void runGameLoop(std::vector<std::vector<char>> grid, Player &player)
// 或者传入初始状态和难度参数，在函数内部创建地图
//
// 循环流程：
//   while (true) {
//     1. 清屏（system("clear") 或打印多个换行）
//     2. 打印地图 printMap(grid)
//     3. 显示操作提示（WASD 移动, R 重新开始, Q 退出）
//     4. 读取输入 char c = getInput()
//     5. 解析输入 int dir = parseDirection(c)
//     6. 根据输入处理：
//        - dir == -1 → 无效输入，提示用户
//        - dir == -2 → 按了 R，重新开始关卡
//        - dir == -3 → 按了 Q，退出游戏
//        - 其他     → 调用 movePlayer(player, dir, grid)
//     7. 如果移动成功，步数 +1
//     8. 检查胜利条件 checkWinCondition(grid)
//        - 胜利 → 显示恭喜信息，break 退出循环
//   }
// ============================================================
void runGameLoop() {
    // TODO: 按上述流程实现游戏主循环
    // 注意：该函数需要接收 grid 和 player 作为参数
    std::cout << "\nGame loop running... (not yet implemented)\n";
}

// ============================================================
// 【下一步工作】完善 main 函数的主菜单逻辑
//
// case '1' 新游戏：
//   - 读取难度选择（1/2/3）
//   - 根据难度设置 MapConfig（箱子数、障碍物数）
//   - 调用 createFixedMap() 或 generateRandomMap() 创建地图
//   - 找到玩家起始位置，创建 Player 对象
//   - 保存初始状态 saveInitialState()
//   - 调用 runGameLoop(grid, player, initial_state)
//   - 游戏结束后保存用户进度
//
// case '2' 继续游戏：
//   - 检查用户是否有保存的进度
//   - 如果有：加载进度，恢复地图和玩家状态
//   - 如果没有：提示用户并回到菜单
//   - 调用 runGameLoop()
//
// case '3' 退出：
//   - 保存用户数据 saveUserData()
//   - 退出程序
// ============================================================
int main() {
    // 显示欢迎界面
    showWelcome();

    // 获取用户名，加载或创建用户数据
    std::string username = getUsername();
    UserData user = loadOrCreateUser(username, "user_data.txt");

    // TODO: 如果用户有历史进度，显示其进度信息
    // 示例：
    // if (user.highest_level > 1) {
    //     std::cout << "Welcome back! Highest level: " << user.highest_level << "\n";
    // }

    // 主菜单循环
    while (true) {
        showMenu();
        char choice;
        std::cin >> choice;

        switch (choice) {
            case '1':
                // 新游戏：选择难度后开始
                showDifficultyMenu();
                // TODO: 读取难度选择，创建地图和玩家，开始游戏
                runGameLoop();
                break;
            case '2':
                // 继续游戏：从已保存的进度恢复
                // TODO: 加载已保存的游戏状态
                runGameLoop();
                break;
            case '3':
                // 退出游戏
                std::cout << "Thanks for playing! Goodbye.\n";
                return 0;
            default:
                // 无效输入
                std::cout << "Invalid choice.\n";
                break;
        }
    }

    return 0;
}
