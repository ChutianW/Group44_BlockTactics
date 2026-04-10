#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include <stack>

// 玩家结构体 —— 记录玩家在地图上的位置和步数
struct Player {
    int row;   // 玩家所在行
    int col;   // 玩家所在列
    int steps; // 已走步数

    Player();
    Player(int start_row, int start_col);
    void resetSteps();
    void incrementSteps();
    int getSteps() const;
    void setPosition(int new_row, int new_col);
};

// 移动方向常量
const int DIR_UP = 0;    // 上
const int DIR_DOWN = 1;  // 下
const int DIR_LEFT = 2;  // 左
const int DIR_RIGHT = 3; // 右

// 撤销系统用的游戏状态快照（动态内存）
struct UndoState {
    std::vector<std::vector<char>> grid;
    int player_row;
    int player_col;
    int step_count;

    UndoState() : player_row(0), player_col(0), step_count(0) {}
    UndoState(const std::vector<std::vector<char>> &g, int pr, int pc, int sc)
        : grid(g), player_row(pr), player_col(pc), step_count(sc) {}
};

// 撤销系统类 —— 使用动态内存（new/delete），课程要求
class UndoSystem {
private:
    std::stack<UndoState *> history;  // 用指针栈管理动态内存
    int max_history;

public:
    UndoSystem(int max_size = 100);
    ~UndoSystem();

    void saveState(const std::vector<std::vector<char>> &grid, const Player &player);
    bool undo(std::vector<std::vector<char>> &grid, Player &player);
    void clear();
    bool canUndo() const;
    int getHistorySize() const;
    int getMaxHistory() const;
};

// —— 玩家操作 ——
// 创建玩家对象（设置初始位置）
Player createPlayer(int row, int col);
// 尝试移动玩家：若移动成功返回 true，失败返回 false
// 该函数内部会处理碰撞检测和箱子推动
bool movePlayer(Player &player, int direction,
                std::vector<std::vector<char>> &grid,
                const std::vector<std::pair<int, int>> &target_positions,
                UndoSystem *undo = nullptr);

// —— 箱子推动 ——
// 检查指定方向的箱子是否可以被推动（目标格子不能是墙/障碍/另一个箱子）
bool canPushBox(int box_row, int box_col, int direction,
                const std::vector<std::vector<char>> &grid);
// 推动箱子到指定方向，并更新地图格子
void pushBox(int &box_row, int &box_col, int direction,
             std::vector<std::vector<char>> &grid,
             const std::vector<std::pair<int, int>> &target_positions);

// —— 输入处理 ——
// 从终端读取单个字符输入（Linux termios 实现，无需按 Enter）
char getch();
// 将输入的字符（W/A/S/D 或方向键）解析为方向常量
int parseDirection(char input);

// —— 方向工具 ——
// 获取方向的行列偏移量
void getDirectionOffset(int direction, int &drow, int &dcol);

// —— 重置 ——
// 将玩家位置重置到初始位置（用于重新开始关卡）
void resetPlayer(Player &player, int start_row, int start_col);

#endif // PLAYER_H
