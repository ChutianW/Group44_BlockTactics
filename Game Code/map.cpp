#include "map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

/*
 * Function: getDifficultySettings
 * Purpose:  Returns the box count and obstacle range for a given difficulty level.
 * Inputs:   diff - the Difficulty enum value (EASY, MEDIUM, or HARD).
 * Output:   Returns a DifficultySettings struct with num_boxes, min_obstacles,
 *           and max_obstacles set according to the difficulty.
 */
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
            settings.min_obstacles = 5;
            settings.max_obstacles = 7;
            break;
        default:
            settings.num_boxes = 3;
            settings.min_obstacles = 0;
            settings.max_obstacles = 0;
    }
    return settings;
}

/*
 * Function: initSampleMap
 * Purpose:  Initializes a fixed, hardcoded test map used as a fallback when
 *           random map generation fails after 50 attempts.
 *           Places border walls, internal walls, 3 boxes, 3 targets, and the player.
 * Inputs:   grid             - the 2D grid to populate (resized to MAP_ROWS x MAP_COLS).
 *           start_row        - output set to the player's starting row.
 *           start_col        - output set to the player's starting column.
 *           target_positions - output vector filled with target (x,y) positions.
 * Output:   Fills grid with the sample map layout; sets start_row, start_col,
 *           and target_positions.
 */
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

/*
 * Function: clearMap
 * Purpose:  Resets the grid to all-empty cells and clears the target position list.
 *           Called at the start of each random map generation attempt.
 * Inputs:   grid             - the 2D grid to reset to SYMBOL_EMPTY.
 *           target_positions - the target list to clear.
 * Output:   Sets all grid cells to SYMBOL_EMPTY; empties target_positions.
 */
void clearMap(std::vector<std::vector<char>> &grid,
              std::vector<std::pair<int, int>> &target_positions) {
    grid.assign(MAP_ROWS, std::vector<char>(MAP_COLS, SYMBOL_EMPTY));
    target_positions.clear();
}

/*
 * Function: addBorderWalls
 * Purpose:  Fills the outermost row and column of the grid with SYMBOL_WALL
 *           to create an impassable border around the playfield.
 * Inputs:   grid - the 2D grid to modify.
 * Output:   Sets row 0, row MAP_ROWS-1, col 0, and col MAP_COLS-1 to SYMBOL_WALL.
 */
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

/*
 * Function: isCorner
 * Purpose:  Determines whether a grid position is a "corner" where a box would
 *           be permanently stuck. A corner is detected when two perpendicular
 *           adjacent cells are both walls or obstacles, or when near the map edge.
 * Inputs:   grid - the current 2D grid.
 *           x    - column index of the position to check.
 *           y    - row index of the position to check.
 * Output:   Returns true if the position is a corner deadlock site; false otherwise.
 */
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

/*
 * Function: isValidBoxPosition
 * Purpose:  Checks whether a position is suitable for placing a box during
 *           map generation. A valid box position must be in bounds, currently
 *           empty, and not a corner (to avoid immediate deadlocks).
 * Inputs:   grid - the current 2D grid.
 *           x    - column index to check.
 *           y    - row index to check.
 * Output:   Returns true if the position is valid for a box; false otherwise.
 */
bool isValidBoxPosition(const std::vector<std::vector<char>> &grid, int x, int y) {
    if (x < 0 || x >= MAP_COLS || y < 0 || y >= MAP_ROWS) return false;
    if (grid[y][x] != SYMBOL_EMPTY) return false;
    if (isCorner(grid, x, y)) return false;
    return true;
}

/*
 * Function: isValidTargetPosition
 * Purpose:  Checks whether a position is suitable for placing a target during
 *           map generation. A valid target must be in bounds, empty, not a corner,
 *           and not adjacent to the outermost wall row/column.
 * Inputs:   grid - the current 2D grid.
 *           x    - column index to check.
 *           y    - row index to check.
 * Output:   Returns true if the position is valid for a target; false otherwise.
 */
