//
//    FILE: PinInGroup.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.6
//    DATE: 2017-04-26
// PURPOSE: PinInGroup library for Arduino
//          goal is to easily read a group of pins that logically
//          belong to each other.
//          The pins can be in any order.
//     URL: https://github.com/RobTillaart/PinInGroup
//          http://forum.arduino.cc/index.php?topic=469599.0
//
//  HISTORY

//  0.1.0   2017-08-20  initial version (based upon pinGroup)
//  0.1.1   2020-05-19  refactor; added clear();
//                      added param for INPUT or INPUT_PULLUP
//  0.1.2   2020-06-19  fix library.json
//  0.1.3   2021-01-05  add Arduino-CI + unit test
//  0.1.4   2021-01-22
//  0.1.5   2021-11-12  update build-CI
//                      update readme, badges
//                      rename variables for readability,
//                      add getIndex() to replace getIdx(),
//                      add getMaxSize(),
//                      fix version number and history.
//  0.1.6  2021-12-23   update library.json, license, minor edits


#include "PinInGroup.h"


PinInGroup::PinInGroup()
{
  clear();
}


void PinInGroup::clear()
{
  _size = 0;
}


uint8_t PinInGroup::add(uint8_t size, uint8_t * pinArray, uint8_t mode)
{
  int n = 0;
  for (uint8_t i = 0; i < size; i++)
  {
    n += add(pinArray[i], mode);
  }
  return n;
}


uint8_t PinInGroup::add(uint8_t pin, uint8_t mode)
{
  if (_size >= PININGROUP_MAXSIZE) return 0;

  _pins[_size] = pin;
  pinMode(pin, mode);
  _size++;
  return 1;
}


uint8_t PinInGroup::isInGroup(uint8_t pin)
{
  uint8_t count = 0;
  for (uint8_t i = 0; i < _size; i++)
  {
    if (_pins[i] == pin) count++;
  }
  return count;
}


uint16_t PinInGroup::read()
{
  uint16_t value = 0;
  uint16_t mask  = 0x0001;
  for (uint8_t i = 0; i < _size; i++)
  {
    if (digitalRead(_pins[i])) value |= mask;
    mask <<= 1;
  }
  return value;
}


uint16_t PinInGroup::read(uint8_t index)
{
  if (index >= _size) return 0xFFFF; // sort of error

  return (digitalRead(_pins[index])) ? 1 : 0;
}


uint8_t PinInGroup::getPin(uint8_t index)
{
  if (index >= _size) return 0xFF;
  return _pins[index];
}


uint8_t PinInGroup::getIndex(uint8_t pin)
{
  for (uint8_t i = 0; i < _size; i++)
  {
    if (_pins[i] == pin) return i;
  }
  return 0xFF;
}


// --- END OF FILE ---

