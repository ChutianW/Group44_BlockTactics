#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

// Map dimensions (10x10 grid)
const int MAP_ROWS = 10;
const int MAP_COLS = 10;

// Symbol constants
const char SYMBOL_WALL = '#';
const char SYMBOL_EMPTY = ' ';
const char SYMBOL_PLAYER = '@';
const char SYMBOL_BOX = '$';
const char SYMBOL_TARGET = '^';
const char SYMBOL_OBSTACLE = '%';
const char SYMBOL_BOX_ON_TARGET = '*';

// ANSI color codes
const std::string COLOR_RESET = "\033[0m";
const std::string COLOR_YELLOW = "\033[33m";
const std::string COLOR_RED = "\033[31m";
const std::string COLOR_GREEN = "\033[32m";
const std::string COLOR_BRIGHT_GREEN = "\033[92m";
const std::string COLOR_GRAY = "\033[90m";
const std::string COLOR_DARK_GRAY = "\033[37m";
const std::string COLOR_CYAN = "\033[36m";
const std::string COLOR_BOLD = "\033[1m";

// Difficulty levels
enum Difficulty {
    EASY = 1,
    MEDIUM = 2,
    HARD = 3
};

// Difficulty settings
struct DifficultySettings {
    int num_boxes;
    int min_obstacles;
    int max_obstacles;
};

// Game state for save/restore
struct GameState {
    std::vector<std::vector<char>> grid;
    int player_row;
    int player_col;
    int step_count;
};

// Map config for random generation
struct MapConfig {
    int box_count;
    int obstacle_count;
};

// Difficulty
DifficultySettings getDifficultySettings(Difficulty diff);

// Map creation
void initSampleMap(std::vector<std::vector<char>> &grid,
                   int &start_row, int &start_col,
                   std::vector<std::pair<int, int>> &target_positions);
void generateRandomMap(std::vector<std::vector<char>> &grid,
                       int &start_row, int &start_col,
                       std::vector<std::pair<int, int>> &target_positions,
                       Difficulty diff);

// Random map generation helpers
void clearMap(std::vector<std::vector<char>> &grid,
              std::vector<std::pair<int, int>> &target_positions);
void addBorderWalls(std::vector<std::vector<char>> &grid);
bool placePlayer(std::vector<std::vector<char>> &grid, int &start_row, int &start_col);
bool placeBoxesAndTargets(std::vector<std::vector<char>> &grid,
                          std::vector<std::pair<int, int>> &target_positions, int count);
bool placeObstacles(std::vector<std::vector<char>> &grid, int count);
bool isCorner(const std::vector<std::vector<char>> &grid, int x, int y);
bool isValidBoxPosition(const std::vector<std::vector<char>> &grid, int x, int y);
bool isValidTargetPosition(const std::vector<std::vector<char>> &grid, int x, int y);
bool hasMinimumMobility(const std::vector<std::vector<char>> &grid, int x, int y);

// Map display
void printMap(const std::vector<std::vector<char>> &grid,
              const std::vector<std::pair<int, int>> &target_positions,
              int player_row, int player_col, bool color_enabled = true);

// Map utilities
bool isValidPosition(int row, int col);
char getCell(const std::vector<std::vector<char>> &grid, int row, int col);
void setCell(std::vector<std::vector<char>> &grid, int row, int col, char value);

// Target management
bool isTarget(const std::vector<std::pair<int, int>> &target_positions, int x, int y);
int getTargetCount(const std::vector<std::pair<int, int>> &target_positions);
int countCompletedTargets(const std::vector<std::vector<char>> &grid,
                          const std::vector<std::pair<int, int>> &target_positions);
bool isBoxOnTarget(const std::vector<std::pair<int, int>> &target_positions,
                   int row, int col);

// Restart helpers
GameState saveInitialState(const std::vector<std::vector<char>> &grid,
                           const std::vector<std::pair<int, int>> &target_positions,
                           int player_row, int player_col);
void restoreInitialState(std::vector<std::vector<char>> &grid,
                         std::vector<std::pair<int, int>> &target_positions,
                         int &player_row, int &player_col, int &step_count,
                         const GameState &initial_state);

#endif // MAP_H
