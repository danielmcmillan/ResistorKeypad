#pragma once

#include <vector>
#include <array>
#include "ADS1x15.h"

namespace Keypad
{
/**
 * Class for interpretting ADC values to determine button presses
 */
class Keypad
{
private:
    struct Range
    {
        int id;
        ADS1x15::SingleEndedValue rangeStart;
        ADS1x15::SingleEndedValue rangeEnd;
    };
    std::array<std::vector<Range>, (unsigned)ADS1x15::Channel::maximum + 1> channels;
    ADS1x15::ADS1015 adc;

public:
    Keypad();

    /**
     * @brief Add a button definition for an analog range on a given channel.
     * @param id Number for the button used to identify it when pressed.
     * @param channel Number of the ADC channel associated with the button.
     * @param rangeStart Low end (inclusive) of range for analogue values associated with the button.
     * @param rangeEnd High end (exclusive) of range for analogue values associated with the button.
     */
    void addButton(int id, int channel, ADS1x15::SingleEndedValue rangeStart, ADS1x15::SingleEndedValue rangeEnd);

    /**
     * Check for a button press.
     * @param button Pointer to write id for button if one is pressed.
     * @return Whether any button is pressed.
     */
    bool buttonPressed(int *button);
};
}
