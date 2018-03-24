#include "sonyRadioConfiguration.h"

namespace ResistorKeypad
{
void configureReaderForSonyRadio(Reader &reader)
{
    reader.addButton(0, 2, 865, 1065);
    reader.addButton(1, 0, 865, 1065);
    reader.addButton(2, 1, 865, 1065);
    reader.addButton(3, 0, 0, 100);
    reader.addButton(4, 1, 1162, 1362);
    reader.addButton(5, 3, 320, 520);
    reader.addButton(6, 2, 550, 750);
    reader.addButton(7, 3, 0, 100);
    reader.addButton(8, 1, 550, 750);
    reader.addButton(9, 0, 550, 750);
    reader.addButton(10, 0, 1162, 1362);
    reader.addButton(11, 1, 0, 100);
    reader.addButton(12, 2, 210, 410);
    reader.addButton(13, 1, 210, 410);
    reader.addButton(14, 0, 210, 410);
    reader.addButton(15, 3, 765, 965);
}
}