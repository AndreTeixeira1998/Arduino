
[![Arduino CI](https://github.com/RobTillaart/MAX6675/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/MAX6675/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/MAX6675/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/MAX6675/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/MAX6675/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/MAX6675/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/MAX6675.svg?maxAge=3600)](https://github.com/RobTillaart/MAX6675/releases)


# MAX6675

Arduino library for MAX6675 chip for K type thermocouple. 

The library is based upon (stripped and adapted version of) the https://github.com/RobTillaart/MAX31855_RT library.

Experimental as not tested with hardware yet.

## Description

The MAX6675 is a chip to convert the reading of a K-type thermocouple to a temperature. The MAX6675 only supports positive degrees Celsius.

The working of thermocouples (TC) is based upon Seebeck effect.
Different TC's have a different Seebeck Coefficient (SC) expressed in �V/�C.
See http://www.analog.com/library/analogDialogue/archives/44-10/thermocouple.html

Library tested with breakout board - 

```
  TODO

```


## Hardware SPI vs software SPI

Default pin connections. ESP32 can overrule with **setGPIOpins()**.

 | HW SPI   |  UNO  |  ESP32 VSPI |  ESP32 HSPI | Notes
 |:---------|:-----:|:-----------:|:-----------:|:----------|
 | CLOCKPIN |   13  |   18        |   14        |
 | MISO     |   12  |   19        |   12        |
 | MOSI     |   11  |   23        |   13        |  *not used...*
 | SELECT   | eg. 4 |    5        |   15        |  *can be others too.*


Performance read() function, timing in us.  (TODO ESP32 @240MHz)

| mode   | clock    | timing UNO | timing ESP32 | Notes
|:-------|---------:|-----------:|-------------:|:----------|
| HW SPI |  4000000 |    ~32     |              | highest supported.
| HW SPI |  2000000 |    ~36     |              |
| HW SPI |  1000000 |    ~44     |              |
| HW SPI |   500000 |    ~60     |              |
| SW SPI | bit bang |    ~276    |              |



## Interface


### Constructor

- **MAX6675()** create object.
- **void begin(const uint8_t select)** set select pin => hardware SPI
- **void begin(const uint8_t sclk, const uint8_t select, const uint8_t miso)** set clock, select and miso pin => software SPI


### Hardware SPI

To be used only if one needs a specific speed.

- **void setSPIspeed(uint32_t speed)** set SPI transfer rate.
- **uint32_t getSPIspeed()** returns SPI transfer rate.
- **void setSWSPIdelay(uint16_t del = 0)** for tuning SW SPI signal quality. Del is the time in micros added per bit. Even numbers keep the duty cycle of the clock around 50%.
- **uint16_t getSWSPIdelay()** get set value in micros.


### ESP32 specific

- **void selectHSPI()** must be called before **begin()**
- **void selectVSPI()** must be called before **begin()**
- **bool usesHSPI()**
- **bool usesVSPI()**
- **void setGPIOpins(uint8_t clk, uint8_t miso, uint8_t mosi, uint8_t select)**  to overrule ESP32 default hardware pins.


### Reading

To make a temperature reading call **read()**.
It returns the status of the read which is a value between 0..7
The function **getStatus()** returns the same status value. 

Table: values returned from **uint8_t read()** and **uint8_t getStatus()**

| value | Description               | Action       |
|:-----:|:--------------------------|:-------------|
|    0  | OK                        |              |
|    1  | Thermocouple open circuit | check wiring |
|    2  | Thermocouple short to GND | check wiring |
|    4  | Thermocouple short to VCC | check wiring |
|    7  | Generic error             |              |
|  128  | No read done yet          | check wiring |
|  129  | No communication          | check wiring |

There are six functions to check the individual error conditions mentioned above.
These make it easier to check them.

- **bool openCircuit()**
- **bool shortToGND()**
- **bool shortToVCC()**
- **bool genericError()**
- **bool noRead()**
- **bool noCommunication()**

After a **uint8_t read()** you can get the temperature with **float getTemperature()** 
and **float getInternal()** for the internal temperature of the chip / board itself.
Normally these are (almost) equal.

Repeated calls to **getTemperature()** will give the same value until a new **read()**.
The latter fetches a new value from the sensor. Note that if the **read()** fails
the value of **getTemperature()** can become incorrect. So it is important to check 
the return value of **read()**.


### Offset

The library supports a fixed offset to calibrate the thermocouple.
For this the functions **float getOffset()** and **void setOffset(float offset)** are available.
This offset is "added" in the **getTemperature()** function.

Note the offset used is a float, so decimals can be used.


### Delta analysis

As the **tc** object holds its last known temperature it is easy to determine the delta 
with the last known temperature, e.g. for trend analysis.

```cpp
  float last = tc.getTemperature();
  int state  = tc.read();
  if (state == STATUS_OK)
  {
    float new  = tc.getTemperature();
    float delta = new - last;
    // process data
  }
```


### Last time read

The **tc** object keeps track of the last time **read()** is called in the function **uint32_t lastRead()**.
The time is tracked in **millis()**. This makes it easy to read the sensor at certain intervals.

```cpp
if (millis() - tc.lastRead() >= interval)
{
  int state = tc.read();
  if (state == STATUS_OK)
  {
    float new = tc.getTemperature();
    // process read value.
  }
  else
  {
    // handle error
  }
}
```


### GetRawData 

The function **uint32_t getRawData()** allows you to get all the 32 bits raw data from the board, 
after the standard **uint8_t tc.read()** call.

Example code can be found in the examples folder.

```cpp
  int state = thermocouple.read();              
  uint32_t value = thermocouple.getRawData();  // Read the raw Data value from the module
```

This allows one to compact the measurement e.g. for storage or sending over a network.


## Pull Up Resistor 

To have proper working of the MAX6675 board, you need to add a pull-up resistor 
(e.g. 4K7 - 1K depending on wire length) between the MISO pin (from constructor call) and the 
VCC (5Volt). This improves the signal quality and will allow you to detect if there is
proper communication with the board. Without pull-up one might get random noise that could 
look like real data.

**Note:** the MISO pin can be different from each board, please refer to your board datasheet.

If the MAX6675 board is not connected **tc.read()** will return **STATUS_NO_COMMUNICATION**.

You can verify this by **tc.getRawData()** which will give 16 HIGH bits or 0xFFFF).

You can use a simple code to detect connection error board:

```cpp
  uint8_t status = thermocouple.read();
  if (status == STATUS_NO_COMMUNICATION)
  {
    Serial.println("NO COMMUNICATION");
  }
```

or

```cpp
  uint8_t status = thermocouple.read();
  if (thermocouple.getRawData() == 0xFFFF)
  {
    Serial.println("NO COMMUNICATION");
  }
```


## Operation

See examples


## Future

- update and verify documentation (as it is copied from MAX31855 lib)
- keep interface in sync with MAX31855 if possible.
- 

