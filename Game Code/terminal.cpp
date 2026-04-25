#include "terminal.h"

#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

bool envEquals(const char *name, const char *value) {
    const char *v = std::getenv(name);
    if (v == nullptr || value == nullptr) {
        return false;
    }
    return std::string(v) == std::string(value);
}

bool envExists(const char *name) {
    return std::getenv(name) != nullptr;
}

#ifdef _WIN32
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

std::string renderModeName(RenderMode mode) {
    return mode == RenderMode::UNICODE ? "Unicode" : "ASCII";
}
