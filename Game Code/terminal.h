#ifndef TERMINAL_H
#define TERMINAL_H

#include <string>

enum class RenderMode {
    ASCII,
    UNICODE
};

struct TerminalConfig {
    bool ansi_color_supported;
    bool color_enabled_by_default;
    RenderMode default_render_mode;
};

TerminalConfig detectTerminalConfig();
std::string renderModeName(RenderMode mode);

#endif // TERMINAL_H
