#include "map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// ============================================================
// Difficulty settings (boxes and obstacles per level)
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
// Create fixed test map (for debugging/MVP)
// Contains: border walls, internal walls, 3 boxes, 3 targets, player start
// ============================================================
void initSampleMap(std::vector<std::vector<char>> &grid,
                   int &start_row, int &start_col,
                   std::vector<std::pair<int, int>> &target_positions) {
    grid.assign(MAP_ROWS, std::vector<char>(MAP_COLS, SYMBOL_EMPTY));
    target_positions.clear();

    // Border walls
    for (int c = 0; c < MAP_COLS; ++c) {
        grid[0][c] = SYMBOL_WALL;
        grid[MAP_ROWS - 1][c] = SYMBOL_WALL;
    }
    for (int r = 0; r < MAP_ROWS; ++r) {
        grid[r][0] = SYMBOL_WALL;
        grid[r][MAP_COLS - 1] = SYMBOL_WALL;
    }

    // Internal walls
    grid[2][3] = SYMBOL_WALL;
    grid[2][4] = SYMBOL_WALL;
    grid[3][3] = SYMBOL_WALL;
    grid[5][6] = SYMBOL_WALL;
    grid[5][7] = SYMBOL_WALL;

    // Targets
    grid[4][7] = SYMBOL_TARGET;
    target_positions.push_back({7, 4});
    grid[6][3] = SYMBOL_TARGET;
    target_positions.push_back({3, 6});
    grid[7][7] = SYMBOL_TARGET;
    target_positions.push_back({7, 7});

    // Boxes
    grid[3][4] = SYMBOL_BOX;
    grid[4][5] = SYMBOL_BOX;
    grid[5][3] = SYMBOL_BOX;

    // Player start
    grid[2][2] = SYMBOL_PLAYER;
    start_row = 2;
    start_col = 2;
}

// ============================================================
// Clear map
// ============================================================
void clearMap(std::vector<std::vector<char>> &grid,
              std::vector<std::pair<int, int>> &target_positions) {
    grid.assign(MAP_ROWS, std::vector<char>(MAP_COLS, SYMBOL_EMPTY));
    target_positions.clear();
}

// ============================================================
// Add border walls
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
// Check if position is a corner (bad for boxes/targets)
// A corner is: next to a wall AND next to a wall, OR
//             next to an obstacle AND next to a wall, OR
//             next to an obstacle AND next to an obstacle
// ============================================================
bool isCorner(const std::vector<std::vector<char>> &grid, int x, int y) {
    // Map boundary corners
    if (x <= 1 || x >= MAP_COLS - 2 || y <= 1 || y >= MAP_ROWS - 2) {
        return true;
    }

    // Check for corner against obstacles/walls
    // A box is in a corner if two perpendicular adjacent cells are blocked
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx < 0 || nx >= MAP_COLS || ny < 0 || ny >= MAP_ROWS) continue;

        char cell = grid[ny][nx];
        if (cell != SYMBOL_WALL && cell != SYMBOL_OBSTACLE) continue;

        // Found first blocking cell, check perpendicular directions
        for (int j = 0; j < 4; j++) {
            if (j == i || j == (i ^ 1)) continue;  // Skip opposite direction
            int nnx = x + dx[j];
            int nny = y + dy[j];
            if (nnx < 0 || nnx >= MAP_COLS || nny < 0 || nny >= MAP_ROWS) continue;

            char cell2 = grid[nny][nnx];
            if (cell2 == SYMBOL_WALL || cell2 == SYMBOL_OBSTACLE) {
                return true;  // Corner detected
            }
        }
    }

    return false;
}

// ============================================================
// Check if box position is valid (in bounds, empty, not corner)
// ============================================================
bool isValidBoxPosition(const std::vector<std::vector<char>> &grid, int x, int y) {
    if (x < 0 || x >= MAP_COLS || y < 0 || y >= MAP_ROWS) return false;
    if (grid[y][x] != SYMBOL_EMPTY) return false;
    if (isCorner(grid, x, y)) return false;
    return true;
}

