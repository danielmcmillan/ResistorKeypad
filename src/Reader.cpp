#include "Reader.h"

namespace ResistorKeypad
{
    Reader::Reader()
    {
        adc.setGain(ADS1x15::Gain::one);
        adc.begin();
    }

    void Reader::addButton(int id, int channel, ADS1x15::SingleEndedValue rangeStart, ADS1x15::SingleEndedValue rangeEnd)
    {
        Range range = {id, rangeStart, rangeEnd};
        channels[channel].push_back(range);
    }

    bool Reader::buttonPressed(int *button)
    {
        for (int c = 0; c < channels.size(); ++c)
        {
            ADS1x15::Channel channel = (ADS1x15::Channel)c;
            // Read the analogue value for this channel
            ADS1x15::SingleEndedValue value = adc.read(channel);
            // Check each configured button against the analogue value
            std::vector<Range> &ranges = channels[c];
            for (auto range = ranges.begin(); range != ranges.end(); ++range)
            {
                if (value >= range->rangeStart && value < range->rangeEnd)
                {
                    *button = range->id;
                    return true;
                }
            }
        }
        return false;
    }
}
