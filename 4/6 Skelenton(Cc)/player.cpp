#include "player.h"
#include "map.h"
#include <iostream>

// 创建玩家对象并设置初始位置（已完成，无需修改）
Player createPlayer(int row, int col) {
    Player p;
    p.row = row;
    p.col = col;
    return p;
}

// ============================================================
// 【下一步工作】实现玩家移动逻辑（第一步：最小可行版本）
//
// 输入：
//   player     - 玩家当前位置（引用，可修改）
//   direction  - 移动方向（DIR_UP/DOWN/LEFT/RIGHT）
//   grid       - 地图网格（引用，可修改）
// 返回值：移动成功返回 true，被阻挡返回 false
//
// 移动判断流程：
//   1. 根据 direction 计算目标格子 (new_row, new_col)
//   2. 检查目标格子是否越界 → 越界则返回 false
//   3. 检查目标格子内容：
//      a) 墙壁(#) 或障碍物(.) → 被阻挡，返回 false
//      b) 箱子($) → 尝试推动：
//         - 计算箱子前方的格子 (box_new_row, box_new_col)
//         - 调用 canPushBox() 检查箱子能否被推动
//         - 若能推动：调用 pushBox()，然后玩家移动到箱子原来的位置
//         - 若不能推动：返回 false
//      c) 空白(' ') 或目标点(^) → 玩家直接移动
//   4. 更新 grid：
//      - 将玩家旧位置设为空白/目标点（需要知道原位置是否是目标点）
//      - 将玩家新位置设为 '@'
//   5. 更新 player.row / player.col
//   6. 返回 true
// ============================================================
bool movePlayer(Player &player, int direction,
                std::vector<std::vector<char>> &grid) {
    // TODO: 按上述流程实现移动逻辑
    // 方向偏移参考：
    //   DIR_UP:    row - 1, col + 0
    //   DIR_DOWN:  row + 1, col + 0
    //   DIR_LEFT:  row + 0, col - 1
    //   DIR_RIGHT: row + 0, col + 1
    (void)player;
    (void)direction;
    (void)grid;
    return false;
}

// ============================================================
// 【下一步工作】检查箱子能否被推动（第一步：最小可行版本）
//
// 输入：
//   box_row, box_col - 箱子当前位置
//   direction        - 推动方向
//   grid             - 地图网格
// 返回值：可以推动返回 true，否则返回 false
//
// 检查流程：
//   1. 根据 direction 计算箱子前方格子 (new_row, new_col)
//   2. 检查是否越界 → 越界则不能推
//   3. 检查该格子内容：
//      - 墙壁(#)、障碍物(.)、箱子($) → 不能推
//      - 空白(' ')、目标点(^)        → 可以推
// ============================================================
bool canPushBox(int box_row, int box_col, int direction,
                const std::vector<std::vector<char>> &grid) {
    // TODO: 按上述流程实现检查逻辑
    (void)box_row;
    (void)box_col;
    (void)direction;
    (void)grid;
    return false;
}

// ============================================================
// 【下一步工作】推动箱子（第一步：最小可行版本）
//
// 输入：
//   box_row, box_col - 箱子当前位置（引用，会被修改）
//   direction        - 推动方向
//   grid             - 地图网格（引用，会被修改）
//
// 操作流程：
//   1. 根据 direction 计算箱子新位置 (new_row, new_col)
//   2. 更新 grid：
//      - 箱子旧位置设为空白/目标点
//      - 箱子新位置设为 '$'（如果新位置是目标点，仍用 '$'，打印时再判断）
//   3. 更新 box_row / box_col 为新位置
// ============================================================
void pushBox(int &box_row, int &box_col, int direction,
             std::vector<std::vector<char>> &grid) {
    // TODO: 按上述流程实现推动逻辑
    (void)box_row;
    (void)box_col;
    (void)direction;
    (void)grid;
}

// ============================================================
// 【下一步工作】读取用户输入（第一步 / 后续改进）
// 当前实现：用 std::cin >> 读取（需要按 Enter）
// 改进方向：使用 termios 实现无缓冲输入（不需要按 Enter）
// 注意：CS 系服务器上 termios 可用，但实现稍复杂
// 建议：先用 cin 实现，最后再优化
// ============================================================
char getInput() {
    char input;
    std::cin >> input;
    return input;
}

// ============================================================
// 【下一步工作】将键盘输入解析为方向（第一步：最小可行版本）
//
// 映射关系：
//   'W' / 'w' → DIR_UP
//   'S' / 's' → DIR_DOWN
//   'A' / 'a' → DIR_LEFT
//   'D' / 'd' → DIR_RIGHT
//   'R' / 'r' → 特殊值 -2（表示重新开始，在 main.cpp 中处理）
//   'Q' / 'q' → 特殊值 -3（表示退出游戏，在 main.cpp 中处理）
//   其他字符  → -1（无效输入）
// ============================================================
int parseDirection(char input) {
    // TODO: 实现输入到方向的映射
    (void)input;
    return -1;
}

// 重置玩家位置到初始坐标（已完成，无需修改）
void resetPlayer(Player &player, int start_row, int start_col) {
    player.row = start_row;
    player.col = start_col;
}
