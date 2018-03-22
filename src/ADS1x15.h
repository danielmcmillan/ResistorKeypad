// Based on source code from Adafruit Industries released under the following license.

// Software License Agreement (BSD License)

// Copyright (c) 2012, Adafruit Industries
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holders nor the
// names of its contributors may be used to endorse or promote products
// derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#pragma once
#include <cstdint>

namespace ADS1x15
{
/**
 * Default I2C address for the ADC
 */
const uint8_t address = 0x48;

enum class Gain
{
    twoThirds,
    one,
    two,
    four,
    eight,
    sixteen
};

enum class Channel: uint8_t
{
    zero = 0,
    one = 1,
    two = 2,
    three = 3
};

enum class DifferentialPair
{
    channel01,
    channel23
};

/*
 * Pointers to register locations
 */
struct RegisterAddress
{
    static const uint8_t mask = 0x03;
    static const uint8_t convert = 0x00;
    static const uint8_t config = 0x01;
    static const uint8_t lowThresh = 0x02;
    static const uint8_t hiThresh = 0x03;
};

/*
 * Configuration flags for the CONFIG register
 */
struct ConfigFlag
{
    static const uint16_t osMask = 0x8000;
    static const uint16_t osSingle = 0x8000;  // Write: Set to start a single-conversion
    static const uint16_t osBusy = 0x0000;    // Read: Bit = 0 when conversion is in progress
    static const uint16_t osNotBusy = 0x8000; // Read: Bit = 1 when device is not performing a conversion
    static const uint16_t muxMask = 0x7000;
    static const uint16_t muxDiff01 = 0x0000;  // Differential P = AIN0, N = AIN1 (default)
    static const uint16_t muxDiff03 = 0x1000;  // Differential P = AIN0, N = AIN3
    static const uint16_t muxDiff13 = 0x2000;  // Differential P = AIN1, N = AIN3
    static const uint16_t muxDiff23 = 0x3000;  // Differential P = AIN2, N = AIN3
    static const uint16_t muxSingle0 = 0x4000; // Single-ended AIN0
    static const uint16_t muxSingle1 = 0x5000; // Single-ended AIN1
    static const uint16_t muxSingle2 = 0x6000; // Single-ended AIN2
    static const uint16_t muxSingle3 = 0x7000; // Single-ended AIN3
    static const uint16_t pgaMask = 0x0E00;
    static const uint16_t pga6144v = 0x0000; // +/-6.144V range = Gain 2/3
    static const uint16_t pga4096v = 0x0200; // +/-4.096V range = Gain 1
    static const uint16_t pga2048v = 0x0400; // +/-2.048V range = Gain 2 (default)
    static const uint16_t pga1024v = 0x0600; // +/-1.024V range = Gain 4
    static const uint16_t pga0512v = 0x0800; // +/-0.512V range = Gain 8
    static const uint16_t pga0256v = 0x0A00; // +/-0.256V range = Gain 16
    static const uint16_t modeMask = 0x0100;
    static const uint16_t modeContin = 0x0000; // Continuous conversion mode
    static const uint16_t modeSingle = 0x0100; // Power-down single-shot mode (default)
    static const uint16_t drMask = 0x00E0;
    static const uint16_t dr128sps = 0x0000;  // 128 samples per second
    static const uint16_t dr250sps = 0x0020;  // 250 samples per second
    static const uint16_t dr490sps = 0x0040;  // 490 samples per second
    static const uint16_t dr920sps = 0x0060;  // 920 samples per second
    static const uint16_t dr1600sps = 0x0080; // 1600 samples per second (default)
    static const uint16_t dr2400sps = 0x00A0; // 2400 samples per second
    static const uint16_t dr3300sps = 0x00C0; // 3300 samples per second
    static const uint16_t cmodeMask = 0x0010;
    static const uint16_t cmodeTrad = 0x0000;   // Traditional comparator with hysteresis (default)
    static const uint16_t cmodeWindow = 0x0010; // Window comparator
    static const uint16_t cpolMask = 0x0008;
    static const uint16_t cpolActvlow = 0x0000; // ALERT/RDY pin is low when active (default)
    static const uint16_t cpolActvhi = 0x0008;  // ALERT/RDY pin is high when active
    static const uint16_t clatMask = 0x0004;    // Determines if ALERT/RDY pin latches once asserted
    static const uint16_t clatNonlat = 0x0000;  // Non-latching comparator (default)
    static const uint16_t clatLatch = 0x0004;   // Latching comparator
    static const uint16_t cqueMask = 0x0003;
    static const uint16_t cque1conv = 0x0000; // Assert ALERT/RDY after one conversions
    static const uint16_t cque2conv = 0x0001; // Assert ALERT/RDY after two conversions
    static const uint16_t cque4conv = 0x0002; // Assert ALERT/RDY after four conversions
    static const uint16_t cqueNone = 0x0003;  // Disable the comparator and put ALERT/RDY in high state (default)

    static uint16_t forGain(Gain gain)
    {
        switch (gain)
        {
        case Gain::twoThirds:
            return pga6144v;
        case Gain::one:
            return pga4096v;
        case Gain::two:
            return pga2048v;
        case Gain::four:
            return pga1024v;
        case Gain::eight:
            return pga0512v;
        case Gain::sixteen:
            return pga0256v;
        }
    }

    static uint16_t forChannel(Channel channel)
    {
        switch (channel)
        {
        case (Channel::zero):
            return muxSingle0;
        case (Channel::one):
            return muxSingle1;
        case (Channel::two):
            return muxSingle2;
        case (Channel::three):
            return muxSingle3;
        }
    }
};

class ADS1015
{
  protected:
    uint8_t m_i2cAddress;
    uint8_t m_conversionDelay;
    uint8_t m_bitShift;
    Gain m_gain;
    int m_i2c_file = 0;

  public:
    ADS1015(uint8_t i2cAddress = address);
    void begin(void);
    uint16_t read(Channel channel);
    int16_t readDifferential(DifferentialPair channels);
    void startComparator(Channel channel, int16_t threshold);
    int16_t getLastConversionResults();
    void setGain(Gain gain);
    Gain getGain(void);

  private:
    static const int conversionDelay = 1;
};

// Derive from ADS1105 & override construction to set properties
class ADS1115 : public ADS1015
{
  public:
    ADS1115(uint8_t i2cAddress = address);

  private:
    static const int conversionDelay = 8;
};
}
