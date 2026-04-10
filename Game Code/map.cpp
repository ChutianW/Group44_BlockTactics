#include "map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// ============================================================
// 难度设置（对应 Easy/Medium/Hard 的箱子和障碍物数量）
// ============================================================
DifficultySettings getDifficultySettings(Difficulty diff) {
    DifficultySettings settings;
    switch (diff) {
        case EASY:
            settings.num_boxes = 3;
            settings.min_obstacles = 0;
            settings.max_obstacles = 0;
            break;
        case MEDIUM:
            settings.num_boxes = 5;
            settings.min_obstacles = 3;
            settings.max_obstacles = 5;
            break;
        case HARD:
            settings.num_boxes = 7;
            settings.min_obstacles = 6;
            settings.max_obstacles = 10;
            break;
        default:
            settings.num_boxes = 3;
            settings.min_obstacles = 0;
            settings.max_obstacles = 0;
    }
    return settings;
}

// ============================================================
// 创建固定测试地图（用于调试和最小可行版本）
// 包含：墙壁边框、内部墙壁、3个箱子、3个目标点、玩家起始位置
// ============================================================
void initSampleMap(std::vector<std::vector<char>> &grid,
                   int &start_row, int &start_col,
                   std::vector<std::pair<int, int>> &target_positions) {
    // 先清空
    grid.assign(MAP_ROWS, std::vector<char>(MAP_COLS, SYMBOL_EMPTY));
    target_positions.clear();

    // 添加四面墙壁边框
    for (int c = 0; c < MAP_COLS; ++c) {
        grid[0][c] = SYMBOL_WALL;
        grid[MAP_ROWS - 1][c] = SYMBOL_WALL;
    }
    for (int r = 0; r < MAP_ROWS; ++r) {
        grid[r][0] = SYMBOL_WALL;
        grid[r][MAP_COLS - 1] = SYMBOL_WALL;
    }

    // 内部墙壁
    grid[2][3] = SYMBOL_WALL;
    grid[2][4] = SYMBOL_WALL;
    grid[3][3] = SYMBOL_WALL;
    grid[5][6] = SYMBOL_WALL;
    grid[5][7] = SYMBOL_WALL;

    // 目标点（同时记录位置用于后续检测）
    grid[4][7] = SYMBOL_TARGET;
    target_positions.push_back({7, 4});
    grid[6][3] = SYMBOL_TARGET;
    target_positions.push_back({3, 6});
    grid[7][7] = SYMBOL_TARGET;
    target_positions.push_back({7, 7});

    // 箱子
    grid[3][4] = SYMBOL_BOX;
    grid[4][5] = SYMBOL_BOX;
    grid[5][3] = SYMBOL_BOX;

    // 玩家起始位置
    grid[2][2] = SYMBOL_PLAYER;
    start_row = 2;
    start_col = 2;
}

// ============================================================
// 清空地图（清除所有元素和目标点记录）
// ============================================================
void clearMap(std::vector<std::vector<char>> &grid,
              std::vector<std::pair<int, int>> &target_positions) {
    grid.assign(MAP_ROWS, std::vector<char>(MAP_COLS, SYMBOL_EMPTY));
    target_positions.clear();
}

// ============================================================
// 添加四面墙壁边框
// ============================================================
void addBorderWalls(std::vector<std::vector<char>> &grid) {
    for (int c = 0; c < MAP_COLS; ++c) {
        grid[0][c] = SYMBOL_WALL;
        grid[MAP_ROWS - 1][c] = SYMBOL_WALL;
    }
    for (int r = 0; r < MAP_ROWS; ++r) {
        grid[r][0] = SYMBOL_WALL;
        grid[r][MAP_COLS - 1] = SYMBOL_WALL;
    }
}

// ============================================================
// 检查位置是否为角落区域（不适合放箱子或目标点）
// ============================================================
bool isCorner(int x, int y) {
    if (x <= 1 || x >= MAP_COLS - 2 || y <= 1 || y >= MAP_ROWS - 2) {
        return true;
    }
    return false;
}

// ============================================================
// 检查箱子位置是否有效（在范围内、当前为空白、不在角落）
// ============================================================
bool isValidBoxPosition(const std::vector<std::vector<char>> &grid, int x, int y) {
    if (x < 0 || x >= MAP_COLS || y < 0 || y >= MAP_ROWS) return false;
    if (grid[y][x] != SYMBOL_EMPTY) return false;
    if (isCorner(x, y)) return false;
    return true;
}

// ============================================================
// 检查目标点位置是否有效（在范围内、空白、不在角落、不贴外墙）
// ============================================================
bool isValidTargetPosition(const std::vector<std::vector<char>> &grid, int x, int y) {
    if (x < 0 || x >= MAP_COLS || y < 0 || y >= MAP_ROWS) return false;
    if (grid[y][x] != SYMBOL_EMPTY) return false;
    if (isCorner(x, y)) return false;
    // 目标点不能紧贴外墙
    if (x == 1 || x == MAP_COLS - 2 || y == 1 || y == MAP_ROWS - 2) return false;
    return true;
}

