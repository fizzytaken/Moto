#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "fonctions.h"

/* ----- DEFINE ----- */
//Neopixel 
  #define LED_COUNT 6

  #define Pin_np_Gauche    5
  #define Pin_np_Droite    18

//BLE
  #define SERVICE_UUID           "c76b3e01-9978-4ab9-9675-f3e59dcb7bb1" // Custom UUID
  #define CHARACTERISTIC_UUID_TX "c76b3e03-9978-4ab9-9675-f3e59dcb7bb1"

//PushButton
  Button Inter_G = {21};
  Button Inter_S = {23};
  Button Inter_D = {22};


/* ----- INIT NeoPixel ----- */
  Adafruit_NeoPixel np_gauche(LED_COUNT, Pin_np_Gauche, NEO_RGBW + NEO_KHZ800);
  Adafruit_NeoPixel np_droite(LED_COUNT, Pin_np_Droite, NEO_RGBW + NEO_KHZ800);


/* -----  INIT BT  -----*/
  BLECharacteristic *pCharacteristic;
  bool deviceConnected = false;
  float txValue = 0;

int mode = 0; 

/* -----  CALLBACKS  ----- */

  class MyServerCallbacks: public BLEServerCallbacks {
      void onConnect(BLEServer* pServer) {
        deviceConnected = true;
      };
      
      void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
      }
  };

  void IRAM_ATTR To_Stop() {
    mode = 1;
    Serial.println("Mode 1");
  }

  void IRAM_ATTR To_Gauche() {
    mode = 2;
    Serial.println("Mode 2");
  }

  void IRAM_ATTR To_Droite() {
    mode = 3;
    Serial.println("Mode 3");
  }

void setup() {
  Serial.begin(9600);
  
  pinMode(Inter_G.PIN, INPUT_PULLUP);
  pinMode(Inter_S.PIN, INPUT_PULLUP);
  pinMode(Inter_D.PIN, INPUT_PULLUP);

  attachInterrupt(Inter_G.PIN, To_Gauche, FALLING);
  attachInterrupt(Inter_S.PIN, To_Stop, FALLING);
  attachInterrupt(Inter_D.PIN, To_Droite, FALLING);

  // Create the BLE Device
  BLEDevice::init("CB500_Fred"); // Give it a name

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("BLE Server Ready... ");

  init_leds();
  clean_leds();
}

void loop() {

  if (deviceConnected) {

    txValue =  mode;
    char txString[8]; // make sure this is big enuffz
    dtostrf(txValue, 1, 1, txString); // float_val, min_width, digits_after_decimal, char_buffer
    pCharacteristic->setValue(txString);
    pCharacteristic->notify(); // Send the value to the app!
  }

 switch (mode) {
      case 0:
        init_leds();
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
    np_droite.setPixelColor(i, sb_orange.r, sb_orange.g, sb_orange.b, sb_orange.w);
    np_gauche.setPixelColor(i, sb_orange.r, sb_orange.g, sb_orange.b, sb_orange.w);
    
    np_droite.show();
    np_gauche.show();
    
  }
}

void droite(int time_del) {
  
  for (int i=0;i<6;i++)
  {
    np_droite.setPixelColor(i, orange.r, orange.g, orange.b, orange.w);
    np_droite.show();
    delay(time_del);
  }
  clean_leds();
}

void gauche(int time_del) {
  
  for (int i=0;i<6;i++)
  {
    np_gauche.setPixelColor(i, orange.r, orange.g, orange.b, orange.w);
    np_gauche.show();
    delay(time_del);
  }
  clean_leds();
}

void clean_leds(){

  for (int i=0;i<6;i++)
  {
    np_droite.setPixelColor(i, sb_orange.r, sb_orange.g, sb_orange.b, sb_orange.w);
    np_gauche.setPixelColor(i, sb_orange.r, sb_orange.g, sb_orange.b, sb_orange.w);

  }
  np_droite.show();
  np_gauche.show();
}

void init_leds(){
  np_gauche.begin();
  np_droite.begin();
  mode = 1;
}
