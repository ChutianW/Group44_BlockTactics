#include "player.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

// ============================================================
// Player 结构体的成员函数实现
// ============================================================

Player::Player() : row(0), col(0), steps(0) {}

Player::Player(int start_row, int start_col)
    : row(start_row), col(start_col), steps(0) {}

void Player::resetSteps() {
    steps = 0;
}

void Player::incrementSteps() {
    steps++;
}

int Player::getSteps() const {
    return steps;
}

void Player::setPosition(int new_row, int new_col) {
    row = new_row;
    col = new_col;
}

// ============================================================
// 撤销系统（动态内存）—— 使用 new/delete 管理状态快照
// 这是课程要求的 Dynamic Memory 评分点
// ============================================================

UndoSystem::UndoSystem(int max_size) : max_history(max_size) {}

UndoSystem::~UndoSystem() {
    clear();
}

// 保存当前状态到历史记录（用 new 分配动态内存）
void UndoSystem::saveState(const std::vector<std::vector<char>> &grid, const Player &player) {
    // 如果最大历史记录为0（Hard难度禁用undo），直接跳过
    if (max_history == 0) return;

    UndoState *state = new UndoState(grid, player.row, player.col, player.getSteps());

    history.push(state);

    // 限制历史记录大小，超出则删除最旧的状态
    while ((int)history.size() > max_history) {
        UndoState *old = history.top();
        history.pop();
        delete old;
    }
}

// 撤销上一步操作，恢复之前的状态
bool UndoSystem::undo(std::vector<std::vector<char>> &grid, Player &player) {
    if (history.empty()) {
        return false;
    }

    UndoState *state = history.top();
    history.pop();

    // 恢复地图和玩家位置（不恢复步数）
    grid = state->grid;
    player.row = state->player_row;
    player.col = state->player_col;

    // 释放动态内存
    delete state;

    return true;
}

// 清空所有历史记录并释放内存
void UndoSystem::clear() {
    while (!history.empty()) {
        UndoState *state = history.top();
        history.pop();
        delete state;
    }
}

bool UndoSystem::canUndo() const {
    return !history.empty();
}

int UndoSystem::getHistorySize() const {
    return history.size();
}

int UndoSystem::getMaxHistory() const {
    return max_history;
}

// ============================================================
// 创建玩家对象并设置初始位置
// ============================================================
Player createPlayer(int row, int col) {
    Player p;
    p.row = row;
    p.col = col;
    p.steps = 0;
    return p;
}

// ============================================================
// 获取方向的行列偏移量
//   DIR_UP:    row - 1, col + 0
//   DIR_DOWN:  row + 1, col + 0
//   DIR_LEFT:  row + 0, col - 1
//   DIR_RIGHT: row + 0, col + 1
// ============================================================
void getDirectionOffset(int direction, int &drow, int &dcol) {
    drow = 0;
    dcol = 0;
    switch (direction) {
        case DIR_UP:
            drow = -1;
            break;
        case DIR_DOWN:
            drow = 1;
            break;
        case DIR_LEFT:
            dcol = -1;
            break;
        case DIR_RIGHT:
            dcol = 1;
            break;
    }
}

// ============================================================
// 检查箱子是否可以被推动
// 推动方向的下一格必须是空白或目标点（不能是墙/障碍/另一个箱子）
// ============================================================
bool canPushBox(int box_row, int box_col, int direction,
                const std::vector<std::vector<char>> &grid) {
    int drow, dcol;
    getDirectionOffset(direction, drow, dcol);

    int behind_row = box_row + drow;
    int behind_col = box_col + dcol;

    // 检查越界
    if (behind_row < 0 || behind_row >= MAP_ROWS || behind_col < 0 || behind_col >= MAP_COLS) {
        return false;
    }

    char behind_cell = grid[behind_row][behind_col];

    // 只能推到空白格或目标点上
    return behind_cell == SYMBOL_EMPTY || behind_cell == SYMBOL_TARGET;
}

// ============================================================
// 推动箱子到指定方向
// 需要判断箱子旧位置和新位置是否是目标点，以恢复正确的格子符号
// ============================================================
void pushBox(int &box_row, int &box_col, int direction,
             std::vector<std::vector<char>> &grid,
             const std::vector<std::pair<int, int>> &target_positions) {
    int drow, dcol;
    getDirectionOffset(direction, drow, dcol);

    int new_row = box_row + drow;
    int new_col = box_col + dcol;

    // 设置箱子新位置的符号（箱子仍在目标点上时保持原样，由 printMap 判断颜色）
    if (grid[new_row][new_col] == SYMBOL_TARGET) {
        grid[new_row][new_col] = SYMBOL_BOX;  // 推到目标点上
    } else {
        grid[new_row][new_col] = SYMBOL_BOX;
    }

    // 恢复箱子旧位置：如果是目标点则恢复为目标符号，否则恢复为空白
    bool was_on_target = false;
    for (const auto &pos : target_positions) {
        if (pos.first == box_col && pos.second == box_row) {
            was_on_target = true;
            break;
        }
    }
    if (was_on_target) {
        grid[box_row][box_col] = SYMBOL_TARGET;
    } else {
        grid[box_row][box_col] = SYMBOL_EMPTY;
    }

    // 更新箱子坐标
    box_row = new_row;
    box_col = new_col;
}

