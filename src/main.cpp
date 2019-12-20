#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include "fonctions.h"


// Which pin on the Arduino is connected to the NeoPixels?
#define Pin_np_Gauche    6
#define Pin_np_Droite    7
#define btn_pin 2
 
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 6
 
Adafruit_NeoPixel np_gauche(LED_COUNT, Pin_np_Gauche, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel np_droite(LED_COUNT, Pin_np_Droite, NEO_RGBW + NEO_KHZ800);

int mode = 0; //Init Mode 
/*
0 -> Init Mode
1 -> Cligno Droit
2 -> Cligno Gauche
11 -> Cligno Frein Droit
12 -> Cligno Frein Gauche
*/

/* ----- FONCTIONS ----- */
void clean_leds();
void droite(int);
void droite_frein(int);
void gauche(int);
void gauche_frein(int);
void init_leds();
void stop();


/* ----- TRANSITIONS ----- */
void frein();
void no_frein();

void setup() {

  Serial.begin(9600);

  pinMode(btn_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn_pin), frein, HIGH);

  init_leds();
  clean_leds();

  Serial.write("BOOT OK\r\n");
}

void loop() {
 Serial.println(mode);

 switch (mode) {
      case 0:
        mode = 1;
        break;

      case 1:
        droite(200);
        mode = 2;
        break;

      case 2:
        gauche(200);
        mode = 1;
        break;
      
      case 11:
        droite_frein(200);
        break;

      case 12:
        gauche_frein(200);
        break;
        
      default:
        break;
 }
}


/* ------- FONCTIONS -------- */

void droite(int time_del) {
  
  for (int i=0;i<6;i++)
  {
    np_droite.setPixelColor(i, orange.g, orange.r, orange.b, orange.w);
    np_droite.show();
    delay(time_del);
  }
  clean_leds();
  
}

void droite_frein(int time_del)
{
  
  for (int i=3;i<6;i++)
  {
    np_droite.setPixelColor(0, rouge.g, rouge.r, rouge.b, rouge.w);
    np_droite.setPixelColor(1, rouge.g, rouge.r, rouge.b, rouge.w);
    np_droite.setPixelColor(2, rouge.g, rouge.r, rouge.b, rouge.w);
    np_droite.setPixelColor(3, rouge.g, rouge.r, rouge.b, rouge.w);

    np_droite.setPixelColor(i, orange.g, orange.r, orange.b, orange.w);
    np_droite.show();
    delay(time_del*2);
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

void gauche_frein(int time_del)
{
  for (int i=3;i<6;i++)
  {
    np_gauche.setPixelColor(0, rouge.g, rouge.r, rouge.b, rouge.w);
    np_gauche.setPixelColor(1, rouge.g, rouge.r, rouge.b, rouge.w);
    np_gauche.setPixelColor(2, rouge.g, rouge.r, rouge.b, rouge.w);
    np_gauche.setPixelColor(3, rouge.g, rouge.r, rouge.b, rouge.w);

    np_gauche.setPixelColor(i, orange.g, orange.r, orange.b, orange.w);
    np_gauche.show();
    delay(time_del*2);
  }
  clean_leds();
}


void stop()
{ 
  int time_del= 300;
  while (digitalRead(btn_pin) == HIGH)
  {
  for (int i=0;i<6;i++)
  {
    np_gauche.setPixelColor(i, rouge.g, rouge.r, rouge.b, rouge.w);
    np_droite.setPixelColor(i, rouge.g, rouge.r, rouge.b, rouge.w);
  }
  np_gauche.show();
  np_droite.show();

  delay(time_del);
  clean_leds();
  delay(time_del);
}}

void clean_leds(){

  for (int i=0;i<6;i++)
  {
    np_gauche.setPixelColor(i, 0, 0, 0, 0);
    np_droite.setPixelColor(i, 0, 0, 0, 0);
  }
  np_gauche.show();
  np_droite.show();
}

void init_leds(){
  np_gauche.begin();
  np_droite.begin();

}


/* ----- TRANSITIONS ----- */

void frein()
{
 if (mode == 1){mode = 11;}
 else if (mode == 2){mode = 12;}
}

