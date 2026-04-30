#include "terminal.h"

#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

/*
 * Function: envEquals
 * Purpose: Checks whether an environment variable has a specific string value.
 * Inputs:  name  - the name of the environment variable to look up.
 *          value - the expected string value to compare against.
 * Output:  Returns true if the variable exists and equals value; false otherwise.
 */
bool envEquals(const char *name, const char *value) {
    const char *v = std::getenv(name);
    if (v == nullptr || value == nullptr) {
        return false;
    }
    return std::string(v) == std::string(value);
}

/*
 * Function: envExists
 * Purpose: Checks whether an environment variable is defined in the process environment.
 * Inputs:  name - the name of the environment variable to check.
 * Output:  Returns true if the variable exists (any value); false if it is not set.
 */
bool envExists(const char *name) {
    return std::getenv(name) != nullptr;
}

#ifdef _WIN32
/*
 * Function: tryEnableAnsiColor
 * Purpose: Attempts to enable ANSI virtual terminal processing on the Windows
 *          console so that ANSI escape codes render as colors.
 * Inputs:  None.
 * Output:  Returns true if ANSI processing was successfully enabled;
 *          false if the console handle is invalid or the mode cannot be set.
 */
bool tryEnableAnsiColor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) {
        return false;
    }

    DWORD desired = mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, desired)) {
        return false;
    }

    return true;
}

/*
 * Function: isUnicodeConfidentOnWindows
 * Purpose: Determines whether the Windows terminal is likely to display Unicode
 *          characters correctly by checking for modern terminals (Windows Terminal,
 *          VS Code) and UTF-8 console code page (65001).
 * Inputs:  None.
 * Output:  Returns true if Unicode output is expected to render correctly;
 *          false if the terminal is a legacy console or code page is not UTF-8.
 */
bool isUnicodeConfidentOnWindows() {
    if (envEquals("BLOCKTACTICS_FORCE_UNICODE", "1")) {
        return true;
    }

    const bool likely_modern_terminal = envExists("WT_SESSION") || envExists("TERM_PROGRAM");
    if (!likely_modern_terminal) {
        return false;
    }

    UINT out_cp = GetConsoleOutputCP();
    return out_cp == 65001;
}
#endif

} // namespace

/*
 * Function: detectTerminalConfig
 * Purpose: Detects terminal capabilities at program startup and sets color
 *          support and render mode defaults appropriate for the current OS.
 *          On Windows, tries to enable ANSI color and checks for UTF-8 support.
 *          On POSIX, ANSI is assumed supported unless NO_COLOR is set.
 * Inputs:  None.
 * Output:  Returns a populated TerminalConfig struct with:
 *          - ansi_color_supported: whether ANSI escape codes will render.
 *          - color_enabled_by_default: whether colors should be on at startup.
 *          - default_render_mode: ASCII or UNICODE based on terminal capability.
 */
TerminalConfig detectTerminalConfig() {
    TerminalConfig cfg;

#ifdef _WIN32
    cfg.ansi_color_supported = tryEnableAnsiColor();
    cfg.color_enabled_by_default = cfg.ansi_color_supported;

    if (envEquals("BLOCKTACTICS_FORCE_ASCII", "1")) {
        cfg.default_render_mode = RenderMode::ASCII;
    } else if (isUnicodeConfidentOnWindows()) {
        cfg.default_render_mode = RenderMode::UNICODE;
    } else {
        cfg.default_render_mode = RenderMode::ASCII;
    }
#else
    cfg.ansi_color_supported = true;
    cfg.color_enabled_by_default = !envExists("NO_COLOR");

    if (envEquals("BLOCKTACTICS_FORCE_ASCII", "1")) {
        cfg.default_render_mode = RenderMode::ASCII;
    } else {
        cfg.default_render_mode = RenderMode::UNICODE;
    }
#endif

    return cfg;
}

/*
 * Function: renderModeName
 * Purpose: Converts a RenderMode enum value into a printable string.
 * Inputs:  mode - the RenderMode value to convert (ASCII or UNICODE).
 * Output:  Returns the string "Unicode" if mode is UNICODE, otherwise "ASCII".
 */
std::string renderModeName(RenderMode mode) {
    return mode == RenderMode::UNICODE ? "Unicode" : "ASCII";
}
