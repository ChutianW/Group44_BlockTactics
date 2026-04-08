/*
 * Block Tactics - A Sokoban-Style Puzzle Game
 * COMP2113 / ENGG1340 Course Project
 * Group 44
 * 
 * This is the main entry point for the game.
 * 
 * Features implemented:
 * - Random map generation with difficulty levels
 * - Player movement and box pushing
 * - Win condition detection
 * - Restart system
 * - Undo system (dynamic memory with stack)
 * - User save/load system (file I/O)
 * - Color rendering (ANSI codes)
 * - Multiple difficulty levels
 * 
 * Required coding elements:
 * 1. Random events - Random map generation
 * 2. Data structures - 2D vectors, structs, stack
 * 3. Dynamic memory - Undo system with new/delete
 * 4. File I/O - User progress save/load
 * 5. Multiple files - Modular design
 */

#include "game.h"

int main() {
    Game game;
    game.run();
    return 0;
}