bool isValidTargetPosition(const std::vector<std::vector<char>> &grid, int x, int y) {
    if (x < 0 || x >= MAP_COLS || y < 0 || y >= MAP_ROWS) return false;
    if (grid[y][x] != SYMBOL_EMPTY) return false;
    if (isCorner(grid, x, y)) return false;
    if (x == 1 || x == MAP_COLS - 2 || y == 1 || y == MAP_ROWS - 2) return false;
    return true;
}

/*
 * Function: isPassable
 * Purpose:  Checks whether a grid cell symbol represents a passable tile
 *           (empty space or target) that a player or box can occupy.
 * Inputs:   cell - the character symbol from the grid.
 * Output:   Returns true if cell is SYMBOL_EMPTY or SYMBOL_TARGET; false otherwise.
 */
static bool isPassable(char cell) {
    return cell == SYMBOL_EMPTY || cell == SYMBOL_TARGET;
}

/*
 * Function: countAdjacentFree
 * Purpose:  Counts how many of the four orthogonal neighbours of a grid position
 *           are passable (empty or target). Used to ensure boxes have mobility.
 * Inputs:   grid - the current 2D grid.
 *           x    - column index of the cell.
 *           y    - row index of the cell.
 * Output:   Returns an integer from 0 to 4 representing passable neighbour count.
 */
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

/*
 * Function: hasMinimumMobility
 * Purpose:  Checks that a newly placed box has at least 2 passable neighbours,
 *           ensuring it is not immediately stuck and can be moved in some direction.
 * Inputs:   grid - the current 2D grid.
 *           x    - column index of the box.
 *           y    - row index of the box.
 * Output:   Returns true if the box has 2 or more free adjacent cells; false if stuck.
 */
bool hasMinimumMobility(const std::vector<std::vector<char>> &grid, int x, int y) {
    return countAdjacentFree(grid, x, y) >= 2;
}

// ============================================================
// Dead Zone Detection (Sokoban Deadlock Detection)
// ============================================================

/*
 * Function: isBlocked
 * Purpose:  Checks whether a grid cell symbol is an impassable blocker
 *           (wall or obstacle) for deadlock detection purposes.
 * Inputs:   cell - the character symbol from the grid.
 * Output:   Returns true if cell is SYMBOL_WALL or SYMBOL_OBSTACLE; false otherwise.
 */
static bool isBlocked(char cell) {
    return cell == SYMBOL_WALL || cell == SYMBOL_OBSTACLE;
}

/*
 * Function: isDirectionBlocked
 * Purpose:  Checks whether a step in the given direction from (row, col) leads
 *           to a blocked cell (wall, obstacle) or is out of map bounds.
 * Inputs:   grid - the current 2D grid.
 *           row  - starting row index.
 *           col  - starting column index.
 *           dRow - row delta of the direction to check.
 *           dCol - column delta of the direction to check.
 * Output:   Returns true if the target cell is blocked or out of bounds; false otherwise.
 */
static bool isDirectionBlocked(const std::vector<std::vector<char>> &grid, int row, int col, int dRow, int dCol) {
    int newRow = row + dRow;
    int newCol = col + dCol;
    if (newRow < 0 || newRow >= MAP_ROWS || newCol < 0 || newCol >= MAP_COLS) {
        return true;
    }
    return isBlocked(grid[newRow][newCol]);
}

/*
 * Function: isCornerDeadlock
 * Purpose:  Detects a corner deadlock: the box is blocked in two perpendicular
 *           directions (UP+LEFT, UP+RIGHT, DOWN+LEFT, or DOWN+RIGHT), meaning it
 *           can never be pushed out without passing through a wall.
 * Inputs:   grid    - the current 2D grid.
 *           box_row - row index of the box to check.
 *           box_col - column index of the box to check.
 * Output:   Returns true if the box is in a corner deadlock position; false otherwise.
 */
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

