//
//    FILE: unit_test_001.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2022-02-11
// PURPOSE: unit tests for the X9C10X library
//          https://github.com/RobTillaart/X9C10X
//          https://github.com/Arduino-CI/arduino_ci/blob/master/REFERENCE.md
//

// supported assertions
// https://github.com/Arduino-CI/arduino_ci/blob/master/cpp/unittest/Assertion.h#L33-L42
// ----------------------------
// assertEqual(expected, actual)
// assertNotEqual(expected, actual)
// assertLess(expected, actual)
// assertMore(expected, actual)
// assertLessOrEqual(expected, actual)
// assertMoreOrEqual(expected, actual)
// assertTrue(actual)
// assertFalse(actual)
// assertNull(actual)
// assertNotNull(actual)

#include <ArduinoUnitTests.h>


#include "Arduino.h"
#include "X9C10X.h"



unittest_setup()
{
  fprintf(stderr, "X9C10X_LIB_VERSION: %s\n", (char *) X9C10X_LIB_VERSION);
}

unittest_teardown()
{
}


unittest(test_constructor)
{
  X9C10X dp0;
  X9C10X dp1(1000);
  X9C10X dp2(900);
  X9C10X dp3(1100);

  assertEqual(10000, dp0.getMaxOhm());
  assertEqual(1000,  dp1.getMaxOhm());
  assertEqual(900,   dp2.getMaxOhm());
  assertEqual(1100,  dp3.getMaxOhm());

  X9C102 x102;
  X9C103 x103;
  X9C104 x104;
  X9C503 x503;

  assertEqual(1000,   x102.getMaxOhm());
  assertEqual(10000,  x103.getMaxOhm());
  assertEqual(100000, x104.getMaxOhm());
  assertEqual(50000,  x503.getMaxOhm());
}


unittest(test_position)
{
  X9C10X dp0;

  dp0.begin(7, 8, 9);
  assertEqual(0, dp0.getPosition());

  fprintf(stderr, "setPosition step 9\n");
  for (uint8_t pos = 0; pos < 100; pos += 9)
  {
    dp0.setPosition(pos);
    assertEqual(pos, dp0.getPosition());
  }

  X9C10X dp1;
  dp1.begin(7, 8, 9, 50);
  assertEqual(50, dp1.getPosition());
}


unittest(test_incr_decr)
{
  X9C10X dp0;

  dp0.begin(7, 8, 9);
  assertEqual(0, dp0.getPosition());

  dp0.setPosition(0);
  for (uint8_t pos = 0; pos < 10; pos++)
  {
    assertEqual(pos, dp0.getPosition());
    dp0.incr();
  }

  for (uint8_t pos = 0; pos < 5; pos++)
  {
    assertEqual(10 - pos, dp0.getPosition());
    dp0.decr();
  }
}


unittest(test_getOhm)
{
  X9C10X dp0;

  dp0.begin(7, 8, 9);
  assertEqual(0, dp0.getPosition());
  assertEqual(10000, dp0.getMaxOhm());

  for (uint8_t pos = 0; pos < 100; pos += 9)
  {
    fprintf(stderr, "VALUE: %d %d Ω\n", dp0.getPosition(), dp0.getOhm());
    dp0.incr();
  }

}


unittest_main()

// --------
