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

/*
 * Function: Renderer::Renderer (constructor)
 * Purpose:  Initializes the renderer with color support and render mode settings.
 *           Color is only enabled if both color_enabled and ansi_color_supported are true.
 * Inputs:   color_enabled        - whether the user wants colors on.
 *           render_mode          - ASCII or UNICODE rendering mode.
 *           ansi_color_supported - whether the terminal actually supports ANSI codes.
 * Output:   A Renderer ready to draw the game UI.
 */
Renderer::Renderer(bool color_enabled, RenderMode render_mode, bool ansi_color_supported)
    : color_enabled(color_enabled && ansi_color_supported),
      ansi_color_supported(ansi_color_supported),
      render_mode(render_mode) {}

/*
 * Function: Renderer::~Renderer (destructor)
 * Purpose:  Cleans up the Renderer instance (no dynamic allocations in base class).
 * Inputs:   None.
 * Output:   No side effects beyond standard object teardown.
 */
Renderer::~Renderer() {}

/*
 * Function: Renderer::on
 * Purpose:  Returns an ANSI color code if color output is enabled; empty string if not.
 *           Used to conditionally wrap output in color codes without extra branching.
 * Inputs:   code - an ANSI escape code string (e.g., COLOR_RED).
 * Output:   Returns code if color_enabled is true; returns an empty string otherwise.
 */
const std::string &Renderer::on(const std::string &code) const {
    static const std::string empty;
    return color_enabled ? code : empty;
}

/*
 * Function: Renderer::line
 * Purpose:  Generates a repeated-character string of a given width.
 *           Used to build horizontal borders and separators in UI boxes.
 * Inputs:   width    - the number of characters to repeat.
 *           ascii_ch - the character to repeat (e.g., '-', '=', ' ').
 * Output:   Returns a std::string of length width filled with ascii_ch.
 */
std::string Renderer::line(int width, char ascii_ch) const {
    return std::string(width, ascii_ch);
}

/*
 * Function: Renderer::boxTop
 * Purpose:  Generates the top border line of a UI box with dashes.
 * Inputs:   width - the inner width of the box (between + corners).
 * Output:   Returns a string in the form "  +---...---+".
 */
std::string Renderer::boxTop(int width) const {
    return "  +" + line(width, '-') + "+";
}

/*
 * Function: Renderer::boxBottom
 * Purpose:  Generates the bottom border line of a UI box with dashes.
 * Inputs:   width - the inner width of the box (between + corners).
 * Output:   Returns a string in the form "  +---...---+".
 */
std::string Renderer::boxBottom(int width) const {
    return "  +" + line(width, '-') + "+";
}

/*
 * Function: Renderer::boxMiddle
 * Purpose:  Generates a horizontal separator line of '=' characters without box corners.
 *           Used for the status bar framing.
 * Inputs:   width - the total width of the separator.
 * Output:   Returns a string in the form "  ===...===".
 */
std::string Renderer::boxMiddle(int width) const {
    return "  " + line(width, '=');
}

/*
 * Function: Renderer::boxEmpty
 * Purpose:  Generates an empty interior line of a UI box (spaces between | borders).
 * Inputs:   width - the inner width of the box (spaces between | borders).
 * Output:   Returns a string in the form "  |   ...   |".
 */
std::string Renderer::boxEmpty(int width) const {
    return "  |" + line(width, ' ') + "|";
}

/*
 * Function: Renderer::boxRow
 * Purpose:  Generates an interior box line with text content, padded to the given width.
 *           If content is shorter than width, it is right-padded with spaces.
 * Inputs:   content - the text to display inside the box row.
 *           width   - the total inner width the row must fill.
 * Output:   Returns a string in the form "  |content   |" padded to width.
 */
std::string Renderer::boxRow(const std::string &content, int width) const {
    std::string row = content;
    if (static_cast<int>(row.size()) < width) {
        row += std::string(width - row.size(), ' ');
    }
    return "  |" + row + "|";
}

/*
 * Function: Renderer::clearScreen
 * Purpose:  Clears the terminal screen using the appropriate system command.
 *           Uses "cls" on Windows and "clear" on POSIX systems.
 * Inputs:   None.
 * Output:   Clears the terminal; no return value.
 */
void Renderer::clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

