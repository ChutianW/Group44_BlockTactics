#include "map.h"
#include <iostream>

// ============================================================
// 【下一步工作】创建固定测试地图（最小可行版本第一步）
// 目标：手动设计一个 10x10 地图，包含以下元素：
//   - 四周墙壁 (#)
//   - 内部部分墙壁/障碍物
//   - 2~4 个箱子 ($)
//   - 2~4 个目标点 (^)
//   - 1 个玩家起始位置 (@)
// 提示：可以直接用 setCell 或直接操作 grid[row][col]
// ============================================================
std::vector<std::vector<char>> createFixedMap() {
    // TODO: 在这里手动布置地图元素
    // 示例参考（简单布局）：
    //   ######
    //   #  $ #
    //   # @^ #
    //   ######
    // 先用空白格子初始化 10x10 网格
    std::vector<std::vector<char>> grid(MAP_ROWS, std::vector<char>(MAP_COLS, SYMBOL_EMPTY));

    // ====== 在下面添加你的地图布置代码 ======
    // 示例：设置四面墙壁
    // for (int c = 0; c < MAP_COLS; ++c) {
    //     grid[0][c] = SYMBOL_WALL;         // 上边框
    //     grid[MAP_ROWS-1][c] = SYMBOL_WALL; // 下边框
    // }
    // for (int r = 0; r < MAP_ROWS; ++r) {
    //     grid[r][0] = SYMBOL_WALL;         // 左边框
    //     grid[r][MAP_COLS-1] = SYMBOL_WALL; // 右边框
    // }

    // 示例：放置一个箱子和目标点
    // grid[3][3] = SYMBOL_BOX;
    // grid[3][5] = SYMBOL_TARGET;

    return grid;
}

// ============================================================
// 【下一步工作】随机地图生成（第五步：随机地图 + 难度）
// 输入：MapConfig（箱子数量、障碍物数量）
// 生成流程：
//   (1) 用墙壁填满整个网格
//   (2) 在内部挖出空地
//   (3) 随机放置障碍物 (.)
//   (4) 随机放置目标点 (^)
//   (5) 随机放置箱子 ($)
//   (6) 放置玩家 (@)
// 约束检查：
//   - 箱子不能放在四个角落
//   - 目标点不能紧贴外墙
//   - 两个箱子不能相邻
//   - 箱子必须至少有一个方向可以移动
// 如果约束不满足：重新生成或调整该对象位置
// ============================================================
std::vector<std::vector<char>> generateRandomMap(MapConfig config) {
    // TODO: 实现随机地图生成
    // 需要包含 <cstdlib> <ctime> 使用 rand()/srand()
    std::vector<std::vector<char>> grid(MAP_ROWS, std::vector<char>(MAP_COLS, SYMBOL_EMPTY));
    return grid;
}

// ============================================================
// 【下一步工作】地图打印加颜色（第八步：颜色系统）
// ANSI 颜色代码示例：
//   "\033[0m"  重置
//   "\033[33m" 黄色（玩家 @）
//   "\033[31m" 红色（箱子 $）
//   "\033[32m" 绿色（目标点 ^）
//   "\033[92m" 亮绿色（箱子在目标点上 *）
//   "\033[37m" 灰色（墙壁 #）
//   "\033[90m" 深灰色（障碍物 .）
// 实现方法：在输出每个字符前先输出对应的颜色代码
// ============================================================
void printMap(const std::vector<std::vector<char>> &grid) {
    // TODO: 根据格子字符输出对应的 ANSI 颜色代码
    // 注意：箱子在目标点上时显示 '*'（亮绿色），需要检测周围是否有目标点
    //       或者维护一个单独的目标点位置列表来判断当前箱子是否在目标上
    for (int r = 0; r < MAP_ROWS; ++r) {
        for (int c = 0; c < MAP_COLS; ++c) {
            std::cout << grid[r][c];
        }
        std::cout << '\n';
    }
}

// 检查坐标是否在地图有效范围内（已完成，无需修改）
bool isValidPosition(int row, int col) {
    return row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS;
}

// 安全获取格子字符：若越界则返回墙壁（已完成，无需修改）
char getCell(const std::vector<std::vector<char>> &grid, int row, int col) {
    if (isValidPosition(row, col)) {
        return grid[row][col];
    }
    return SYMBOL_WALL;
}

// 安全设置格子字符：仅在坐标有效时修改（已完成，无需修改）
void setCell(std::vector<std::vector<char>> &grid, int row, int col, char value) {
    if (isValidPosition(row, col)) {
        grid[row][col] = value;
    }
}

// ============================================================
// 【下一步工作】保存初始状态（第二步：重启功能）
// 用途：按 R 键时恢复到关卡开始时的状态
// 需要保存：
//   - 地图网格的完整副本（深拷贝）
//   - 玩家初始位置
//   - 步数清零
// 该函数已在关卡开始时调用一次，结果存起来供重启使用
// ============================================================
GameState saveInitialState(const std::vector<std::vector<char>> &grid,
                           int player_row, int player_col) {
    GameState state;
    state.grid = grid;               // vector 赋值是深拷贝
    state.player_row = player_row;
    state.player_col = player_col;
    state.step_count = 0;
    return state;
}
