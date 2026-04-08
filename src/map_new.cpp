#include "map.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Get difficulty settings
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

// Constructor - initialize empty map
Map::Map() : player_start_x(1), player_start_y(1), color_enabled(true) {
    grid.resize(MAP_HEIGHT, vector<char>(MAP_WIDTH, EMPTY));
    initial_grid.resize(MAP_HEIGHT, vector<char>(MAP_WIDTH, EMPTY));
}

// Clear the map
void Map::clearMap() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = EMPTY;
        }
    }
    target_positions.clear();
}

// Add border walls
void Map::addBorderWalls() {
    for (int x = 0; x < MAP_WIDTH; x++) {
        grid[0][x] = WALL;
        grid[MAP_HEIGHT - 1][x] = WALL;
    }
    for (int y = 0; y < MAP_HEIGHT; y++) {
        grid[y][0] = WALL;
        grid[y][MAP_WIDTH - 1] = WALL;
    }
}

// Check if position is a corner
bool Map::isCorner(int x, int y) const {
    // Inner corners (excluding border)
    if (x <= 1 || x >= MAP_WIDTH - 2 || y <= 1 || y >= MAP_HEIGHT - 2) {
        return true;
    }
    return false;
}

// Check if position is adjacent to outer wall
bool Map::isAdjacentToOuterWall(int x, int y) const {
    return (x == 1 || x == MAP_WIDTH - 2 || y == 1 || y == MAP_HEIGHT - 2);
}

// Count adjacent free cells
int Map::countAdjacentFree(int x, int y) const {
    int count = 0;
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isValidPosition(nx, ny)) {
            char cell = grid[ny][nx];
            if (cell == EMPTY || cell == TARGET) {
                count++;
            }
        }
    }
    return count;
}

// Check if box has minimum mobility (at least 2 adjacent free cells)
bool Map::hasMinimumMobility(int x, int y) const {
    return countAdjacentFree(x, y) >= 2;
}

// Check if position is valid for box placement
bool Map::isValidBoxPosition(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    if (grid[y][x] != EMPTY) return false;
    if (isCorner(x, y)) return false;
    
    // Temporarily place box to check mobility
    return true;
}

// Check if position is valid for target placement
bool Map::isValidTargetPosition(int x, int y) const {
    if (!isValidPosition(x, y)) return false;
    if (grid[y][x] != EMPTY) return false;
    if (isCorner(x, y)) return false;
    if (isAdjacentToOuterWall(x, y)) return false;
    return true;
}

// Place player on the map
bool Map::placePlayer() {
    // Try to place player in a good starting position
    for (int attempts = 0; attempts < 100; attempts++) {
        int x = 2 + rand() % (MAP_WIDTH - 4);
        int y = 2 + rand() % (MAP_HEIGHT - 4);
        
        if (grid[y][x] == EMPTY) {
            grid[y][x] = PLAYER;
            player_start_x = x;
            player_start_y = y;
            return true;
        }
    }
    return false;
}

// Place boxes and targets
bool Map::placeBoxesAndTargets(int count) {
    int placed = 0;
    int max_attempts = 1000;
    
    while (placed < count && max_attempts > 0) {
        max_attempts--;
        
        // Find valid box position
        int box_x = 2 + rand() % (MAP_WIDTH - 4);
        int box_y = 2 + rand() % (MAP_HEIGHT - 4);
        
        if (!isValidBoxPosition(box_x, box_y)) continue;
        
        // Find valid target position
        int target_x = 2 + rand() % (MAP_WIDTH - 4);
        int target_y = 2 + rand() % (MAP_HEIGHT - 4);
        
        if (!isValidTargetPosition(target_x, target_y)) continue;
        if (target_x == box_x && target_y == box_y) continue;
        
        // Place box
        grid[box_y][box_x] = BOX;
        
        // Check if box has mobility
        if (!hasMinimumMobility(box_x, box_y)) {
            grid[box_y][box_x] = EMPTY;
            continue;
        }
        
        // Place target
        grid[target_y][target_x] = TARGET;
        addTarget(target_x, target_y);
        
        placed++;
    }
    
    return placed == count;
}

// Place obstacles
bool Map::placeObstacles(int count) {
    int placed = 0;
    int max_attempts = 500;
    
    while (placed < count && max_attempts > 0) {
        max_attempts--;
        
        int x = 2 + rand() % (MAP_WIDTH - 4);
        int y = 2 + rand() % (MAP_HEIGHT - 4);
        
        if (grid[y][x] != EMPTY) continue;
        
        // Don't block narrow corridors
        int adjacent_walls = 0;
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};
        
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (isValidPosition(nx, ny)) {
                char cell = grid[ny][nx];
                if (cell == WALL || cell == OBSTACLE) {
                    adjacent_walls++;
                }
            }
        }
        
        // Don't place if it would create a blocked corridor
        if (adjacent_walls >= 2) continue;
        
        grid[y][x] = OBSTACLE;
        placed++;
    }
    
    return placed >= count / 2;  // Allow partial placement
}