// ============================================================
// Check if target position is valid (in bounds, empty, not corner, not against outer wall)
// ============================================================
bool isValidTargetPosition(const std::vector<std::vector<char>> &grid, int x, int y) {
    if (x < 0 || x >= MAP_COLS || y < 0 || y >= MAP_ROWS) return false;
    if (grid[y][x] != SYMBOL_EMPTY) return false;
    if (isCorner(grid, x, y)) return false;
    if (x == 1 || x == MAP_COLS - 2 || y == 1 || y == MAP_ROWS - 2) return false;
    return true;
}

// Check if cell is passable (can be walked through)
static bool isPassable(char cell) {
    return cell == SYMBOL_EMPTY || cell == SYMBOL_TARGET;
}

// ============================================================
// Count adjacent free cells (for box mobility check)
// Obstacles and walls are impassable
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
            if (isPassable(cell)) {
                count++;
            }
        }
    }
    return count;
}

// ============================================================
// Check box has at least 2 adjacent free cells (ensures mobility)
// ============================================================
bool hasMinimumMobility(const std::vector<std::vector<char>> &grid, int x, int y) {
    return countAdjacentFree(grid, x, y) >= 2;
}

// ============================================================
// Dead Zone Detection (Sokoban Deadlock Detection)
// ============================================================

// Check if cell is blocked (wall or obstacle)
static bool isBlocked(char cell) {
    return cell == SYMBOL_WALL || cell == SYMBOL_OBSTACLE;
}

// Check if a direction is blocked
static bool isDirectionBlocked(const std::vector<std::vector<char>> &grid, int row, int col, int dRow, int dCol) {
    int newRow = row + dRow;
    int newCol = col + dCol;
    if (newRow < 0 || newRow >= MAP_ROWS || newCol < 0 || newCol >= MAP_COLS) {
        return true;
    }
    return isBlocked(grid[newRow][newCol]);
}

// 1. Corner Deadlock: box in corner with walls on two perpendicular sides
// A corner is permanently dead if it's against walls in an L-shape
bool isCornerDeadlock(const std::vector<std::vector<char>> &grid, int box_row, int box_col) {
    // Must be against at least one wall or obstacle
    bool hasBlocker = false;

    // Check all four directions
    // UP, DOWN, LEFT, RIGHT
    bool blocked[4] = {false};

    for (int dir = 0; dir < 4; dir++) {
        int dRow = (dir == 0) ? -1 : (dir == 1) ? 1 : 0;
        int dCol = (dir == 2) ? -1 : (dir == 3) ? 1 : 0;

        if (isDirectionBlocked(grid, box_row, box_col, dRow, dCol)) {
            blocked[dir] = true;
            hasBlocker = true;
        }
    }

    if (!hasBlocker) {
        return false;  // Not against any wall, not a corner deadlock
    }

    // Corner deadlock: walls in two perpendicular directions
    // UP + LEFT, UP + RIGHT, DOWN + LEFT, DOWN + RIGHT
    if (blocked[0] && blocked[2]) return true;  // UP + LEFT
    if (blocked[0] && blocked[3]) return true;  // UP + RIGHT
    if (blocked[1] && blocked[2]) return true;  // DOWN + LEFT
    if (blocked[1] && blocked[3]) return true;  // DOWN + RIGHT

    return false;
}