/*
 * Function: Renderer::getInput
 * Purpose:  Reads a single keypress from the terminal without requiring Enter.
 *           Uses _getch() on Windows and raw termios mode on POSIX.
 * Inputs:   None.
 * Output:   Returns the character corresponding to the key pressed.
 */
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

/*
 * Function: Renderer::setColorEnabled
 * Purpose:  Toggles ANSI color output on or off at runtime.
 *           Color can only be enabled if the terminal actually supports ANSI codes.
 * Inputs:   enabled - true to turn on colors, false to turn them off.
 * Output:   Sets color_enabled to (enabled && ansi_color_supported).
 */
void Renderer::setColorEnabled(bool enabled) {
    color_enabled = enabled && ansi_color_supported;
}

/*
 * Function: Renderer::isColorEnabled
 * Purpose:  Reports whether ANSI color output is currently active.
 * Inputs:   None.
 * Output:   Returns true if colors are on; false if running in plain text mode.
 */
bool Renderer::isColorEnabled() const {
    return color_enabled;
}

/*
 * Function: Renderer::setRenderMode
 * Purpose:  Switches the render mode between ASCII and UNICODE at runtime.
 * Inputs:   mode - the new RenderMode (ASCII or UNICODE).
 * Output:   Updates the internal render_mode member.
 */
void Renderer::setRenderMode(RenderMode mode) {
    render_mode = mode;
}

/*
 * Function: Renderer::getRenderMode
 * Purpose:  Returns the current render mode setting.
 * Inputs:   None.
 * Output:   Returns ASCII or UNICODE as set during construction or by setRenderMode().
 */
RenderMode Renderer::getRenderMode() const {
    return render_mode;
}

/*
 * Function: Renderer::printWelcome
 * Purpose:  Displays the game's splash/welcome screen with title and team info.
 *           Clears the screen, draws a framed title box, and waits for a keypress.
 * Inputs:   None.
 * Output:   Prints the welcome banner to stdout; blocks until key press.
 */
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

/*
 * Function: Renderer::menuBorder
 * Purpose:  Generates a centered header banner string for a menu section.
 *           Pads both sides of the title with '=' characters to reach ~26 chars.
 * Inputs:   title - the text label to embed in the banner.
 * Output:   Returns a string like " === TITLE ===".
 */
std::string Renderer::menuBorder(const std::string &title) const {
    int title_len = static_cast<int>(title.size());
    int side_len = (24 - title_len) / 2;
    // Always use ASCII: = for borders, works on all terminals
    std::string side(side_len, '=');
    return " " + side + title + side + " ";
}

/*
 * Function: Renderer::menuSep
 * Purpose:  Generates a horizontal separator line of '-' characters for tables.
 * Inputs:   width - the number of '-' characters in the separator.
 * Output:   Returns a string of "  " followed by width '-' characters.
 */
std::string Renderer::menuSep(int width) const {
    return "  " + std::string(width, '-');
}

/*
 * Function: Renderer::printMenu
 * Purpose:  Displays the main menu with all available options.
 *           Clears the screen and prints numbered options and prompts for input.
 * Inputs:   None.
 * Output:   Prints the main menu to stdout; does not read input itself.
 */
void Renderer::printMenu() {
    clearScreen();
    std::cout << COLOR_CYAN << COLOR_BOLD;
    std::cout << "\n  " << menuBorder(" MAIN MENU ") << "\n\n";
    std::cout << COLOR_RESET;
    std::cout << "  [1] New Game\n";
    std::cout << "  [2] Continue (from saved progress)\n";
    std::cout << "  [3] Game Controls & Rules\n";
    std::cout << "  [4] View Progress\n";
    std::cout << "  [5] Toggle Colors\n";
    std::cout << "  [L] Leaderboard\n";
    std::cout << "  [Q] Quit\n\n";
    std::cout << "  Enter your choice: ";
}

/*
 * Function: Renderer::printDifficultyMenu
 * Purpose:  Displays the difficulty selection screen (Easy/Medium/Hard) with
 *           color-coded labels and obstacle/box count descriptions.
 * Inputs:   None.
 * Output:   Prints the difficulty menu to stdout; does not read input.
 */
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

/*
 * Function: Renderer::printControls
 * Purpose:  Displays the full controls reference screen showing movement keys,
 *           action keys, and symbol legend. Waits for a keypress to dismiss.
 * Inputs:   None.
 * Output:   Prints the controls page to stdout; blocks until key press.
 */
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
    std::cout << "    G - Regenerate Map\n";
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

