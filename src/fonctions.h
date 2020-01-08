#include <Arduino.h>
#ifndef FONCTIONS_H
#define FONCTIONS_H

/* ----- Define Functions ----- */
void standby();
void droite(int);
void gauche(int);
void clean_leds();
void init_leds();
void status_BLE(int);

/* ----- Define STRUCTS ----- */
struct led {
  int g; 
  int r;
  int b;
  int w;
};

struct Button {
  const uint8_t PIN;
};


/* ----- Color Cofiguration ----- */
struct led rouge = {0,25,0,0};
struct led orange = {200,200,0,0};
struct led sb_orange = {50,50,0,10};

#endif