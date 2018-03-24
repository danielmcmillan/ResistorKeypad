#include "Keypad.h"

namespace Keypad
{
/**
 * Configure Keypad object for Sony Dream Machine iPod dock keypad.
 * 
 * Each button is given a range that is +/-100 from its measured analogue value.
 * 
 * |----|------------|---------|-------|
 * |    | Button     | Channel | Value |
 * |----|------------|---------|-------|
 * |  0 | Preset 1   | 2       |   965 |
 * |  1 | Preset 2   | 0       |   965 |
 * |  2 | Preset 3   | 1       |   965 |
 * |  3 | Preset 4   | 0       |     0 |
 * |  4 | Preset 5   | 1       |  1262 |
 * |  5 | Sleep      | 3       |   420 |
 * |  6 | MEGA Xpand | 2       |   650 |
 * |  7 | Snooze     | 3       |     0 |
 * |  8 | Volume -   | 1       |   650 |
 * |  9 | Volume +   | 0       |   650 |
 * | 10 | Audio In   | 0       |  1262 |
 * | 11 | Radio On   | 1       |     0 |
 * | 12 | iPod       | 2       |   310 |
 * | 13 | Previous   | 1       |   310 |
 * | 14 | Next       | 0       |   310 |
 * | 15 | Off        | 3       |   865 |
 * |----|------------|---------|-------|
 */
    void configureKeypadForSonyRadio(Keypad &keypad);
}