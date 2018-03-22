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

#include "ADS1x15.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"

namespace ADS1x15
{
/**
 * Swap higher and lower bytes of a 16 bit integer
 */
static uint16_t swapEndianness(uint16_t value)
{
    return (value >> 8) | ((value & 0xFF) << 8);
}

/**
 * @brief Writes 16-bits to the specified destination register
 */
static void writeRegister(int file, uint8_t reg, uint16_t value)
{
    wiringPiI2CWriteReg16(file, reg, swapEndianness(value));
}

/**
 * @brief Writes 16-bits to the specified destination register
 */
static uint16_t readRegister(int file, uint8_t reg)
{
    return swapEndianness(wiringPiI2CReadReg16(file, reg));
}

/**
 * @brief Instantiates an object for reading from an ADS1015
 */
ADS1015::ADS1015(uint8_t i2cAddress)
{
    m_i2cAddress = i2cAddress;
    m_conversionDelay = ADS1015::conversionDelay;
    m_bitShift = 4;
    m_gain = Gain::twoThirds;
}

/**
 * @brief Instantiates an object for reading from an ADS1115
 */
ADS1115::ADS1115(uint8_t i2cAddress)
{
    m_i2cAddress = i2cAddress;
    m_conversionDelay = ADS1115::conversionDelay;
    m_bitShift = 0;
    m_gain = Gain::twoThirds;
}

/**
 * @brief Sets up the HW (reads coefficients values, etc.)
 */
void ADS1015::begin()
{
    m_i2c_file = wiringPiI2CSetup(m_i2cAddress);
}

/**
 * @brief Sets the gain and input voltage range
 */
void ADS1015::setGain(Gain gain)
{
    m_gain = gain;
}

/**
 * @brief Gets a gain and input voltage range
 */
Gain ADS1015::getGain()
{
    return m_gain;
}

/**
 * @brief Gets a single-ended ADC reading from the specified channel
 */
SingleEndedValue ADS1015::read(Channel channel)
{
    // Configuration register values
    uint16_t config = ConfigFlag::cqueNone | // Disable the comparator (default val)
                      ConfigFlag::clatNonlat | // Non-latching (default val)
                      ConfigFlag::cpolActvlow | // Alert/Rdy active low (default val)
                      ConfigFlag::cmodeTrad | // Traditional comparator (default val)
                      ConfigFlag::dr1600sps | // 1600 samples per second (default)
                      ConfigFlag::modeSingle | // Single-shot mode (default)
                      ConfigFlag::forGain(m_gain) | // PGA/voltage range
                      ConfigFlag::forChannel(channel); // Single-ended input channel

    // Set 'start single-conversion' bit
    config |= ConfigFlag::osSingle;

    // Write config register to the ADC
    writeRegister(m_i2c_file, RegisterAddress::config, config);

    // Wait for the conversion to complete
    delay(m_conversionDelay);

    // Read the conversion results
    // Shift 12-bit results right 4 bits for the ADS1015
    return readRegister(m_i2c_file, RegisterAddress::convert) >> m_bitShift;
}

/**
 * @brief Reads the conversion results, measuring the voltage difference between the P (AIN0/2) and
 *  N (AIN1/3) input. Generates a signed value since the difference can be either positive or
 *  negative.
 */
DifferentialValue ADS1015::readDifferential(DifferentialPair channels)
{
    // Start with default values
    uint16_t config = ConfigFlag::cqueNone | // Disable the comparator (default val)
                      ConfigFlag::clatNonlat | // Non-latching (default val)
                      ConfigFlag::cpolActvlow | // Alert/Rdy active low (default val)
                      ConfigFlag::cmodeTrad | // Traditional comparator (default val)
                      ConfigFlag::dr1600sps | // 1600 samples per second (default)
                      ConfigFlag::modeSingle | // Single-shot mode (default)
                      ConfigFlag::forGain(m_gain); // PGA/voltage range

    // Set channels
    switch (channels)
    {
    case DifferentialPair::channel01:
        config |= ConfigFlag::muxDiff01; // AIN0 = P, AIN1 = N
        break;
    case DifferentialPair::channel23:
        config |= ConfigFlag::muxDiff23; // AIN2 = P, AIN3 = N
        break;
    }

    // Set 'start single-conversion' bit
    config |= ConfigFlag::osSingle;

    // Write config register to the ADC
    writeRegister(m_i2c_file, RegisterAddress::config, config);

    // Wait for the conversion to complete
    delay(m_conversionDelay);

    // Read the conversion results
    uint16_t res = readRegister(m_i2c_file, RegisterAddress::convert) >> m_bitShift;
    if (m_bitShift == 0)
    {
        return (int16_t)res;
    }
    else
    {
        // Shift 12-bit results right 4 bits for the ADS1015,
        // making sure we keep the sign bit intact
        if (res > 0x07FF)
        {
            // negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
        return (int16_t)res;
    }
}

/**
 * @brief Sets up the comparator to operate in basic mode, causing the ALERT/RDY pin to assert
 *  (go from high to low) when the ADC value exceeds the specified threshold.
 *
 * This will also set the ADC in continuous conversion mode.
 */
void ADS1015::startComparator(Channel channel, int16_t threshold)
{
    // Configuration register values
    uint16_t config = ConfigFlag::cque1conv | // Comparator enabled and asserts on 1 match
                      ConfigFlag::clatLatch | // Latching mode
                      ConfigFlag::cpolActvlow | // Alert/Rdy active low (default val)
                      ConfigFlag::cmodeTrad | // Traditional comparator (default val)
                      ConfigFlag::dr1600sps | // 1600 samples per second (default)
                      ConfigFlag::modeContin | // Continuous conversion mode
                      ConfigFlag::modeContin | // Continuous conversion mode
                      ConfigFlag::forGain(m_gain) | // PGA/voltage range
                      ConfigFlag::forChannel(channel); // Single-ended input channel

    // Set the high threshold register
    // Shift 12-bit results left 4 bits for the ADS1015
    writeRegister(m_i2c_file, RegisterAddress::hiThresh, threshold << m_bitShift);

    // Write config register to the ADC
    writeRegister(m_i2c_file, RegisterAddress::config, config);
}

/**
 * @brief In order to clear the comparator, we need to read the conversion results. This function
 *  reads the last conversion results without changing the config value.
 */
DifferentialValue ADS1015::getLastConversionResults()
{
    // Wait for the conversion to complete
    delay(m_conversionDelay);

    // Read the conversion results
    uint16_t res = readRegister(m_i2c_file, RegisterAddress::convert) >> m_bitShift;
    if (m_bitShift == 0)
    {
        return (int16_t)res;
    }
    else
    {
        // Shift 12-bit results right 4 bits for the ADS1015,
        // making sure we keep the sign bit intact
        if (res > 0x07FF)
        {
            // negative number - extend the sign to 16th bit
            res |= 0xF000;
        }
        return (int16_t)res;
    }
}
}