// ============================================================
// 计算某位置周围自由格子数（用于检查箱子可移动性）
// ============================================================
static int countAdjacentFree(const std::vector<std::vector<char>> &grid, int x, int y) {
    int count = 0;
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < MAP_COLS && ny >= 0 && ny < MAP_ROWS) {
            char cell = grid[ny][nx];
            if (cell == SYMBOL_EMPTY || cell == SYMBOL_TARGET) {
                count++;
            }
        }
    }
    return count;
}

// ============================================================
// 检查箱子是否至少有2个相邻自由格子（保证基本可移动性）
// ============================================================
bool hasMinimumMobility(const std::vector<std::vector<char>> &grid, int x, int y) {
    return countAdjacentFree(grid, x, y) >= 2;
}

// ============================================================
// 随机放置玩家（在内部空白区域寻找位置）
// ============================================================
bool placePlayer(std::vector<std::vector<char>> &grid, int &start_row, int &start_col) {
    for (int attempts = 0; attempts < 100; attempts++) {
        int x = 2 + rand() % (MAP_COLS - 4);
        int y = 2 + rand() % (MAP_ROWS - 4);

        if (grid[y][x] == SYMBOL_EMPTY) {
            grid[y][x] = SYMBOL_PLAYER;
            start_row = y;
            start_col = x;
            return true;
        }
    }
    return false;
}

// ============================================================
// 随机放置箱子和目标点（成对放置，确保箱子可移动）
// ============================================================
bool placeBoxesAndTargets(std::vector<std::vector<char>> &grid,
                          std::vector<std::pair<int, int>> &target_positions, int count) {
    int placed = 0;
    int max_attempts = 1000;

    while (placed < count && max_attempts > 0) {
        max_attempts--;

        // 随机选箱子位置
        int box_x = 2 + rand() % (MAP_COLS - 4);
        int box_y = 2 + rand() % (MAP_ROWS - 4);

        if (!isValidBoxPosition(grid, box_x, box_y)) continue;

        // 随机选目标点位置
        int target_x = 2 + rand() % (MAP_COLS - 4);
        int target_y = 2 + rand() % (MAP_ROWS - 4);

        if (!isValidTargetPosition(grid, target_x, target_y)) continue;
        if (target_x == box_x && target_y == box_y) continue;

        // 放置箱子
        grid[box_y][box_x] = SYMBOL_BOX;

        // 检查箱子可移动性，不满足则移除
        if (!hasMinimumMobility(grid, box_x, box_y)) {
            grid[box_y][box_x] = SYMBOL_EMPTY;
            continue;
        }

        // 放置目标点并记录
        grid[target_y][target_x] = SYMBOL_TARGET;
        target_positions.push_back({target_x, target_y});

        placed++;
    }

    return placed == count;
}

// ============================================================
// 随机放置障碍物（避免相邻太多墙壁导致路径被封死）
// ============================================================
bool placeObstacles(std::vector<std::vector<char>> &grid, int count) {
    int placed = 0;
    int max_attempts = 500;

    while (placed < count && max_attempts > 0) {
        max_attempts--;

        int x = 2 + rand() % (MAP_COLS - 4);
        int y = 2 + rand() % (MAP_ROWS - 4);

        if (grid[y][x] != SYMBOL_EMPTY) continue;

        // 统计相邻墙壁/障碍物数量
        int adjacent_walls = 0;
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < MAP_COLS && ny >= 0 && ny < MAP_ROWS) {
                char cell = grid[ny][nx];
                if (cell == SYMBOL_WALL || cell == SYMBOL_OBSTACLE) {
                    adjacent_walls++;
                }
            }
        }

        // 相邻墙壁 >= 2 则不放置（避免封死路径）
        if (adjacent_walls >= 2) continue;

        grid[y][x] = SYMBOL_OBSTACLE;
        placed++;
    }

    return placed >= count / 2;
}

// ============================================================
// 根据难度等级随机生成地图
// 流程：清空 → 加边框 → 放障碍物 → 放箱子/目标点 → 放玩家
// 如果生成失败（50次重试后），回退到固定测试地图
// ============================================================
void generateRandomMap(std::vector<std::vector<char>> &grid,
                       int &start_row, int &start_col,
                       std::vector<std::pair<int, int>> &target_positions,
                       Difficulty diff) {
    DifficultySettings settings = getDifficultySettings(diff);

    int attempts = 0;
    bool valid_map = false;

    while (!valid_map && attempts < 50) {
        attempts++;

        clearMap(grid, target_positions);
        addBorderWalls(grid);

        // 确定障碍物数量
        int num_obstacles = settings.min_obstacles;
        if (settings.max_obstacles > settings.min_obstacles) {
            num_obstacles = settings.min_obstacles +
                          rand() % (settings.max_obstacles - settings.min_obstacles + 1);
        }

        // 放置障碍物
        if (num_obstacles > 0) {
            placeObstacles(grid, num_obstacles);
        }

        // 放置箱子和目标点
        if (!placeBoxesAndTargets(grid, target_positions, settings.num_boxes)) {
            continue;
        }

        // 放置玩家
        if (!placePlayer(grid, start_row, start_col)) {
            continue;
        }

        valid_map = true;
    }

    // 如果50次都没成功，回退到固定测试地图
    if (!valid_map) {
        initSampleMap(grid, start_row, start_col, target_positions);
        return;
    }
}