// Generate random map based on difficulty
void Map::generateRandomMap(Difficulty diff) {
    DifficultySettings settings = getDifficultySettings(diff);
    
    int attempts = 0;
    bool valid_map = false;
    
    while (!valid_map && attempts < 50) {
        attempts++;
        
        clearMap();
        addBorderWalls();
        
        // Place obstacles first (if any)
        int num_obstacles = settings.min_obstacles;
        if (settings.max_obstacles > settings.min_obstacles) {
            num_obstacles = settings.min_obstacles + 
                          rand() % (settings.max_obstacles - settings.min_obstacles + 1);
        }
        
        if (num_obstacles > 0) {
            placeObstacles(num_obstacles);
        }
        
        // Place boxes and targets
        if (!placeBoxesAndTargets(settings.num_boxes)) {
            continue;
        }
        
        // Place player
        if (!placePlayer()) {
            continue;
        }
        
        valid_map = true;
    }
    
    if (!valid_map) {
        // Fallback to sample map
        initSampleMap();
        return;
    }
    
    saveInitialState();
}

// Initialize with a fixed sample map for testing
void Map::initSampleMap() {
    clearMap();
    addBorderWalls();

    // Add some internal walls
    grid[2][3] = WALL;
    grid[2][4] = WALL;
    grid[3][3] = WALL;
    grid[5][6] = WALL;
    grid[5][7] = WALL;

    // Add targets and track them
    grid[4][7] = TARGET;
    addTarget(7, 4);
    grid[6][3] = TARGET;
    addTarget(3, 6);
    grid[7][7] = TARGET;
    addTarget(7, 7);

    // Add boxes
    grid[3][4] = BOX;
    grid[4][5] = BOX;
    grid[5][3] = BOX;

    // Player start position
    grid[2][2] = PLAYER;
    player_start_x = 2;
    player_start_y = 2;

    // Save initial state
    saveInitialState();
}

// Get cell value at position
char Map::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return WALL;
    }
    return grid[y][x];
}

// Set cell value at position
void Map::setCell(int x, int y, char value) {
    if (isValidPosition(x, y)) {
        grid[y][x] = value;
    }
}

// Print the map to console (no color)
void Map::printMap() const {
    if (color_enabled) {
        printMapWithColor();
        return;
    }
    
    cout << "\n";
    for (int y = 0; y < MAP_HEIGHT; y++) {
        cout << "  ";
        for (int x = 0; x < MAP_WIDTH; x++) {
            cout << grid[y][x];
        }
        cout << "\n";
    }
    cout << "\n";
}

// Print the map with ANSI colors
void Map::printMapWithColor() const {
    cout << "\n";
    for (int y = 0; y < MAP_HEIGHT; y++) {
        cout << "  ";
        for (int x = 0; x < MAP_WIDTH; x++) {
            char cell = grid[y][x];
            switch (cell) {
                case PLAYER:
                    cout << COLOR_YELLOW << COLOR_BOLD << cell << COLOR_RESET;
                    break;
                case BOX:
                    cout << COLOR_RED << cell << COLOR_RESET;
                    break;
                case TARGET:
                    cout << COLOR_GREEN << cell << COLOR_RESET;
                    break;
                case BOX_ON_TARGET:
                    cout << COLOR_BRIGHT_GREEN << COLOR_BOLD << cell << COLOR_RESET;
                    break;
                case WALL:
                    cout << COLOR_GRAY << cell << COLOR_RESET;
                    break;
                case OBSTACLE:
                    cout << COLOR_DARK_GRAY << cell << COLOR_RESET;
                    break;
                default:
                    cout << cell;
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

// Save initial state for restart
void Map::saveInitialState() {
    initial_grid = grid;
    initial_target_positions = target_positions;
}

// Restore initial state
void Map::restoreInitialState() {
    grid = initial_grid;
    target_positions = initial_target_positions;
}

// Add target position to tracking list
void Map::addTarget(int x, int y) {
    target_positions.push_back(make_pair(x, y));
}

// Clear all targets
void Map::clearTargets() {
    target_positions.clear();
}

// Check if position is a target
bool Map::isTarget(int x, int y) const {
    for (const auto& pos : target_positions) {
        if (pos.first == x && pos.second == y) {
            return true;
        }
    }
    return false;
}

// Get total number of targets
int Map::getTargetCount() const {
    return target_positions.size();
}

// Check if position is within map bounds
bool Map::isValidPosition(int x, int y) const {
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT;
}

// Check if cell is walkable (empty or target)
bool Map::isWalkable(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    char cell = grid[y][x];
    return cell == EMPTY || cell == TARGET;
}

// Get map width
int Map::getWidth() const {
    return MAP_WIDTH;
}

// Get map height
int Map::getHeight() const {
    return MAP_HEIGHT;
}

// Get player start X
int Map::getPlayerStartX() const {
    return player_start_x;
}

// Get player start Y
int Map::getPlayerStartY() const {
    return player_start_y;
}

// Set color enabled
void Map::setColorEnabled(bool enabled) {
    color_enabled = enabled;
}

// Check if color is enabled
bool Map::isColorEnabled() const {
    return color_enabled;
}

// Get grid for undo system
vector<vector<char>> Map::getGrid() const {
    return grid;
}

// Set grid from undo system
void Map::setGrid(const vector<vector<char>>& new_grid) {
    grid = new_grid;
}
