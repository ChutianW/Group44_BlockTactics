# Work Log

## Mar 21
- First team meeting
- Conducted brainstorming
- Confirmed the core content of the game

## Apr 3
- Started project construction
- Created GitHub repository
- Wrote the project agenda
- Finalized detailed game design and task assignment

## Apr 8
- Completed the program framework
- Finished the initial version of the program
- Carried out game trial and found problems:
  1. Defect in undo function: unlimited number of uses
  2. Sudden flashing/glitching phenomenon in the game
- Items pending testing:
  1. Game record (File I/O) function
  2. Perfection and robustness of the game algorithm

## Apr 22
- Bug Fixing by Person B:
  1. ~~Boxes on targets now locked (cannot be pushed out)~~ **Reverted on Apr 22: Boxes on targets can now be pushed out again**
  2. Undo limit per difficulty: Easy=5, Medium=3, Hard=0
  3. Undo no longer affects step count
  4. Added undo tracking to UserData; status bar shows "U: X/Y"
  5. Changed obstacle symbol from `.` to `%` since the previous `.` was too small can causes confusion.
  6. Added Leaderboard feature [L] in the Main Menu.

- Periodic Group Meeting 2
  1. Go through the game together again
  2. Reverted Changes: Boxes on targets can now be pushed out again (boxes on targets are no longer locked)

## Apr 22 (Later)
- OOP Refactoring for A+ Requirements:
  - Added `Game` class: encapsulates game logic, state management, input handling
  - Added `Renderer` class: encapsulates all display/rendering logic
  - Added subclasses `EasyGame`, `MediumGame`, `HardGame`: demonstrate polymorphism
  - Added subclass `RendererWithEffects`: demonstrates polymorphism (method overriding)
  - Demonstrates **Encapsulation**: private members, public interface
  - Demonstrates **Polymorphism**: virtual functions, method overriding
  - Code compiles without warnings (-Wall -Wextra)

