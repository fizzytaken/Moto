
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
struct led orange = {200,200,0,0};
struct led sb_orange = {50,50,0,20};

#endif