// 2. Single-Side Wall Deadlock: box against wall but can only be pushed one way
// If box is in a straight line against a wall and can't be pushed sideways
bool isWallDeadlock(const std::vector<std::vector<char>> &grid, int box_row, int box_col) {
    // Check if box is against a wall/obstacle
    int blockingCount = 0;
    int blockingDirs[2] = {-1, -1};

    for (int dir = 0; dir < 4; dir++) {
        int dRow = (dir == 0) ? -1 : (dir == 1) ? 1 : 0;
        int dCol = (dir == 2) ? -1 : (dir == 3) ? 1 : 0;

        if (isDirectionBlocked(grid, box_row, box_col, dRow, dCol)) {
            if (blockingCount < 2) {
                blockingDirs[blockingCount] = dir;
            }
            blockingCount++;
        }
    }

    if (blockingCount != 2) {
        return false;  // Need exactly 2 blockers for wall deadlock
    }

    // Check if the two blockers are opposite directions (parallel to wall)
    // This means the box can only move in one direction
    if ((blockingDirs[0] == 0 && blockingDirs[1] == 1) ||
        (blockingDirs[0] == 1 && blockingDirs[1] == 0) ||
        (blockingDirs[0] == 2 && blockingDirs[1] == 3) ||
        (blockingDirs[0] == 3 && blockingDirs[1] == 2)) {
        // Box is sandwiched between two walls - it's in a corridor
        // This is NOT a wall deadlock unless it's at the end of the corridor
        return false;
    }

    // Two perpendicular blockers - box is in a corner-like position
    // Check if it's actually a valid corner (box can still be pushed)
    // A wall deadlock occurs when box is against a single wall and can't move sideways
    // For simplicity: if box has only 2 adjacent free cells and is against a wall
    int freeCount = countAdjacentFree(grid, box_col, box_row);
    if (freeCount == 1 && blockingCount == 1) {
        return true;  // Can only move in one direction, stuck at wall
    }

    return false;
}

// 3. Dead-End Alley Single-Row Deadlock: box at end of dead-end corridor
// Check if box is in a 1-cell wide corridor that's a dead end
bool isDeadEndDeadlock(const std::vector<std::vector<char>> &grid, int box_row, int box_col) {
    // A dead-end is a 1-cell wide corridor where the box is at the blind end
    // The corridor must have walls on both sides perpendicular to its direction

    // Check if this is a horizontal corridor
    bool leftBlocked = isDirectionBlocked(grid, box_row, box_col, 0, -1);
    bool rightBlocked = isDirectionBlocked(grid, box_row, box_col, 0, 1);
    bool upBlocked = isDirectionBlocked(grid, box_row, box_col, -1, 0);
    bool downBlocked = isDirectionBlocked(grid, box_row, box_col, 1, 0);

    // Horizontal dead-end: walls above and below, open on one side, blocked on other
    if (upBlocked && downBlocked) {
        // Horizontal corridor
        int freeLeft = 0, freeRight = 0;
        // Check how far we can go left/right while staying in corridor
        int leftCol = box_col - 1;
        while (leftCol > 0 && !isBlocked(grid[box_row][leftCol])) {
            // Check if still in corridor
            if (isDirectionBlocked(grid, box_row, leftCol, -1, 0) &&
                isDirectionBlocked(grid, box_row, leftCol, 1, 0)) {
                freeLeft++;
            } else {
                break;
            }
            leftCol--;
        }

        int rightCol = box_col + 1;
        while (rightCol < MAP_COLS - 1 && !isBlocked(grid[box_row][rightCol])) {
            if (isDirectionBlocked(grid, box_row, rightCol, -1, 0) &&
                isDirectionBlocked(grid, box_row, rightCol, 1, 0)) {
                freeRight++;
            } else {
                break;
            }
            rightCol++;
        }

        // Dead-end if one side open, other side closed
        bool leftOpen = leftCol > 0 && !isBlocked(grid[box_row][leftCol]);
        bool rightOpen = rightCol < MAP_COLS - 1 && !isBlocked(grid[box_row][rightCol]);

        if ((leftOpen && !rightOpen) || (!leftOpen && rightOpen)) {
            // Box is at the blind end of a corridor
            // Check if the box can be pulled back (player can get behind it)
            int pullDir = leftOpen ? -1 : 1;
            int behindRow = box_row;
            int behindCol = box_col + pullDir;  // Direction to pull from

            // The player needs to be on the opposite side to pull
            // This is a simplified check - actual implementation may need player position
            return true;
        }
    }

    // Vertical dead-end
    bool leftBlockedV = isDirectionBlocked(grid, box_row, box_col, 0, -1);
    bool rightBlockedV = isDirectionBlocked(grid, box_row, box_col, 0, 1);
    bool upBlockedV = isDirectionBlocked(grid, box_row, box_col, -1, 0);
    bool downBlockedV = isDirectionBlocked(grid, box_row, box_col, 1, 0);

    if (leftBlockedV && rightBlockedV) {
        // Vertical corridor
        int freeUp = 0, freeDown = 0;
        int upRow = box_row - 1;
        while (upRow > 0 && !isBlocked(grid[upRow][box_col])) {
            if (isDirectionBlocked(grid, upRow, box_col, 0, -1) &&
                isDirectionBlocked(grid, upRow, box_col, 0, 1)) {
                freeUp++;
            } else {
                break;
            }
            upRow--;
        }

        int downRow = box_row + 1;
        while (downRow < MAP_ROWS - 1 && !isBlocked(grid[downRow][box_col])) {
            if (isDirectionBlocked(grid, downRow, box_col, 0, -1) &&
                isDirectionBlocked(grid, downRow, box_col, 0, 1)) {
                freeDown++;
            } else {
                break;
            }
            downRow++;
        }

        bool upOpen = upRow > 0 && !isBlocked(grid[upRow][box_col]);
        bool downOpen = downRow < MAP_ROWS - 1 && !isBlocked(grid[downRow][box_col]);

        if ((upOpen && !downOpen) || (!upOpen && downOpen)) {
            return true;
        }
    }

    return false;
}

