
[![Arduino CI](https://github.com/RobTillaart/PCA9685_RT/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/PCA9685_RT/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/PCA9685_RT/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/PCA9685_RT/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/PCA9685_RT/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/PCA9685_RT/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/PCA9685_RT.svg?maxAge=3600)](https://github.com/RobTillaart/PCA9685_RT/releases)


# PCA9685_RT

Arduino library for I2C PCA9685 16 channel PWM extender.


# Description

This library is to control the I2C PCA9685 PWM extender. 
The 16 channels are independently configurable in steps of 1/4096.
This allows for better than 0.1% fine tuning of the duty-cycle
of the PWM signal. 

The PWM's of the different channels have individual start and stop moments. 
This can be used to distribute the power more evenly over multiple servo's 
or give special effects when used in an RGB LED.

The frequency of the PWM can be set from 24 to 1526 according to the datasheet, 
however in practice not all frequencies are set accurate.
Lower frequencies do better than higher frequencies.


## Interface

### Constructor

- **PCA9685(uint8_t deviceAddress, TwoWire \* wire = &Wire)** I2C address + optional I2C interface.
- **bool begin()** initializes the library after startup. Mandatory.
- **bool begin(uint8_t sda, uint8_t scl)** idem, ESP32 ESP8266 only. 
Library does not support multiple Wire instances (yet).
- **void reset()** resets the library to start up conditions.
- **bool isConnected()** checks if address is available on I2C bus.
- **uint8_t channelCount()** returns the number of channels = 16.


### Mode

- **writeMode(uint8_t reg, uint8_t value)** configuration of one of the two configuration registers.
check datasheet for details.
- **uint8_t readMode(reg)** reads back the configured mode, 
useful to add or remove a single flag (bit masking)


### PWM

- **void setPWM(uint8_t channel, uint16_t onTime, uint16_t offTime)** The chip has 16 channels to do PWM.
The signal is divided in 4096 steps, 0..4095. 
The pulse can begin **onTime** on any step and it can stop on any step **offTime**.
This allows e.g. to distribute the power over the 16 channels, e.g. the
channels do not need to start at the same moment with HIGH.
- **void setPWM(uint8_t channel, offTime)** simple PWM that always start on **onTime = 0**.
- **void getPWM(uint8_t channel, uint16_t \* onTime, uint16_t \* offTime)** 
read back the configuration of the channel.
- **void allOFF()** switches all PWM channels OFF. **Experimental** in 0.3.0.
To "undo" the allOFF one can call the **reset()** function and set all 
PWM channels again.
- **void digitalWrite(channel, mode)** mode = HIGH or LOW, just use the PCA9685 as 
a digital pin. 
This single function replaces the setON() and setOFF() that will become
obsolete in the future.


### Frequency 

- **void setFrequency(uint16_t freq, int offset = 0)** set the update speed of the channels. 
This value is set the same for all channels at once.
- **int getFrequency(bool cache = true)** get the current update frequency of the channels. 
This is same for all channels. If cache is false, the frequency is fetched and
calculated from the **preScaler** register and will probably differ from the 
value set with **setFrequency()**.

The frequency is constrained to be between 24 and 1526 Hz.
As the frequency is converted to an 8 bit **preScaler**,
the frequency set will seldom be exact.
After changing the frequency, one must set all channels (again), 
so one should set the frequency in **setup()**

The parameter offset can be used to tune the **preScaler** to get a frequency
closer to the requested value. See **PCA9685_setFrequency_offset** example. 
Default the offset = 0. As the **preScaler** is smaller at higher frequencies 
higher frequencies are less accurate.
Making offset too large can result in very incorrect frequencies.

When using offset, the **getFrequency(false)** will return the adjusted **preScaler**.


### Miscellaneous

- **int lastError()** returns **PCA9685_OK = 0** if all is OK, and 

| Error code          | Value | Description |
|:--------------------|:-----:|:------------|
| PCA9685_OK          | 0x00  | Everything went well
| PCA9685_ERROR       | 0xFF  | generic error
| PCA9685_ERR_CHANNEL | 0xFE  | Channel out of range
| PCA9685_ERR_MODE    | 0xFD  | Invalid mode register chosen    | 
| PCA9685_ERR_I2C     | 0xFC  | PCA9685 I2C communication error |


## Operation

See examples


## Future

- improve documentation
- add unit tests (if possible)
- investigate set/getFrequency int vs uint16_t ?
-



