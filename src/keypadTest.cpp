
#include <iostream>
#include "Reader.h"
#include "EventEmitter.h"
#include "sonyRadioConfiguration.h"

int main(void)
{
    ResistorKeypad::Reader reader;
    configureReaderForSonyRadio(reader);

    // Register a button press listener and asynchronously read from the keypad
    ResistorKeypad::EventEmitter emitter(reader, std::chrono::milliseconds(50)); // Event emitter that reads the keypad every 50 milliseconds
    emitter.addListener([](ResistorKeypad::EventEmitter::ButtonPressedEvent event) {
        std::cout << "Button " << event.buttonId << " was just pressed" << std::endl;
    });

    std::cin.get();
}