// 4. Multi-Box Interlocking Deadlock: boxes blocking each other
// Two boxes in a narrow channel, neither can move
bool isMultiBoxDeadlock(const std::vector<std::vector<char>> &grid, int box_row, int box_col) {
    // Check for horizontal pair
    if (box_col > 0 && grid[box_row][box_col - 1] == SYMBOL_BOX) {
        // Box to the left
        int behindCol = box_col + 1;  // Direction we'd push from
        if (behindCol < MAP_COLS - 1) {
            char behind = grid[box_row][behindCol];
            if (isBlocked(behind)) {
                // Both boxes blocked in horizontal channel
                // Check if vertical escape exists for either
                bool box1CanEscape = !isDirectionBlocked(grid, box_row, box_col, -1, 0) ||
                                     !isDirectionBlocked(grid, box_row, box_col, 1, 0);
                bool box2CanEscape = !isDirectionBlocked(grid, box_row, box_col - 1, -1, 0) ||
                                    !isDirectionBlocked(grid, box_row, box_col - 1, 1, 0);
                if (!box1CanEscape && !box2CanEscape) {
                    return true;
                }
            }
        }
    }

    if (box_col < MAP_COLS - 1 && grid[box_row][box_col + 1] == SYMBOL_BOX) {
        // Box to the right
        int behindCol = box_col - 1;  // Direction we'd push from
        if (behindCol > 0) {
            char behind = grid[box_row][behindCol];
            if (isBlocked(behind)) {
                bool box1CanEscape = !isDirectionBlocked(grid, box_row, box_col, -1, 0) ||
                                     !isDirectionBlocked(grid, box_row, box_col, 1, 0);
                bool box2CanEscape = !isDirectionBlocked(grid, box_row, box_col + 1, -1, 0) ||
                                    !isDirectionBlocked(grid, box_row, box_col + 1, 1, 0);
                if (!box1CanEscape && !box2CanEscape) {
                    return true;
                }
            }
        }
    }

    // Check for vertical pair
    if (box_row > 0 && grid[box_row - 1][box_col] == SYMBOL_BOX) {
        // Box above
        int behindRow = box_row + 1;
        if (behindRow < MAP_ROWS - 1) {
            char behind = grid[behindRow][box_col];
            if (isBlocked(behind)) {
                bool box1CanEscape = !isDirectionBlocked(grid, box_row, box_col, 0, -1) ||
                                     !isDirectionBlocked(grid, box_row, box_col, 0, 1);
                bool box2CanEscape = !isDirectionBlocked(grid, box_row - 1, box_col, 0, -1) ||
                                    !isDirectionBlocked(grid, box_row - 1, box_col, 0, 1);
                if (!box1CanEscape && !box2CanEscape) {
                    return true;
                }
            }
        }
    }

    if (box_row < MAP_ROWS - 1 && grid[box_row + 1][box_col] == SYMBOL_BOX) {
        // Box below
        int behindRow = box_row - 1;
        if (behindRow > 0) {
            char behind = grid[behindRow][box_col];
            if (isBlocked(behind)) {
                bool box1CanEscape = !isDirectionBlocked(grid, box_row, box_col, 0, -1) ||
                                     !isDirectionBlocked(grid, box_row, box_col, 0, 1);
                bool box2CanEscape = !isDirectionBlocked(grid, box_row + 1, box_col, 0, -1) ||
                                    !isDirectionBlocked(grid, box_row + 1, box_col, 0, 1);
                if (!box1CanEscape && !box2CanEscape) {
                    return true;
                }
            }
        }
    }

    return false;
}

