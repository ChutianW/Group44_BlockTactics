#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Map dimensions
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

// Game symbols
const char PLAYER = '@';
const char BOX = '$';
const char TARGET = '^';
const char WALL = '#';
const char OBSTACLE = '.';
const char EMPTY = ' ';
const char BOX_ON_TARGET = '*';  // Special state for box on target

// ANSI Color codes
const string COLOR_RESET = "\033[0m";
const string COLOR_YELLOW = "\033[33m";      // Player
const string COLOR_RED = "\033[31m";         // Box
const string COLOR_GREEN = "\033[32m";       // Target
const string COLOR_BRIGHT_GREEN = "\033[92m"; // Box on target
const string COLOR_GRAY = "\033[90m";        // Wall
const string COLOR_DARK_GRAY = "\033[37m";   // Obstacle
const string COLOR_CYAN = "\033[36m";        // UI elements
const string COLOR_BOLD = "\033[1m";

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

// Map class declaration
class Map {
private:
    vector<vector<char>> grid;
    vector<vector<char>> initial_grid;  // For restart
    vector<pair<int, int>> target_positions;  // Track target locations
    vector<pair<int, int>> initial_target_positions;  // For restart
    int player_start_x;
    int player_start_y;
    bool color_enabled;

    // Random generation helpers
    bool isCorner(int x, int y) const;
    bool isAdjacentToOuterWall(int x, int y) const;
    bool hasMinimumMobility(int x, int y) const;
    int countAdjacentFree(int x, int y) const;

public:
    // Constructor
    Map();

    // Initialize with a fixed sample map
    void initSampleMap();

    // Random map generation
    void generateRandomMap(Difficulty diff);
    void clearMap();
    void addBorderWalls();
    bool placePlayer();
    bool placeBoxesAndTargets(int count);
    bool placeObstacles(int count);
    bool isValidBoxPosition(int x, int y) const;
    bool isValidTargetPosition(int x, int y) const;

    // Get/Set cell
    char getCell(int x, int y) const;
    void setCell(int x, int y, char value);

    // Print the map to console
    void printMap() const;
    void printMapWithColor() const;

    // Store initial state for restart
    void saveInitialState();
    void restoreInitialState();

    // Target tracking
    void addTarget(int x, int y);
    void clearTargets();
    bool isTarget(int x, int y) const;
    int getTargetCount() const;

    // Boundary check
    bool isValidPosition(int x, int y) const;

    // Check if cell is walkable
    bool isWalkable(int x, int y) const;

    // Get map dimensions
    int getWidth() const;
    int getHeight() const;

    // Player start position
    int getPlayerStartX() const;
    int getPlayerStartY() const;

    // Color toggle
    void setColorEnabled(bool enabled);
    bool isColorEnabled() const;

    // Get grid for undo system
    vector<vector<char>> getGrid() const;
    void setGrid(const vector<vector<char>>& new_grid);
};

// Get difficulty settings
DifficultySettings getDifficultySettings(Difficulty diff);

#endif // MAP_H