/*
 * Function: isWallDeadlock
 * Purpose:  Detects a single-wall deadlock where the box is blocked on exactly
 *           two perpendicular sides and has insufficient free neighbours to escape.
 *           A box sandwiched between two opposite walls (corridor) is not flagged.
 * Inputs:   grid    - the current 2D grid.
 *           box_row - row index of the box to check.
 *           box_col - column index of the box to check.
 * Output:   Returns true if the box is in a wall deadlock position; false otherwise.
 */
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

/*
 * Function: isDeadEndDeadlock
 * Purpose:  Detects a dead-end alley deadlock: the box is in a 1-cell wide corridor
 *           (walls on both perpendicular sides) that is open on one end and closed
 *           on the other, making the box permanently stuck at the blind end.
 *           Checks both horizontal and vertical corridors.
 * Inputs:   grid    - the current 2D grid.
 *           box_row - row index of the box to check.
 *           box_col - column index of the box to check.
 * Output:   Returns true if the box is at the dead end of an alley; false otherwise.
 */
bool isDeadEndDeadlock(const std::vector<std::vector<char>> &grid, int box_row, int box_col) {
    // A dead-end is a 1-cell wide corridor where the box is at the blind end
    // The corridor must have walls on both sides perpendicular to its direction

    // Check if this is a horizontal corridor
    bool upBlocked = isDirectionBlocked(grid, box_row, box_col, -1, 0);
    bool downBlocked = isDirectionBlocked(grid, box_row, box_col, 1, 0);

    // Horizontal dead-end: walls above and below, open on one side, blocked on other
    if (upBlocked && downBlocked) {
        // Horizontal corridor
        int leftCol = box_col - 1;
        while (leftCol > 0 && !isBlocked(grid[box_row][leftCol])) {
            if (isDirectionBlocked(grid, box_row, leftCol, -1, 0) &&
                isDirectionBlocked(grid, box_row, leftCol, 1, 0)) {
            } else {
                break;
            }
            leftCol--;
        }

        int rightCol = box_col + 1;
        while (rightCol < MAP_COLS - 1 && !isBlocked(grid[box_row][rightCol])) {
            if (isDirectionBlocked(grid, box_row, rightCol, -1, 0) &&
                isDirectionBlocked(grid, box_row, rightCol, 1, 0)) {
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
            // TODO: Verify the box can actually be pulled back by checking player
            // position relative to the corridor opening (pullDir = leftOpen ? -1 : 1).
            // Current implementation conservatively flags this as a deadlock.
            return true;
        }
    }

    // Vertical dead-end
    bool leftBlockedV = isDirectionBlocked(grid, box_row, box_col, 0, -1);
    bool rightBlockedV = isDirectionBlocked(grid, box_row, box_col, 0, 1);

    if (leftBlockedV && rightBlockedV) {
        // Vertical corridor
        int upRow = box_row - 1;
        while (upRow > 0 && !isBlocked(grid[upRow][box_col])) {
            if (isDirectionBlocked(grid, upRow, box_col, 0, -1) &&
                isDirectionBlocked(grid, upRow, box_col, 0, 1)) {
            } else {
                break;
            }
            upRow--;
        }

        int downRow = box_row + 1;
        while (downRow < MAP_ROWS - 1 && !isBlocked(grid[downRow][box_col])) {
            if (isDirectionBlocked(grid, downRow, box_col, 0, -1) &&
                isDirectionBlocked(grid, downRow, box_col, 0, 1)) {
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

/*
 * Function: isMultiBoxDeadlock
 * Purpose:  Detects a multi-box interlocking deadlock: two adjacent boxes are
 *           packed into a narrow channel where neither can escape vertically or
 *           horizontally. Checks all four neighbor directions for paired boxes.
 * Inputs:   grid    - the current 2D grid.
 *           box_row - row index of the reference box.
 *           box_col - column index of the reference box.
 * Output:   Returns true if the box is part of an interlocking deadlock; false otherwise.
 */
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

/*
 * Function: isDeadlocked
 * Purpose:  Master deadlock detection function that runs all four deadlock checks
 *           (corner, wall, dead-end, multi-box) on a given box position.
 *           Boxes already on a target are never considered deadlocked.
 * Inputs:   grid             - the current 2D grid.
 *           target_positions - list of target positions to skip already-solved boxes.
 *           box_row          - row index of the box to evaluate.
 *           box_col          - column index of the box to evaluate.
 * Output:   Returns true if any deadlock condition is detected; false if box is free.
 */
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

/*
 * Function: placePlayer
 * Purpose:  Randomly places the player symbol on an empty interior cell of the grid.
 *           Attempts up to 100 random positions before giving up.
 * Inputs:   grid      - the 2D grid (modified with SYMBOL_PLAYER on success).
 *           start_row - output set to the player's placed row index.
 *           start_col - output set to the player's placed column index.
 * Output:   Returns true and updates grid, start_row, start_col if placement succeeds.
 *           Returns false if no empty cell was found in 100 attempts.
 */
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

/*
 * Function: placeBoxesAndTargets
 * Purpose:  Randomly places the given number of box/target pairs on the grid.
 *           Each box is validated for position and minimum mobility before placement.
 *           Each target is placed at a separate valid location.
 *           Up to 1000 combined attempts are made before giving up.
 * Inputs:   grid             - the 2D grid (modified in place).
 *           target_positions - output vector receiving placed target (x,y) positions.
 *           count            - number of box/target pairs to place.
 * Output:   Returns true if exactly count pairs were placed; false otherwise.
 */
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

/*
 * Function: placeObstacles
 * Purpose:  Randomly places a given number of obstacle symbols on empty interior
 *           cells. Avoids cells already adjacent to 2 or more walls/obstacles
 *           to prevent path-blocking clusters. Up to 500 attempts are made.
 * Inputs:   grid  - the 2D grid (modified in place).
 *           count - the desired number of obstacles to place.
 * Output:   Returns true if at least count/2 obstacles were placed; false otherwise.
 */
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

/*
 * Function: generateRandomMap
 * Purpose:  Generates a complete random playable map for the given difficulty.
 *           Flow per attempt: clear grid -> add border walls -> place obstacles
 *           -> place boxes and targets -> place player.
 *           Falls back to initSampleMap() if 50 generation attempts all fail.
 * Inputs:   grid             - the 2D grid to populate.
 *           start_row        - output set to the player's starting row.
 *           start_col        - output set to the player's starting column.
 *           target_positions - output vector filled with target positions.
 *           diff             - the Difficulty determining box and obstacle counts.
 * Output:   Fills grid, start_row, start_col, and target_positions with a valid map.
 */
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

/*
 * Function: printMap
 * Purpose:  Prints the full game grid directly to stdout with optional ANSI colors.
 *           This is the standalone (non-class) version used for legacy/debug output.
 *           Boxes on targets render as bright green; boxes off targets as red.
 * Inputs:   grid             - the 2D grid to render.
 *           target_positions - target positions for box-on-target color detection.
 *           player_row       - current row of the player.
 *           player_col       - current column of the player.
 *           color_enabled    - whether to apply ANSI color codes.
 * Output:   Prints the 10x10 grid to stdout with two-space indent per row.
 */
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

/*
 * Function: isValidPosition
 * Purpose:  Checks whether a (row, col) pair falls within the valid map grid bounds.
 * Inputs:   row - the row index to validate.
 *           col - the column index to validate.
 * Output:   Returns true if 0 <= row < MAP_ROWS and 0 <= col < MAP_COLS; false otherwise.
 */
bool isValidPosition(int row, int col) {
    return row >= 0 && row < MAP_ROWS && col >= 0 && col < MAP_COLS;
}

/*
 * Function: getCell
 * Purpose:  Safely retrieves the character at a grid position.
 *           Returns SYMBOL_WALL if the position is out of bounds.
 * Inputs:   grid - the 2D grid to query.
 *           row  - row index to access.
 *           col  - column index to access.
 * Output:   Returns the grid cell character, or SYMBOL_WALL if out of bounds.
 */
char getCell(const std::vector<std::vector<char>> &grid, int row, int col) {
    if (isValidPosition(row, col)) {
        return grid[row][col];
    }
    return SYMBOL_WALL;
}

/*
 * Function: setCell
 * Purpose:  Safely sets the character at a grid position.
 *           Does nothing if the position is out of bounds.
 * Inputs:   grid  - the 2D grid to modify.
 *           row   - row index of the cell to set.
 *           col   - column index of the cell to set.
 *           value - the character value to write.
 * Output:   Updates grid[row][col] to value if position is valid; no-op otherwise.
 */
void setCell(std::vector<std::vector<char>> &grid, int row, int col, char value) {
    if (isValidPosition(row, col)) {
        grid[row][col] = value;
    }
}

// ============================================================
// Target management functions
// ============================================================

/*
 * Function: isTarget
 * Purpose:  Checks whether a given (x, y) grid position is a target location.
 * Inputs:   target_positions - the list of target (x,y) coordinate pairs.
 *           x                - column index to check.
 *           y                - row index to check.
 * Output:   Returns true if (x, y) matches any entry in target_positions; false otherwise.
 */
bool isTarget(const std::vector<std::pair<int, int>> &target_positions, int x, int y) {
    for (const auto &pos : target_positions) {
        if (pos.first == x && pos.second == y) {
            return true;
        }
    }
    return false;
}

/*
 * Function: getTargetCount
 * Purpose:  Returns the total number of targets on the current map.
 * Inputs:   target_positions - the list of target positions.
 * Output:   Returns the size of the target_positions vector as an integer.
 */
int getTargetCount(const std::vector<std::pair<int, int>> &target_positions) {
    return target_positions.size();
}

/*
 * Function: countCompletedTargets
 * Purpose:  Counts how many target positions currently have a box (SYMBOL_BOX) on them.
 *           Used to track win condition progress and display the status bar.
 * Inputs:   grid             - the current 2D game grid.
 *           target_positions - list of all target (x,y) positions.
 * Output:   Returns the number of targets with a box on them (0 to total targets).
 */
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

/*
 * Function: isBoxOnTarget
 * Purpose:  Checks whether the cell at (row, col) is a target position.
 *           A convenience wrapper around isTarget() with swapped coordinate order.
 * Inputs:   target_positions - list of all target positions.
 *           row              - row index of the cell to check.
 *           col              - column index of the cell to check.
 * Output:   Returns true if (col, row) is a target; false otherwise.
 */
bool isBoxOnTarget(const std::vector<std::pair<int, int>> &target_positions,
                   int row, int col) {
    return isTarget(target_positions, col, row);
}

/*
 * Function: saveInitialState
 * Purpose:  Takes a deep copy snapshot of the grid and player position at level
 *           start so the level can be fully restored when the player presses R.
 * Inputs:   grid             - the initial 2D grid to snapshot.
 *           target_positions - unused (reserved for future target restoration).
 *           player_row       - the player's starting row.
 *           player_col       - the player's starting column.
 * Output:   Returns a GameState with a copy of grid, player position, and step_count=0.
 */
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

/*
 * Function: restoreInitialState
 * Purpose:  Overwrites the current game state with the saved initial snapshot,
 *           effectively resetting the level to its starting configuration.
 * Inputs:   grid             - the 2D grid to overwrite from the snapshot.
 *           target_positions - unused (reserved for future target restoration).
 *           player_row       - output set to the player's initial row.
 *           player_col       - output set to the player's initial column.
 *           step_count       - output set to the initial step count (0).
 *           initial_state    - the GameState snapshot created by saveInitialState().
 * Output:   Updates grid, player_row, player_col, and step_count from the snapshot.
 */
void restoreInitialState(std::vector<std::vector<char>> &grid,
                         std::vector<std::pair<int, int>> & /* target_positions */,
                         int &player_row, int &player_col, int &step_count,
                         const GameState &initial_state) {
    grid = initial_state.grid;
    player_row = initial_state.player_row;
    player_col = initial_state.player_col;
    step_count = initial_state.step_count;
}
