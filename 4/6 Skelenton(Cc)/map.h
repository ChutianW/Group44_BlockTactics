#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

// 地图行数和列数（10x10 网格）
const int MAP_ROWS = 10;
const int MAP_COLS = 10;

// 符号常量定义
const char SYMBOL_WALL = '#';        // 墙壁
const char SYMBOL_EMPTY = ' ';       // 空白格子
const char SYMBOL_PLAYER = '@';      // 玩家
const char SYMBOL_BOX = '$';         // 箱子
const char SYMBOL_TARGET = '^';      // 目标点
const char SYMBOL_OBSTACLE = '.';    // 障碍物
const char SYMBOL_BOX_ON_TARGET = '*';  // 箱子已放置在目标点上

// ANSI 颜色代码
const std::string COLOR_RESET = "\033[0m";
const std::string COLOR_YELLOW = "\033[33m";       // 玩家
const std::string COLOR_RED = "\033[31m";          // 箱子
const std::string COLOR_GREEN = "\033[32m";        // 目标点
const std::string COLOR_BRIGHT_GREEN = "\033[92m"; // 箱子在目标点上
const std::string COLOR_GRAY = "\033[90m";         // 墙壁
const std::string COLOR_DARK_GRAY = "\033[37m";    // 障碍物
const std::string COLOR_CYAN = "\033[36m";         // UI 元素
const std::string COLOR_BOLD = "\033[1m";

// 难度等级
enum Difficulty {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3
};

// 难度设置
struct DifficultySettings {
    int num_boxes;
    int min_obstacles;
    int max_obstacles;
};

// 游戏状态结构体 —— 用于保存/回退游戏进度
struct GameState {
    std::vector<std::vector<char>> grid;  // 当前地图网格
    int player_row;                       // 玩家所在行
    int player_col;                       // 玩家所在列
    int step_count;                       // 已走步数
};

// 地图配置结构体 —— 用于随机生成地图时传入参数
struct MapConfig {
    int box_count;      // 箱子数量
    int obstacle_count; // 障碍物数量
};

// —— 难度相关 ——
DifficultySettings getDifficultySettings(Difficulty diff);

// —— 地图创建 ——
void initSampleMap(std::vector<std::vector<char>> &grid,
                   int &start_row, int &start_col,
                   std::vector<std::pair<int, int>> &target_positions);
void generateRandomMap(std::vector<std::vector<char>> &grid,
                       int &start_row, int &start_col,
                       std::vector<std::pair<int, int>> &target_positions,
                       Difficulty diff);

// —— 随机地图生成辅助函数 ——
void clearMap(std::vector<std::vector<char>> &grid,
              std::vector<std::pair<int, int>> &target_positions);
void addBorderWalls(std::vector<std::vector<char>> &grid);
bool placePlayer(std::vector<std::vector<char>> &grid, int &start_row, int &start_col);
bool placeBoxesAndTargets(std::vector<std::vector<char>> &grid,
                          std::vector<std::pair<int, int>> &target_positions, int count);
bool placeObstacles(std::vector<std::vector<char>> &grid, int count);
bool isCorner(int x, int y);
bool isValidBoxPosition(const std::vector<std::vector<char>> &grid, int x, int y);
bool isValidTargetPosition(const std::vector<std::vector<char>> &grid, int x, int y);
bool hasMinimumMobility(const std::vector<std::vector<char>> &grid, int x, int y);

// —— 地图显示 ——
void printMap(const std::vector<std::vector<char>> &grid,
              const std::vector<std::pair<int, int>> &target_positions,
              int player_row, int player_col, bool color_enabled = true);

// —— 地图工具函数 ——
bool isValidPosition(int row, int col);
char getCell(const std::vector<std::vector<char>> &grid, int row, int col);
void setCell(std::vector<std::vector<char>> &grid, int row, int col, char value);

// —— 目标点管理 ——
bool isTarget(const std::vector<std::pair<int, int>> &target_positions, int x, int y);
int getTargetCount(const std::vector<std::pair<int, int>> &target_positions);
int countCompletedTargets(const std::vector<std::vector<char>> &grid,
                          const std::vector<std::pair<int, int>> &target_positions);
bool isBoxOnTarget(const std::vector<std::pair<int, int>> &target_positions,
                   int row, int col);

// —— 重新开始辅助 ——
GameState saveInitialState(const std::vector<std::vector<char>> &grid,
                           const std::vector<std::pair<int, int>> &target_positions,
                           int player_row, int player_col);
void restoreInitialState(std::vector<std::vector<char>> &grid,
                         std::vector<std::pair<int, int>> &target_positions,
                         int &player_row, int &player_col, int &step_count,
                         const GameState &initial_state);

#endif // MAP_H
