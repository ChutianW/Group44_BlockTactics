#ifndef GAME_H
#define GAME_H

#include "map.h"
#include "player.h"
#include "renderer.h"
#include <vector>

// Forward declaration
struct UserData;
struct LeaderboardEntry;

// ============================================================================
// Game class - encapsulates core game logic (ENCAPSULATION)
// Responsible for: game state management, level generation, input processing,
//                  win/lose detection
// Uses composition: holds Renderer, Player, UndoSystem objects
// ============================================================================
class Game {
public:
    virtual ~Game();  // Virtual destructor for polymorphism

    // === Game Flow Control ===
    void run();
    void startGame();  // Public method to start a new game (calls initLevel)
    void restart();
    void nextLevel();
    void setUserData(UserData *user);

    // === State Query ===
    bool isRunning() const;
    bool isLevelComplete() const;
    Difficulty getDifficulty() const;

    // === Polymorphic hooks (can be overridden by subclasses) ===
    virtual int getUndoLimit() const;
    virtual int getBoxCount() const;
    virtual void getObstacleRange(int &min, int &max) const;

protected:
    // === Protected constructors (accessible by subclasses) ===
    Game(Difficulty difficulty, Renderer *renderer);

    // === Protected members (accessible by subclasses) ===
    std::vector<std::vector<char>> grid_;
    std::vector<std::pair<int, int>> targets_;
    Player player_;
    Difficulty difficulty_;
    UndoSystem undo_;
    GameState initial_state_;
    Renderer *renderer_;
    UserData *user_;
    bool game_running_;
    bool level_complete_;

    // === Protected methods (accessible by subclasses) ===
    void initLevel();
    void generateNewMap();
    bool checkWinCondition();
    void saveProgress();
    void promptNextAction();
    void onLevelComplete();
    void processInput(char input);
    void handleMovement(char direction);
    void handleRestart();
    void handleUndo();
    void handleHelp();
    void handleQuit();

private:
    // === Private members ===
    int getCurrentUndoLimit() const;
};

// ============================================================================
// EasyGame subclass - demonstrates polymorphism: easy difficulty (POLYMORPHISM)
// ============================================================================
class EasyGame : public Game {
public:
    EasyGame(Renderer *renderer);
    virtual ~EasyGame();

    int getBoxCount() const override;
    void getObstacleRange(int &min, int &max) const override;
};

// ============================================================================
// MediumGame subclass - demonstrates polymorphism: medium difficulty (POLYMORPHISM)
// ============================================================================
class MediumGame : public Game {
public:
    MediumGame(Renderer *renderer);
    virtual ~MediumGame();

    int getBoxCount() const override;
    void getObstacleRange(int &min, int &max) const override;
};

// ============================================================================
// HardGame subclass - demonstrates polymorphism: hard difficulty (POLYMORPHISM)
// ============================================================================
class HardGame : public Game {
public:
    HardGame(Renderer *renderer);
    virtual ~HardGame();

    int getBoxCount() const override;
    void getObstacleRange(int &min, int &max) const override;
};

#endif // GAME_H
