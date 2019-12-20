
#include <Arduino.h>
#ifndef FONCTIONS_H
#define FONCTIONS_H

struct led {
  int g; 
  int r;
  int b;
  int w;
};

// Green, Red, Blue, White
struct led rouge = {0,25,0,0};
struct led orange = {15,15,0,0};

#endif