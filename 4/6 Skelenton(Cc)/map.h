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

// —— 地图创建 ——
// 创建一个固定的测试地图
std::vector<std::vector<char>> createFixedMap();
// 根据配置随机生成地图
std::vector<std::vector<char>> generateRandomMap(MapConfig config);

// —— 地图显示 ——
// 打印地图到终端（后续需加入 ANSI 颜色代码）
void printMap(const std::vector<std::vector<char>> &grid);

// —— 地图工具函数 ——
// 检查坐标是否在地图范围内
bool isValidPosition(int row, int col);
// 安全地获取某个格子的字符
char getCell(const std::vector<std::vector<char>> &grid, int row, int col);
// 安全地设置某个格子的字符
void setCell(std::vector<std::vector<char>> &grid, int row, int col, char value);

// —— 重新开始辅助 ——
// 保存初始地图状态，用于按 R 重置关卡
GameState saveInitialState(const std::vector<std::vector<char>> &grid,
                           int player_row, int player_col);

#endif // MAP_H
