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

/*
 * Function: detectTerminalConfig
 * Purpose: Detects the terminal's capabilities at program startup and returns
 *          a configuration struct used to set color and render mode defaults.
 * Inputs:  None.
 * Output:  Returns a TerminalConfig struct with ansi_color_supported,
 *          color_enabled_by_default, and default_render_mode populated
 *          based on the current operating environment.
 */
TerminalConfig detectTerminalConfig();

/*
 * Function: renderModeName
 * Purpose: Converts a RenderMode enum value to its human-readable string name.
 * Inputs:  mode - the RenderMode enum value (ASCII or UNICODE).
 * Output:  Returns "Unicode" if mode is UNICODE, otherwise returns "ASCII".
 */
std::string renderModeName(RenderMode mode);

#endif // TERMINAL_H
