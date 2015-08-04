/*
 * http://playground.arduino.cc/Code/Struct
 * http://forum.arduino.cc/index.php?topic=43587.0
 * http://forum.arduino.cc/index.php?topic=44890.0
 */
#ifndef MyTypes_h
#define MyTypes_h

#include <WString.h>

typedef struct {
  byte R;
  byte G;
  byte B;
  String Sens;
} STHumidex;

#endif
