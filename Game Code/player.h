#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include <deque>

// Player struct - position and steps
struct Player {
    int row;
    int col;
    int steps;

    Player();
    Player(int start_row, int start_col);
    void resetSteps();
    void incrementSteps();
    int getSteps() const;
    void setPosition(int new_row, int new_col);
};

// Direction constants
const int DIR_UP = 0;
const int DIR_DOWN = 1;
const int DIR_LEFT = 2;
const int DIR_RIGHT = 3;

// Undo state snapshot (dynamic memory)
struct UndoState {
    std::vector<std::vector<char>> grid;
    int player_row;
    int player_col;
    int step_count;

    UndoState() : player_row(0), player_col(0), step_count(0) {}
    UndoState(const std::vector<std::vector<char>> &g, int pr, int pc, int sc)
        : grid(g), player_row(pr), player_col(pc), step_count(sc) {}
};

// Undo system - count limit + state stack
// Limits: Easy=5, Medium=3, Hard=0
// Undo restores position but not steps, consumes one undo
class UndoSystem {
private:
    std::deque<UndoState *> history;
    int undos_left;
    int max_undos;
    bool used_undo;

public:
    UndoSystem(int max_size = 0);
    ~UndoSystem();

    void saveState(const std::vector<std::vector<char>> &grid, const Player &player);
    bool undo(std::vector<std::vector<char>> &grid, Player &player);
    void clear();
    bool canUndo() const;
    int getUndosLeft() const;
    int getMaxUndos() const;
    bool hasUsedUndo() const;
    void reset(int new_max_undos);
};

// Player operations
Player createPlayer(int row, int col);
bool movePlayer(Player &player, int direction,
                std::vector<std::vector<char>> &grid,
                const std::vector<std::pair<int, int>> &target_positions,
                UndoSystem *undo = nullptr);

// Box pushing
bool canPushBox(int box_row, int box_col, int direction,
                const std::vector<std::vector<char>> &grid,
                const std::vector<std::pair<int, int>> &target_positions);
void pushBox(int &box_row, int &box_col, int direction,
             std::vector<std::vector<char>> &grid,
             const std::vector<std::pair<int, int>> &target_positions);

// Input handling
int parseDirection(char input);

// Direction utilities
void getDirectionOffset(int direction, int &drow, int &dcol);

// Reset
void resetPlayer(Player &player, int start_row, int start_col);

#endif // PLAYER_H
