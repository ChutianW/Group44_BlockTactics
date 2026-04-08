#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include <stack>

// Player structure
struct Player {
    int x;
    int y;
    int step_count;

    // Constructor
    Player();
    Player(int start_x, int start_y);

    // Reset step count
    void resetSteps();

    // Increment step count
    void incrementSteps();

    // Get current step count
    int getSteps() const;

    // Reset position
    void setPosition(int new_x, int new_y);
};

// Movement directions
enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Game state for undo system (Dynamic Memory feature)
struct GameState {
    vector<vector<char>> grid;
    int player_x;
    int player_y;
    int step_count;

    GameState() : player_x(0), player_y(0), step_count(0) {}
    GameState(const vector<vector<char>>& g, int px, int py, int steps)
        : grid(g), player_x(px), player_y(py), step_count(steps) {}
};

// Undo system class using dynamic memory (stack)
class UndoSystem {
private:
    stack<GameState*> history;  // Using pointers for dynamic memory
    int max_history;

public:
    UndoSystem(int max_size = 100);
    ~UndoSystem();

    void saveState(const Map& game_map, const Player& player);
    bool undo(Map& game_map, Player& player);
    void clear();
    bool canUndo() const;
    int getHistorySize() const;
};

// Movement function declarations
bool movePlayer(Player& player, Map& game_map, Direction dir, UndoSystem* undo = nullptr);
bool canMove(const Player& player, const Map& game_map, Direction dir);
bool canPushBox(const Map& game_map, int box_x, int box_y, Direction dir);

// Get direction offsets
void getDirectionOffset(Direction dir, int& dx, int& dy);

#endif // PLAYER_H
