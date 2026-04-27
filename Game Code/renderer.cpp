#include "renderer.h"
#include "player.h"
#include "file_io.h"
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif
#include <iomanip>
#include <ctime>

// ===============================================================================
// Renderer base class implementation
// ===============================================================================

Renderer::Renderer(bool color_enabled, RenderMode render_mode, bool ansi_color_supported)
    : color_enabled(color_enabled && ansi_color_supported),
      ansi_color_supported(ansi_color_supported),
      render_mode(render_mode) {}

Renderer::~Renderer() {}

const std::string &Renderer::on(const std::string &code) const {
    static const std::string empty;
    return color_enabled ? code : empty;
}

std::string Renderer::line(int width, char ascii_ch, const std::string &unicode_ch) const {
    return std::string(width, ascii_ch);
}

std::string Renderer::boxTop(int width) const {
    return "  +" + line(width, '-', "-") + "+";
}

std::string Renderer::boxBottom(int width) const {
    return "  +" + line(width, '-', "-") + "+";
}

std::string Renderer::boxMiddle(int width) const {
    return "  " + line(width, '=', "=");
}

std::string Renderer::boxEmpty(int width) const {
    return "  |" + line(width, ' ', " ") + "|";
}

std::string Renderer::boxRow(const std::string &content, int width) const {
    std::string row = content;
    if (static_cast<int>(row.size()) < width) {
        row += std::string(width - row.size(), ' ');
    }
    return "  |" + row + "|";
}