// ============================================================
// 打印地图到终端（支持 ANSI 颜色）
// 注意：需要单独维护 target_positions 来判断箱子是否在目标点上
// 箱子在目标点上时显示 '*'（亮绿色），否则显示 '$'（红色）
// 玩家位置也需要动态判断是否在目标点上
// ============================================================
void printMap(const std::vector<std::vector<char>> &grid,
              const std::vector<std::pair<int, int>> &target_positions,
              int player_row, int player_col, bool color_enabled) {
    std::cout << "\n";
    for (int r = 0; r < MAP_ROWS; r++) {
        std::cout << "  ";
        for (int c = 0; c < MAP_COLS; c++) {
            char cell = grid[r][c];

            if (!color_enabled) {
                // 无颜色模式：直接输出字符
                std::cout << cell;
                continue;
            }

            // 颜色模式：根据格子类型输出带颜色的字符
            if (r == player_row && c == player_col) {
                // 玩家位置
                std::cout << COLOR_YELLOW << COLOR_BOLD << cell << COLOR_RESET;
            } else if (cell == SYMBOL_BOX) {
                // 检查箱子是否在目标点上
                bool on_target = isTarget(target_positions, c, r);
                if (on_target) {
                    std::cout << COLOR_BRIGHT_GREEN << COLOR_BOLD << cell << COLOR_RESET;
                } else {
                    std::cout << COLOR_RED << cell << COLOR_RESET;
                }
            } else if (cell == SYMBOL_TARGET) {
                std::cout << COLOR_GREEN << cell << COLOR_RESET;
            } else if (cell == SYMBOL_WALL) {
                std::cout << COLOR_GRAY << cell << COLOR_RESET;
            } else if (cell == SYMBOL_OBSTACLE) {
                std::cout << COLOR_DARK_GRAY << cell << COLOR_RESET;
            } else {
                std::cout << cell;
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
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
// 目标点管理函数
// ============================================================

// 检查某位置是否为已记录的目标点
bool isTarget(const std::vector<std::pair<int, int>> &target_positions, int x, int y) {
    for (const auto &pos : target_positions) {
        if (pos.first == x && pos.second == y) {
            return true;
        }
    }
    return false;
}

// 获取目标点总数
int getTargetCount(const std::vector<std::pair<int, int>> &target_positions) {
    return target_positions.size();
}

// 统计有多少个目标点上已经放了箱子
int countCompletedTargets(const std::vector<std::vector<char>> &grid,
                          const std::vector<std::pair<int, int>> &target_positions) {
    int count = 0;
    for (const auto &pos : target_positions) {
        int tx = pos.first;
        int ty = pos.second;
        if (isValidPosition(ty, tx) && grid[ty][tx] == SYMBOL_BOX) {
            count++;
        }
    }
    return count;
}

// 检查某位置是否为箱子所在位置
bool isBoxOnTarget(const std::vector<std::pair<int, int>> &target_positions,
                   int row, int col) {
    return isTarget(target_positions, col, row);
}

// ============================================================
// 保存初始游戏状态，用于关卡重置（按 R 键）
// 深拷贝地图、目标点列表、玩家位置和步数
// ============================================================
GameState saveInitialState(const std::vector<std::vector<char>> &grid,
                           const std::vector<std::pair<int, int>> & /* target_positions */,
                           int player_row, int player_col) {
    GameState state;
    state.grid = grid;                        // vector 赋值是深拷贝
    state.player_row = player_row;
    state.player_col = player_col;
    state.step_count = 0;
    // 注意：target_positions 不包含在 GameState 中，
    // 因为目标点位置是固定的，不会改变
    return state;
}

// ============================================================
// 从保存的初始状态恢复整个关卡
// ============================================================
void restoreInitialState(std::vector<std::vector<char>> &grid,
                         std::vector<std::pair<int, int>> & /* target_positions */,
                         int &player_row, int &player_col, int &step_count,
                         const GameState &initial_state) {
    grid = initial_state.grid;
    // 目标点列表不需要恢复（位置固定不变）
    player_row = initial_state.player_row;
    player_col = initial_state.player_col;
    step_count = initial_state.step_count;
}