// ============================================================
// 移动玩家 —— 核心游戏逻辑
//
// 移动判断流程：
//   1. 计算目标格子 (new_row, new_col)
//   2. 检查越界 → 返回 false
//   3. 检查目标格子内容：
//      a) 墙壁(#) 或障碍物(.) → 被阻挡，返回 false
//      b) 箱子($) → 尝试推动：
//         - 调用 canPushBox() 检查能否推动
//         - 能推：pushBox()，玩家移动到箱子原位置
//         - 不能推：返回 false
//      c) 空白(' ') 或目标点(^) → 直接移动
//   4. 更新 grid：清除玩家旧位置，设置新位置
//   5. 更新 player 坐标，步数+1
//   6. 保存状态到撤销系统（如果 undo != nullptr）
// ============================================================
bool movePlayer(Player &player, int direction,
                std::vector<std::vector<char>> &grid,
                const std::vector<std::pair<int, int>> &target_positions,
                UndoSystem *undo) {
    int drow, dcol;
    getDirectionOffset(direction, drow, dcol);

    int new_row = player.row + drow;
    int new_col = player.col + dcol;

    // 检查越界
    if (new_row < 0 || new_row >= MAP_ROWS || new_col < 0 || new_col >= MAP_COLS) {
        return false;
    }

    char target_cell = grid[new_row][new_col];

    // 墙壁或障碍物 → 被阻挡
    if (target_cell == SYMBOL_WALL || target_cell == SYMBOL_OBSTACLE) {
        return false;
    }

    // 箱子 → 尝试推动
    if (target_cell == SYMBOL_BOX) {
        if (!canPushBox(new_row, new_col, direction, grid)) {
            return false;  // 箱子推不动
        }

        // 保存状态（推动前）
        if (undo != nullptr) {
            undo->saveState(grid, player);
        }

        // 推动箱子
        int box_row = new_row;
        int box_col = new_col;
        pushBox(box_row, box_col, direction, grid, target_positions);

        // 恢复玩家旧位置（清除旧位置的@）
        bool was_on_target = false;
        for (const auto &pos : target_positions) {
            if (pos.first == player.col && pos.second == player.row) {
                was_on_target = true;
                break;
            }
        }
        if (was_on_target) {
            grid[player.row][player.col] = SYMBOL_TARGET;
        } else {
            grid[player.row][player.col] = SYMBOL_EMPTY;
        }

        // 玩家移动到箱子原来的位置
        grid[new_row][new_col] = SYMBOL_PLAYER;
        player.row = new_row;
        player.col = new_col;

    } else if (target_cell == SYMBOL_EMPTY || target_cell == SYMBOL_TARGET) {
        // 空白或目标点 → 直接移动
        // 保存状态（移动前）
        if (undo != nullptr) {
            undo->saveState(grid, player);
        }

        // 清除玩家旧位置
        bool was_on_target = false;
        for (const auto &pos : target_positions) {
            if (pos.first == player.col && pos.second == player.row) {
                was_on_target = true;
                break;
            }
        }
        if (was_on_target) {
            grid[player.row][player.col] = SYMBOL_TARGET;
        } else {
            grid[player.row][player.col] = SYMBOL_EMPTY;
        }

        // 设置玩家新位置
        grid[new_row][new_col] = SYMBOL_PLAYER;
        player.row = new_row;
        player.col = new_col;

    } else {
        return false;  // 其他情况不处理
    }

    player.incrementSteps();
    return true;
}

// ============================================================
// Linux 下读取单字符输入（无需按 Enter）
// 使用 termios 禁用行缓冲和回显，替代 Windows 的 _getch()
// ============================================================
char getch() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // 禁用行缓冲和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // 恢复终端设置
    return ch;
}

// ============================================================
// 将键盘输入解析为方向或特殊命令
//
// 映射关系：
//   'W' / 'w' → DIR_UP
//   'S' / 's' → DIR_DOWN
//   'A' / 'a' → DIR_LEFT
//   'D' / 'd' → DIR_RIGHT
//   其他字符  → -1（无效输入，调用者在 main.cpp 中处理 R/Q/U/H 等）
// ============================================================
int parseDirection(char input) {
    switch (input) {
        case 'W': case 'w': return DIR_UP;
        case 'S': case 's': return DIR_DOWN;
        case 'A': case 'a': return DIR_LEFT;
        case 'D': case 'd': return DIR_RIGHT;
        default: return -1;
    }
}

// 重置玩家位置到初始坐标（已完成，无需修改）
void resetPlayer(Player &player, int start_row, int start_col) {
    player.row = start_row;
    player.col = start_col;
}
