#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <vector>
#include "map.h"
#include "terminal.h"

// Forward declarations
struct Player;
struct UserData;
struct LeaderboardEntry;

// ============================================================================
// Renderer class - encapsulates all display output (ENCAPSULATION)
// Responsible for: clearing screen, printing menus, map, status, etc.
// ============================================================================
class Renderer {
public:
    Renderer(bool color_enabled = true,
             RenderMode render_mode = RenderMode::ASCII,
             bool ansi_color_supported = true);
    ~Renderer();

    // === Basic Output ===
    void clearScreen();
    char getInput();

    // === Interface Display ===
    void printWelcome();
    void printMenu();
    void printDifficultyMenu();
    void printControls();
    void printUserProgress(const UserData &user, bool logged_in);
    void printLeaderboard(const std::vector<LeaderboardEntry> &entries);

    // === In-Game Display ===
    void printGameStatus(Difficulty diff, int steps, int completed, int total,
                         int undo_left, int undo_max);
    void printMap(const std::vector<std::vector<char>> &grid,
                  const std::vector<std::pair<int, int>> &target_positions,
                  int player_row, int player_col);
    void printHelp();
    virtual void printWinScreen(int steps, const UserData &user, Difficulty diff);

    // === Color Control ===
    void setColorEnabled(bool enabled);
    bool isColorEnabled() const;

    // === Render Mode Control ===
    void setRenderMode(RenderMode mode);
    RenderMode getRenderMode() const;

private:
    bool color_enabled;
    bool ansi_color_supported;
    RenderMode render_mode;

    const std::string &on(const std::string &code) const;
    std::string line(int width, char ascii_ch) const;
    std::string boxTop(int width) const;
    std::string boxBottom(int width) const;
    std::string boxMiddle(int width) const;
    std::string boxEmpty(int width) const;
    std::string boxRow(const std::string &content, int width) const;
    std::string menuBorder(const std::string &title) const;
    std::string menuSep(int width) const;

    // Polymorphism: different cell types print with different colors
    // Base class method, can be overridden by subclasses
    virtual void printCell(char cell, bool is_player_position,
                           const std::vector<std::pair<int, int>> &target_positions,
                           int row, int col);

    // Get cell color type
    enum class CellType { PLAYER, BOX, BOX_ON_TARGET, TARGET, WALL, OBSTACLE, EMPTY };
    CellType getCellType(char cell, bool is_player_pos,
                         const std::vector<std::pair<int, int>> &target_positions,
                         int row, int col) const;
};

// ============================================================================
// RendererWithEffects subclass - polymorphism (method overriding)
// ============================================================================
class RendererWithEffects : public Renderer {
public:
    RendererWithEffects(bool color_enabled = true);
    ~RendererWithEffects();

    // Override base class method: add effects (POLYMORPHISM)
    void printWinScreen(int steps, const UserData &user, Difficulty diff) override;

    // New effect methods
    void printCelebration();

private:
    int celebration_frames;
};

#endif // RENDERER_H