// Main deadlock detection function
// Returns true if the box at (box_row, box_col) is in a permanent deadlock
bool isDeadlocked(const std::vector<std::vector<char>> &grid,
                  const std::vector<std::pair<int, int>> &target_positions,
                  int box_row, int box_col) {
    // If box is already on a target, it's not deadlocked
    if (isTarget(target_positions, box_col, box_row)) {
        return false;
    }

    // 1. Corner deadlock: box in corner with walls on two sides
    if (isCornerDeadlock(grid, box_row, box_col)) {
        return true;
    }

    // 2. Wall deadlock: box against wall but stuck
    if (isWallDeadlock(grid, box_row, box_col)) {
        return true;
    }

    // 3. Dead-end alley deadlock
    if (isDeadEndDeadlock(grid, box_row, box_col)) {
        return true;
    }

    // 4. Multi-box interlocking
    if (isMultiBoxDeadlock(grid, box_row, box_col)) {
        return true;
    }

    return false;
}

// ============================================================
// Randomly place player (find position in inner empty area)
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
// Randomly place boxes and targets (paired, ensure mobility)
// ============================================================
bool placeBoxesAndTargets(std::vector<std::vector<char>> &grid,
                          std::vector<std::pair<int, int>> &target_positions, int count) {
    int placed = 0;
    int max_attempts = 1000;

    while (placed < count && max_attempts > 0) {
        max_attempts--;

        int box_x = 2 + rand() % (MAP_COLS - 4);
        int box_y = 2 + rand() % (MAP_ROWS - 4);

        if (!isValidBoxPosition(grid, box_x, box_y)) continue;

        int target_x = 2 + rand() % (MAP_COLS - 4);
        int target_y = 2 + rand() % (MAP_ROWS - 4);

        if (!isValidTargetPosition(grid, target_x, target_y)) continue;
        if (target_x == box_x && target_y == box_y) continue;

        grid[box_y][box_x] = SYMBOL_BOX;

        if (!hasMinimumMobility(grid, box_x, box_y)) {
            grid[box_y][box_x] = SYMBOL_EMPTY;
            continue;
        }

        grid[target_y][target_x] = SYMBOL_TARGET;
        target_positions.push_back({target_x, target_y});

        placed++;
    }

    return placed == count;
}

