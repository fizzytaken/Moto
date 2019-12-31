#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "fonctions.h"


// Which pin on the Arduino is connected to the NeoPixels?
#define Pin_np_Gauche    6
#define Pin_np_Droite    5

#define btn_droite 7
#define btn_gauche 2
#define btn_stop 3
 
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 6
 
Adafruit_NeoPixel np_gauche(LED_COUNT, Pin_np_Gauche, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel np_droite(LED_COUNT, Pin_np_Droite, NEO_RGBW + NEO_KHZ800);

int mode = 0; //Init Mode 
/*
0 -> Init Mode
1 -> StandBy Mode
2 -> Cligno Droit
3 -> Cligno Gauche
*/

/* ----- FONCTIONS ----- */
void init_leds();
void clean_leds();
void standby();

void droite(int);
void gauche(int);

void sw_droite();
void sw_gauche();
void sw_stop();


/* ----- TRANSITIONS ----- */

void setup() {

  Serial.begin(9600);

  pinMode(btn_droite, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn_droite), sw_droite, HIGH);

  pinMode(btn_gauche, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn_gauche), sw_gauche, HIGH);

  pinMode(btn_stop, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn_stop), sw_stop, HIGH);

  init_leds();
  clean_leds();


  Serial.write("BOOT OK\r\n");
}

void loop() {
 Serial.println(mode);

 switch (mode) {
      case 0:
        init_leds();
        mode = 1;
        break;

      case 1:
        standby();
        break;

      case 2:
        droite(200);
        break;

      case 3:
        gauche(200);
        break;      

      default:
        break;
 }
}


/* ------- FONCTIONS -------- */

void standby() {
  
  for (int i=0;i<6;i++)
  {
    np_droite.setPixelColor(i, sb_orange.g, sb_orange.r, sb_orange.b, sb_orange.w);
    np_gauche.setPixelColor(i, sb_orange.g, sb_orange.r, sb_orange.b, sb_orange.w);
    
    np_droite.show();
    np_gauche.show();
    
  }
}

void droite(int time_del) {
  
  for (int i=0;i<6;i++)
  {
    np_droite.setPixelColor(i, orange.g, orange.r, orange.b, orange.w);
    np_droite.show();
    delay(time_del);
  }
  clean_leds();
}

void gauche(int time_del) {
  
  for (int i=0;i<6;i++)
  {
    np_gauche.setPixelColor(i, orange.g, orange.r, orange.b, orange.w);
    np_gauche.show();
    delay(time_del);
  }
  clean_leds();
}

void clean_leds(){

  for (int i=0;i<6;i++)
  {
    np_droite.setPixelColor(i, sb_orange.g, sb_orange.r, sb_orange.b, sb_orange.w);
    np_gauche.setPixelColor(i, sb_orange.g, sb_orange.r, sb_orange.b, sb_orange.w);

  }
  np_droite.show();
  np_gauche.show();
}

void init_leds(){
  np_gauche.begin();
  np_droite.begin();
}

/* --- Transitions --- */

void sw_stop(){
  mode = 1;
}

void sw_droite(){
  mode = 2;
}

void sw_gauche(){
  mode = 3;
}