/*
 * Function: Renderer::printUserProgress
 * Purpose:  Displays the current user's progress including highest difficulty
 *           reached and best step counts for each difficulty level.
 * Inputs:   user      - the UserData record for the current player.
 *           logged_in - whether a user is currently logged in (shows "not logged in" if false).
 * Output:   Prints the progress screen to stdout; blocks until key press.
 */
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

/*
 * Function: Renderer::printLeaderboard
 * Purpose:  Displays a ranked leaderboard table of players sorted by difficulty
 *           then best score. Top 3 entries are highlighted with gold/silver/orange.
 * Inputs:   entries - a sorted vector of LeaderboardEntry structs to display.
 * Output:   Prints the leaderboard table to stdout; blocks until key press.
 */
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

/*
 * Function: Renderer::printGameStatus
 * Purpose:  Renders the in-game status bar showing difficulty, steps taken,
 *           target completion progress, and remaining undo count.
 *           The bar width is dynamically computed from the content.
 * Inputs:   diff      - the current difficulty (EASY, MEDIUM, or HARD).
 *           steps     - total steps taken by the player this level.
 *           completed - number of boxes currently on targets.
 *           total     - total number of targets (boxes needed).
 *           undo_left - remaining undos available.
 *           undo_max  - maximum undos for this difficulty (0 = N/A).
 * Output:   Prints a framed status bar to stdout with ANSI color coding.
 */
void Renderer::printGameStatus(Difficulty diff, int steps, int completed, int total,
                                int undo_left, int undo_max) {
    // Build plain content to measure visible width
    std::string diff_str;
    switch (diff) {
        case EASY:   diff_str = "Easy  "; break;
        case MEDIUM: diff_str = "Medium"; break;
        case HARD:   diff_str = "Hard  "; break;
    }
    std::string undo_str = (undo_max == 0) ? "N/A"
        : std::to_string(undo_left) + "/" + std::to_string(undo_max);
    std::string plain = "  Difficulty: " + diff_str + " | Steps: "
        + std::to_string(steps) + " | Target: " + std::to_string(completed)
        + "/" + std::to_string(total) + " | U:" + undo_str + "  ";
    int width = (int)plain.size();

    // Print frame and colored content
    std::cout << COLOR_CYAN;
    std::cout << "  +" << line(width, '=') << "+\n";
    std::cout << "  |";
    std::cout << "  Difficulty: ";
    switch (diff) {
        case EASY:   std::cout << COLOR_GREEN << "Easy  " << COLOR_CYAN; break;
        case MEDIUM: std::cout << COLOR_YELLOW << "Medium" << COLOR_CYAN; break;
        case HARD:   std::cout << COLOR_RED << "Hard  " << COLOR_CYAN; break;
    }
    std::cout << " | Steps: " << COLOR_YELLOW << steps << COLOR_CYAN;
    std::cout << " | Target: " << completed << "/" << total;
    if (undo_max == 0) {
        std::cout << " | U:" << COLOR_GRAY << "N/A" << COLOR_CYAN;
    } else {
        std::cout << " | U:" << COLOR_YELLOW << undo_left << COLOR_CYAN << "/" << COLOR_YELLOW << undo_max << COLOR_CYAN;
    }
    std::cout << "  |\n";
    std::cout << "  +" << line(width, '=') << "+\n";
    std::cout << COLOR_RESET;
}

/*
 * Function: Renderer::getCellType
 * Purpose:  Classifies a grid cell into a CellType enum used to select the
 *           correct ANSI color when rendering. Detects player, box, box-on-target,
 *           target, wall, obstacle, and empty cell types.
 * Inputs:   cell             - the character symbol in the grid cell.
 *           is_player_pos    - true if this cell is the player's current position.
 *           target_positions - list of target positions to detect box-on-target.
 *           row              - row index of the cell.
 *           col              - column index of the cell.
 * Output:   Returns the corresponding CellType enum value.
 */
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

/*
 * Function: Renderer::printCell
 * Purpose:  Prints a single grid cell character to stdout with appropriate ANSI
 *           color. Virtual so subclasses can override for custom rendering.
 *           Falls back to plain character output when color is disabled.
 * Inputs:   cell             - the character symbol to print.
 *           is_player_position - true if this is the player's cell.
 *           target_positions - list of target positions for box-on-target detection.
 *           row              - row index of the cell.
 *           col              - column index of the cell.
 * Output:   Prints the colored (or plain) cell character to stdout.
 */
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

