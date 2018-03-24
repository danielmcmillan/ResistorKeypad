Library for handling presses of buttons arranged in a resistor ladder and connected to a ADS1015 ADC on Raspberry Pi I2C bus.

## Building

```
cmake <root of repository>
make
```

## Usage

```
#include "Reader.h"
#include "EventEmitter.h"
...
ResistorKeypad::Reader reader;
reader.addButton(1, 0, 500, 700) // Button 1 is connected to ADC channel 0 with analogue value [500,700)

// Check which button is pressed (resistor ladder cannot be used to check for simultaneous button presses)
int button;
if (reader.buttonPressed(&button))
{
	std::cout << "Button " << button << " is pressed" << std::endl;
}

// Register a button press listener and asynchronously read from the keypad
ResistorKeypad::EventEmitter emitter(reader, std::chrono::milliseconds(50)); // Event emitter that reads the keypad every 50 milliseconds
emitter.addListener([](ResistorKeypad::EventEmitter::ButtonPressedEvent event) {
	std::cout << "Button " << event.buttonId << " was just pressed" << std::endl;
});
...
```