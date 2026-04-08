#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

// 玩家结构体 —— 记录玩家在地图上的位置
struct Player {
    int row;  // 玩家所在行
    int col;  // 玩家所在列
};

// 移动方向常量
const int DIR_UP = 0;    // 上
const int DIR_DOWN = 1;  // 下
const int DIR_LEFT = 2;  // 左
const int DIR_RIGHT = 3; // 右

// —— 玩家操作 ——
// 创建玩家对象（设置初始位置）
Player createPlayer(int row, int col);
// 尝试移动玩家：若移动成功返回 true，失败返回 false
// 该函数内部会处理碰撞检测和箱子推动
bool movePlayer(Player &player, int direction,
                std::vector<std::vector<char>> &grid);

// —— 箱子推动 ——
// 检查指定方向的箱子是否可以被推动（目标格子不能是墙/障碍/另一个箱子）
bool canPushBox(int box_row, int box_col, int direction,
                const std::vector<std::vector<char>> &grid);
// 推动箱子到指定方向，并更新地图格子
void pushBox(int &box_row, int &box_col, int direction,
             std::vector<std::vector<char>> &grid);

// —— 输入处理 ——
// 从终端读取单个字符输入
char getInput();
// 将输入的字符（W/A/S/D 或方向键）解析为方向常量
int parseDirection(char input);

// —— 重置 ——
// 将玩家位置重置到初始位置（用于重新开始关卡）
void resetPlayer(Player &player, int start_row, int start_col);

#endif // PLAYER_H