// ============================================================
// Randomly place obstacles (avoid blocking paths)
// ============================================================
bool placeObstacles(std::vector<std::vector<char>> &grid, int count) {
    int placed = 0;
    int max_attempts = 500;

    while (placed < count && max_attempts > 0) {
        max_attempts--;

        int x = 2 + rand() % (MAP_COLS - 4);
        int y = 2 + rand() % (MAP_ROWS - 4);

        if (grid[y][x] != SYMBOL_EMPTY) continue;

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

        if (adjacent_walls >= 2) continue;

        grid[y][x] = SYMBOL_OBSTACLE;
        placed++;
    }

    return placed >= count / 2;
}

// ============================================================
// Generate random map by difficulty
// Flow: clear -> border -> obstacles -> boxes/targets -> player
// Fallback to fixed test map if 50 retries fail
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

        int num_obstacles = settings.min_obstacles;
        if (settings.max_obstacles > settings.min_obstacles) {
            num_obstacles = settings.min_obstacles +
                          rand() % (settings.max_obstacles - settings.min_obstacles + 1);
        }

        if (num_obstacles > 0) {
            placeObstacles(grid, num_obstacles);
        }

        if (!placeBoxesAndTargets(grid, target_positions, settings.num_boxes)) {
            continue;
        }

        if (!placePlayer(grid, start_row, start_col)) {
            continue;
        }

        valid_map = true;
    }

    if (!valid_map) {
        initSampleMap(grid, start_row, start_col, target_positions);
        return;
    }
}

// ============================================================
// Print map to terminal (ANSI colors)
// Note: target_positions needed to check if box is on target
// Box on target shows '*' (bright green), else '$' (red)
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
                std::cout << cell;
                continue;
            }

            if (r == player_row && c == player_col) {
                std::cout << COLOR_YELLOW << COLOR_BOLD << cell << COLOR_RESET;
            } else if (cell == SYMBOL_BOX) {
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

// Check if position is within map bounds
bool isValidPosition(int row, int col) {
    return row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS;
}

// Safe get cell: return wall if out of bounds
char getCell(const std::vector<std::vector<char>> &grid, int row, int col) {
    if (isValidPosition(row, col)) {
        return grid[row][col];
    }
    return SYMBOL_WALL;
}

// Safe set cell: only modify if position is valid
void setCell(std::vector<std::vector<char>> &grid, int row, int col, char value) {
    if (isValidPosition(row, col)) {
        grid[row][col] = value;
    }
}

// ============================================================
// Target management functions
// ============================================================

// Check if position is a target
bool isTarget(const std::vector<std::pair<int, int>> &target_positions, int x, int y) {
    for (const auto &pos : target_positions) {
        if (pos.first == x && pos.second == y) {
            return true;
        }
    }
    return false;
}

// Get total target count
int getTargetCount(const std::vector<std::pair<int, int>> &target_positions) {
    return target_positions.size();
}

// Count targets with boxes on them
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

// Check if position has box on target
bool isBoxOnTarget(const std::vector<std::pair<int, int>> &target_positions,
                   int row, int col) {
    return isTarget(target_positions, col, row);
}

// ============================================================
// Save initial game state for level reset (R key)
// Deep copy: grid, targets, player position, steps
// ============================================================
GameState saveInitialState(const std::vector<std::vector<char>> &grid,
                           const std::vector<std::pair<int, int>> & /* target_positions */,
                           int player_row, int player_col) {
    GameState state;
    state.grid = grid;
    state.player_row = player_row;
    state.player_col = player_col;
    state.step_count = 0;
    return state;
}

// ============================================================
// Restore level from saved initial state
// ============================================================
void restoreInitialState(std::vector<std::vector<char>> &grid,
                         std::vector<std::pair<int, int>> & /* target_positions */,
                         int &player_row, int &player_col, int &step_count,
                         const GameState &initial_state) {
    grid = initial_state.grid;
    player_row = initial_state.player_row;
    player_col = initial_state.player_col;
    step_count = initial_state.step_count;
}
