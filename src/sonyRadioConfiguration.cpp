#include "sonyRadioConfiguration.h"

namespace Keypad
{
void configureKeypadForSonyRadio(Keypad &keypad)
{
    keypad.addButton(0, 2, 865, 1065);
    keypad.addButton(1, 0, 865, 1065);
    keypad.addButton(2, 1, 865, 1065);
    keypad.addButton(3, 0, 0, 100);
    keypad.addButton(4, 1, 1162, 1362);
    keypad.addButton(5, 3, 320, 520);
    keypad.addButton(6, 2, 550, 750);
    keypad.addButton(7, 3, 0, 100);
    keypad.addButton(8, 1, 550, 750);
    keypad.addButton(9, 0, 550, 750);
    keypad.addButton(10, 0, 1162, 1362);
    keypad.addButton(11, 1, 0, 100);
    keypad.addButton(12, 2, 210, 410);
    keypad.addButton(13, 1, 210, 410);
    keypad.addButton(14, 0, 210, 410);
    keypad.addButton(15, 3, 765, 965);
}
}