/*
 * Function: Renderer::printMap
 * Purpose:  Renders the full game grid to stdout, calling printCell() for each cell.
 *           Adds two-space indentation per row for alignment.
 * Inputs:   grid             - the 2D game grid to render.
 *           target_positions - target positions for box-on-target color detection.
 *           player_row       - current row of the player for PLAYER cell type.
 *           player_col       - current column of the player.
 * Output:   Prints the full 10x10 grid to stdout with surrounding blank lines.
 */
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

/*
 * Function: Renderer::printHelp
 * Purpose:  Prints a compact in-game help overlay showing key bindings and goal.
 *           Does not wait for input; the caller is responsible for that.
 * Inputs:   None.
 * Output:   Prints the quick-help text block to stdout.
 */
void Renderer::printHelp() {
    std::cout << "\n";
    std::cout << COLOR_CYAN;
    std::cout << "  " << menuBorder(" QUICK HELP ") << "\n";
    std::cout << COLOR_RESET;
    std::cout << "  W/A/S/D = Move | R = Restart | U = Undo\n";
    std::cout << "  G = New Map | Q = Quit to Menu | H = This help\n";
    std::cout << "  Goal: Push all " << COLOR_RED << "$" << COLOR_RESET
              << " onto " << COLOR_GREEN << "^" << COLOR_RESET << "\n";
    std::cout << COLOR_CYAN;
    std::cout << "  " << menuBorder("           ") << "\n";
    std::cout << COLOR_RESET;
}

/*
 * Function: Renderer::printWinScreen
 * Purpose:  Displays the level-complete win screen with step count, new-best
 *           indicator (if applicable), and post-win action options (N/R/M).
 * Inputs:   steps - the number of steps taken to complete the level.
 *           user  - the UserData record used to compare against the best score.
 *           diff  - the current difficulty for selecting the correct best score.
 * Output:   Clears screen and prints the win screen to stdout; does not read input.
 */
void Renderer::printWinScreen(int steps, const UserData &user, Difficulty diff) {
    clearScreen();
    std::cout << COLOR_BRIGHT_GREEN << COLOR_BOLD;
    std::cout << "\n";
    std::cout << "  +" << line(40, '=') << "+\n";
    std::cout << "  |" << line(40, ' ') << "|\n";
    std::cout << "  |           CONGRATULATIONS!             |\n";
    std::cout << "  |" << line(40, ' ') << "|\n";
    std::cout << "  |            Level Complete!             |\n";
    std::cout << "  |" << line(40, ' ') << "|\n";
    std::cout << "  +" << line(40, '=') << "+\n";
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

/*
 * Function: RendererWithEffects::RendererWithEffects (constructor)
 * Purpose:  Constructs a RendererWithEffects with celebration_frames preset to 5.
 *           Inherits all base Renderer behavior.
 * Inputs:   color_enabled - whether ANSI colors should be used.
 * Output:   A RendererWithEffects ready to use with animated win screen.
 */
RendererWithEffects::RendererWithEffects(bool color_enabled)
    : Renderer(color_enabled), celebration_frames(5) {}

/*
 * Function: RendererWithEffects::~RendererWithEffects (destructor)
 * Purpose:  Virtual destructor for safe deletion of RendererWithEffects.
 * Inputs:   None.
 * Output:   Delegates to base Renderer destructor.
 */
RendererWithEffects::~RendererWithEffects() {}

/*
 * Function: RendererWithEffects::printWinScreen
 * Purpose:  Overrides base printWinScreen to add an ASCII celebration animation
 *           after the standard win screen. Demonstrates runtime polymorphism.
 * Inputs:   steps - number of steps taken to win the level.
 *           user  - the UserData used for best score comparison.
 *           diff  - current difficulty.
 * Output:   Calls base printWinScreen, then prints the celebration animation.
 */
void RendererWithEffects::printWinScreen(int steps, const UserData &user, Difficulty diff) {
    Renderer::printWinScreen(steps, user, diff);
    printCelebration();
}

/*
 * Function: RendererWithEffects::printCelebration
 * Purpose:  Prints a simple ASCII staircase celebration animation using '*' symbols.
 *           Number of animation frames controlled by celebration_frames member.
 * Inputs:   None.
 * Output:   Prints the celebration pattern to stdout, then "Press any key" prompt.
 */
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