// Clear screen (Linux/Windows compatible)
void Renderer::clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// Get single char input (cross-platform)
char Renderer::getInput() {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

void Renderer::setColorEnabled(bool enabled) {
    color_enabled = enabled && ansi_color_supported;
}

bool Renderer::isColorEnabled() const {
    return color_enabled;
}

void Renderer::setRenderMode(RenderMode mode) {
    render_mode = mode;
}

RenderMode Renderer::getRenderMode() const {
    return render_mode;
}

// Welcome screen
void Renderer::printWelcome() {
    clearScreen();
    std::cout << on(COLOR_CYAN) << on(COLOR_BOLD);
    std::cout << "\n";
    std::cout << boxTop(42) << "\n";
    std::cout << boxEmpty(42) << "\n";
    std::cout << boxRow("         BLOCK TACTICS                    ", 42) << "\n";
    std::cout << boxEmpty(42) << "\n";
    std::cout << boxRow("      A Sokoban-Style Puzzle Game         ", 42) << "\n";
    std::cout << boxEmpty(42) << "\n";
    std::cout << boxRow("         COMP2113 / ENGG1340              ", 42) << "\n";
    std::cout << boxRow("            Group 44                      ", 42) << "\n";
    std::cout << boxEmpty(42) << "\n";
    std::cout << boxBottom(42) << "\n";
    std::cout << on(COLOR_RESET) << "\n";
    std::cout << "  Press any key to continue...\n";
    getInput();
}

// ===============================================================================
// Menu helpers - all use render_mode to switch Unicode/ASCII
// ===============================================================================

std::string Renderer::menuBorder(const std::string &title) const {
    int title_len = static_cast<int>(title.size());
    int side_len = (24 - title_len) / 2;
    // Always use ASCII: = for borders, works on all terminals
    std::string side(side_len, '=');
    return " " + side + title + side + " ";
}

std::string Renderer::menuSep(int width) const {
    return "  " + std::string(width, '-');
}

// Main menu
void Renderer::printMenu() {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  " << menuBorder(" MAIN MENU ") << "\n\n";
    std::cout << COLOR_RESET;
    std::cout << "  [1] New Game\n";
    std::cout << "  [2] Continue (from saved progress)\n";
    std::cout << "  [3] View Controls\n";
    std::cout << "  [4] View Progress\n";
    std::cout << "  [5] Toggle Colors\n";
    std::cout << "  [L] Leaderboard\n";
    std::cout << "  [Q] Quit\n\n";
    std::cout << "  Enter your choice: ";
}

// Difficulty selection menu
void Renderer::printDifficultyMenu() {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  " << menuBorder(" SELECT DIFFICULTY ") << "\n\n";
    std::cout << COLOR_RESET;

    std::cout << "  [1] " << COLOR_GREEN << "Easy" << COLOR_RESET
              << "   - 3 boxes, no obstacles\n";
    std::cout << "  [2] " << COLOR_YELLOW << "Medium" << COLOR_RESET
              << " - 5 boxes, 3-5 obstacles\n";
    std::cout << "  [3] " << COLOR_RED << "Hard" << COLOR_RESET
              << "   - 7 boxes, 5-7 obstacles\n";
    std::cout << "  [B] Back to menu\n\n";
    std::cout << "  Enter your choice: ";
}

// Controls display
void Renderer::printControls() {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  " << menuBorder(" CONTROLS ") << "\n\n";
    std::cout << COLOR_RESET;

    std::cout << "  " << COLOR_BOLD << "Movement:" << COLOR_RESET << "\n";
    std::cout << "    W - Move Up\n";
    std::cout << "    A - Move Left\n";
    std::cout << "    S - Move Down\n";
    std::cout << "    D - Move Right\n\n";

    std::cout << "  " << COLOR_BOLD << "Actions:" << COLOR_RESET << "\n";
    std::cout << "    R - Restart Level\n";
    std::cout << "    U - Undo Last Move\n";
    std::cout << "    H - Show Help\n";
    std::cout << "    Q - Quit to Menu\n\n";

    std::cout << "  " << COLOR_BOLD << "Symbols:" << COLOR_RESET << "\n";
    std::cout << "    " << COLOR_YELLOW << "@" << COLOR_RESET << " - Player\n";
    std::cout << "    " << COLOR_RED << "$" << COLOR_RESET << " - Box\n";
    std::cout << "    " << COLOR_GREEN << "^" << COLOR_RESET << " - Target\n";
    std::cout << "    " << COLOR_BRIGHT_GREEN << "*" << COLOR_RESET << " - Box on Target\n";
    std::cout << "    " << COLOR_GRAY << "#" << COLOR_RESET << " - Wall\n";
    std::cout << "    " << COLOR_DARK_GRAY << "%" << COLOR_RESET << " - Obstacle\n\n";

    std::cout << "  " << COLOR_BOLD << "Goal:" << COLOR_RESET << "\n";
    std::cout << "    Push all boxes ($) onto targets (^)\n";
    std::cout << "    When a box is on a target, it shows as (*)\n\n";

    std::cout << "  Press any key to continue...\n";
    getInput();
}

// User progress display
void Renderer::printUserProgress(const UserData &user, bool logged_in) {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  " << menuBorder(" PROGRESS ") << "\n\n";
    std::cout << COLOR_RESET;

    if (!logged_in) {
        std::cout << "  No user logged in.\n\n";
    } else {
        std::cout << "  User: " << COLOR_YELLOW << user.username << COLOR_RESET << "\n\n";
        std::cout << "  Highest Level: ";
        switch (user.highest_level) {
            case 1: std::cout << COLOR_GREEN << "Easy" << COLOR_RESET; break;
            case 2: std::cout << COLOR_YELLOW << "Medium" << COLOR_RESET; break;
            case 3: std::cout << COLOR_RED << "Hard" << COLOR_RESET; break;
        }
        std::cout << "\n\n";

        std::cout << "  Best Steps:\n";
        std::cout << "    Easy:   " << (user.best_steps_easy > 0 ?
                                   std::to_string(user.best_steps_easy) : "Not completed") << "\n";
        std::cout << "    Medium: " << (user.best_steps_medium > 0 ?
                                   std::to_string(user.best_steps_medium) : "Not completed") << "\n";
        std::cout << "    Hard:   " << (user.best_steps_hard > 0 ?
                                   std::to_string(user.best_steps_hard) : "Not completed") << "\n";
    }

    std::cout << "\n  Press any key to continue...\n";
    getInput();
}

// Leaderboard display
void Renderer::printLeaderboard(const std::vector<LeaderboardEntry> &entries) {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  " << menuBorder(" LEADERBOARD ") << "\n\n";
    std::cout << COLOR_RESET;

    if (entries.empty()) {
        std::cout << "  No records yet. Start playing to set records!\n";
    } else {
        const std::string COLOR_GOLD = "\033[33m";
        const std::string COLOR_SILVER = "\033[37m";
        const std::string COLOR_ORANGE = "\033[38;5;208m";

        std::cout << "  " << COLOR_BOLD;
        std::cout << "Rank  Username     Difficulty  Steps  Undos  Created\n";
        std::cout << COLOR_RESET;
        std::cout << menuSep(50) << "\n";

        for (size_t i = 0; i < entries.size(); i++) {
            const auto &e = entries[i];

            std::string color;
            if (i == 0) color = COLOR_GOLD;
            else if (i == 1) color = COLOR_SILVER;
            else if (i == 2) color = COLOR_ORANGE;
            else color = COLOR_RESET;

            std::string diff_name;
            switch (e.difficulty) {
                case 1: diff_name = "Easy"; break;
                case 2: diff_name = "Medium"; break;
                case 3: diff_name = "Hard"; break;
            }

            char created_str[20];
            struct tm *tm_info = localtime(&e.created_at);
            strftime(created_str, sizeof(created_str), "%Y-%m-%d", tm_info);

            std::cout << "  " << color;
            std::cout << (i + 1) << ".  " << e.username;
            for (size_t s = e.username.length(); s < 12; s++) std::cout << " ";
            std::cout << diff_name;
            for (size_t s = diff_name.length(); s < 8; s++) std::cout << " ";
            std::cout << e.best_steps << "    " << e.total_undos << "    " << created_str << "\n";
            std::cout << COLOR_RESET;
        }
    }

    std::cout << "\n  Press any key to continue...\n";
    getInput();
}

// Game status bar
void Renderer::printGameStatus(Difficulty diff, int steps, int completed, int total,
                                int undo_left, int undo_max) {
    std::cout << COLOR_CYAN;
    std::cout << "  +" << line(38, '=', "=") << "+\n";
    std::cout << "  |  ";
    std::cout << "Difficulty: ";
    switch (diff) {
        case EASY:   std::cout << COLOR_GREEN << "Easy  " << COLOR_CYAN; break;
        case MEDIUM: std::cout << COLOR_YELLOW << "Medium" << COLOR_CYAN; break;
        case HARD:   std::cout << COLOR_RED << "Hard  " << COLOR_CYAN; break;
    }
    std::cout << " | Steps: " << COLOR_YELLOW << steps << COLOR_CYAN;
    std::cout << " | " << completed << "/" << total;
    if (undo_max == 0) {
        std::cout << " | U:" << COLOR_GRAY << "N/A" << COLOR_CYAN;
    } else {
        std::cout << " | U:" << COLOR_YELLOW << undo_left << COLOR_CYAN << "/" << COLOR_YELLOW << undo_max << COLOR_CYAN;
    }
    std::cout << "  |\n";
    std::cout << "  +" << line(38, '=', "=") << "+\n";
    std::cout << COLOR_RESET;
}

// Get cell type (polymorphism helper)
Renderer::CellType Renderer::getCellType(char cell, bool is_player_pos,
                                         const std::vector<std::pair<int, int>> &target_positions,
                                         int row, int col) const {
    if (is_player_pos) {
        return CellType::PLAYER;
    }

    if (cell == SYMBOL_BOX) {
        for (const auto &pos : target_positions) {
            if (pos.first == col && pos.second == row) {
                return CellType::BOX_ON_TARGET;
            }
        }
        return CellType::BOX;
    }

    if (cell == SYMBOL_TARGET) {
        return CellType::TARGET;
    }
    if (cell == SYMBOL_WALL) {
        return CellType::WALL;
    }
    if (cell == SYMBOL_OBSTACLE) {
        return CellType::OBSTACLE;
    }

    return CellType::EMPTY;
}

// Print single cell (virtual, subclasses can override - POLYMORPHISM)
void Renderer::printCell(char cell, bool is_player_position,
                        const std::vector<std::pair<int, int>> &target_positions,
                        int row, int col) {
    CellType type = getCellType(cell, is_player_position, target_positions, row, col);

    if (!color_enabled) {
        std::cout << cell;
        return;
    }

    switch (type) {
        case CellType::PLAYER:
            std::cout << COLOR_YELLOW << COLOR_BOLD << cell << COLOR_RESET;
            break;
        case CellType::BOX:
            std::cout << COLOR_RED << cell << COLOR_RESET;
            break;
        case CellType::BOX_ON_TARGET:
            std::cout << COLOR_BRIGHT_GREEN << COLOR_BOLD << cell << COLOR_RESET;
            break;
        case CellType::TARGET:
            std::cout << COLOR_GREEN << cell << COLOR_RESET;
            break;
        case CellType::WALL:
            std::cout << COLOR_GRAY << cell << COLOR_RESET;
            break;
        case CellType::OBSTACLE:
            std::cout << COLOR_DARK_GRAY << cell << COLOR_RESET;
            break;
        case CellType::EMPTY:
        default:
            std::cout << cell;
            break;
    }
}

// Print map
void Renderer::printMap(const std::vector<std::vector<char>> &grid,
                        const std::vector<std::pair<int, int>> &target_positions,
                        int player_row, int player_col) {
    std::cout << "\n";
    for (int r = 0; r < MAP_ROWS; r++) {
        std::cout << "  ";
        for (int c = 0; c < MAP_COLS; c++) {
            bool is_player = (r == player_row && c == player_col);
            char cell = is_player ? SYMBOL_PLAYER : grid[r][c];
            printCell(cell, is_player, target_positions, r, c);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// Quick help
void Renderer::printHelp() {
    std::cout << "\n";
    std::cout << COLOR_CYAN;
    std::cout << "  " << menuBorder(" QUICK HELP ") << "\n";
    std::cout << COLOR_RESET;
    std::cout << "  W/A/S/D = Move | R = Restart | U = Undo\n";
    std::cout << "  Q = Quit to Menu | H = This help\n";
    std::cout << "  Goal: Push all " << COLOR_RED << "$" << COLOR_RESET
              << " onto " << COLOR_GREEN << "^" << COLOR_RESET << "\n";
    std::cout << COLOR_CYAN;
    std::cout << "  " << menuBorder("           ") << "\n";
    std::cout << COLOR_RESET;
}

// Win screen
void Renderer::printWinScreen(int steps, const UserData &user, Difficulty diff) {
    clearScreen();
    std::cout << COLOR_BRIGHT_GREEN << COLOR_BOLD;
    std::cout << "\n";
    std::cout << "  +" << line(40, '=', "=") << "+\n";
    std::cout << "  |" << line(42, ' ', " ") << "|\n";
    std::cout << "  |      CONGRATULATIONS!              |\n";
    std::cout << "  |" << line(42, ' ', " ") << "|\n";
    std::cout << "  |         Level Complete!             |\n";
    std::cout << "  |" << line(42, ' ', " ") << "|\n";
    std::cout << "  +" << line(40, '=', "=") << "+\n";
    std::cout << COLOR_RESET << "\n";

    std::cout << "  Steps taken: " << COLOR_YELLOW << steps << COLOR_RESET << "\n\n";

    int current_best = 0;
    switch (diff) {
        case EASY:   current_best = user.best_steps_easy; break;
        case MEDIUM: current_best = user.best_steps_medium; break;
        case HARD:   current_best = user.best_steps_hard; break;
    }

    if (current_best == 0 || steps < current_best) {
        std::cout << "  " << COLOR_BRIGHT_GREEN << "NEW BEST SCORE!" << COLOR_RESET << "\n\n";
    }

    std::cout << "  [N] Next Level\n";
    std::cout << "  [R] Replay Same Level\n";
    std::cout << "  [M] Return to Menu\n\n";
    std::cout << "  Enter your choice: ";
}

// ===============================================================================
// RendererWithEffects subclass - polymorphism (method overriding)
// ===============================================================================

RendererWithEffects::RendererWithEffects(bool color_enabled)
    : Renderer(color_enabled), celebration_frames(5) {}

RendererWithEffects::~RendererWithEffects() {}

// Override win screen: add celebration effect (POLYMORPHISM)
void RendererWithEffects::printWinScreen(int steps, const UserData &user, Difficulty diff) {
    Renderer::printWinScreen(steps, user, diff);
    printCelebration();
}

// Celebration animation
void RendererWithEffects::printCelebration() {
    std::cout << COLOR_YELLOW;
    for (int i = 0; i < celebration_frames; i++) {
        std::cout << "  *";
        for (int j = 0; j < i; j++) std::cout << "  ";
        std::cout << "*\n";
        std::cout << "  ";
        for (int j = 0; j < celebration_frames - i - 1; j++) std::cout << "  ";
        for (int j = 0; j <= i; j++) std::cout << "* ";
        std::cout << "\n";
    }
    std::cout << COLOR_RESET;
    std::cout << "\n  Press any key to continue...\n";
}
