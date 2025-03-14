#pragma once
//
//    FILE: PrintCharArray.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.3.2
// PURPOSE: Class that captures prints into a char array
//    DATE: 2017-12-07
//     URL: https://github.com/RobTillaart/PrintCharArray
//
//  HISTORY
//  0.1.0   2017-12-07  initial version
//  0.1.1   2020-04-28  minor optimization
//  0.2.0   2020-04-30  dynamic memory
//  0.2.1   2020-06-19  fix library.json
//  0.3.0   2021-01-06  Arduino-CI + unit test, free -> available()
//  0.3.1   2021-11-13  update Arduino-CI, readme.md
//                      add conditional PRINTCHARARRAY_MAX_BUFFER_SIZE
//                      add write(char * str, uint8_t length) Print interface.
//  0.3.2   2021-12-24  update library.json, license, minor edits


#include "Print.h"


#define PRINTCHARARRAY_VERSION            (F("0.3.2"))

#ifndef PRINTCHARARRAY_MAX_BUFFER_SIZE
#define PRINTCHARARRAY_MAX_BUFFER_SIZE    250
#endif


class PrintCharArray: public Print
{
public:
  PrintCharArray(uint8_t size = 100)
  {
    _bufSize = constrain(size, 20, PRINTCHARARRAY_MAX_BUFFER_SIZE);
    _buffer = (char *) malloc(_bufSize);
  };


  ~PrintCharArray()
  {
    if (_buffer) free(_buffer);
  };


  size_t write(uint8_t c)
  {
    if (_index < _bufSize - 1)
    {
      _buffer[_index++] = c;
      return 1;
    }
    return 0;
  }


  size_t write(uint8_t * str, uint8_t length)
  {
    if ( (int(_index) + length) >= _bufSize) return 0;  // does not fit.

    uint8_t len = length;
    uint8_t i = 0;
    while (len--)
    {
      _buffer[_index++] = str[i++];
    }
    return length;
  }


  void clear()
  {
    _index = 0;
  }


  int available()
  { 
    return (_bufSize - _index);
  }


  // int length() { return _index; };   // better as size()?
  int size()
  { 
    return _index;
  }


  int bufSize()
  {
    return _bufSize;
  }


  char * getBuffer() 
  {
    _buffer[_index] = '\0';
    return _buffer; 
  }


private:
  char*   _buffer;
  uint8_t _bufSize = 0;
  uint8_t _index   = 0;
};


// -- END OF FILE --

