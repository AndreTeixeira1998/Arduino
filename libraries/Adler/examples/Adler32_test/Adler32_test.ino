//
//    FILE: Adler32_test.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo

#include "Arduino.h"
#include "Adler.h"

//  expected output
//  Adler32_test
//  116982386
//  117310067
//  117047923



Adler32 ad;

uint8_t arr1[5]  = { 100, 120, 130, 135, 140 };
uint8_t arr2[5]  = { 101, 120, 130, 135, 140 };  // minimal diff.
uint8_t arr3[5]  = { 100, 120, 130, 135, 141 };  // minimal diff.

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.println("Adler32_test");

  ad.begin();
  ad.add(arr1, 5);
  Serial.println(ad.getAdler());

  ad.begin();
  ad.add(arr2, 5);
  Serial.println(ad.getAdler());

  ad.begin();
  ad.add(arr3, 5);
  Serial.println(ad.getAdler());
}


void loop()
{
}


// -- END OF FILE --
