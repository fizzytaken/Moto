
#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#include "BLEDevice.h"
#include "fonctions.h"

 #define LED_COUNT 7

  #define Pin_np_Gauche    5
  #define Pin_np_Droite    18

//Server to connect UUIDs 
static BLEUUID serviceUUID("042bd80f-14f6-42be-a45c-a62836a4fa3f");
static BLEUUID charUUID("065de41b-79fb-479d-b592-47caf39bfccb");

//Flags for BLE
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

uint8_t mode = 0; 
uint8_t prev_mode = mode; 

/* ----- INIT NeoPixel ----- */
  Adafruit_NeoPixel np_gauche(LED_COUNT, Pin_np_Gauche, NEO_GRB + NEO_KHZ800);
  Adafruit_NeoPixel np_droite(LED_COUNT, Pin_np_Droite, NEO_GRB + NEO_KHZ800);


/* ----- INIT Callbacks ----- */
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("New mode: ");
    Serial.println(pData[0]);
    prev_mode = mode;
    mode = pData[0];
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void setup() {
  init_leds();
  clean_leds();
  wait_connect();

  Serial.begin(9600);
  Serial.println("Starting application...");
  BLEDevice::init("");
  
  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(300, false);


} // End of setup.


// This is the Arduino main loop function.
void loop() {
    if (connected == false) {wait_connect();}

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
      doConnect = false;
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
      doConnect = true;
    }
    
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {

    switch (mode) {
      case 0:
        init_leds();
        break;

      case 1:
        if (prev_mode != mode){standby();}
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
    prev_mode = mode;
    /*
    String newValue = "Time since boot: " + String(millis()/1000);
    Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
    */

  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
    
  }
  
  delay(500); // Delay a second between loops.
} // End of loop
/* ------- FONCTIONS -------- */

void standby() {
  
  for (int i=0;i<LED_COUNT;i++)
  {
    np_droite.setPixelColor(i, sb_orange.r, sb_orange.g, sb_orange.b);
    np_gauche.setPixelColor(i, sb_orange.r, sb_orange.g, sb_orange.b);
    delay(10);
    np_droite.show();
    np_gauche.show();
    
  }
}

void droite(int time_del) {
  np_droite.setPixelColor(0, sb_orange.r, sb_orange.g, sb_orange.b);

  for (int i=1;i<LED_COUNT;i++)
  {
    np_droite.setPixelColor(i, orange.r, orange.g, orange.b);
    delay(time_del);
    np_droite.show();
  }
  delay(time_del);
  clean_leds();
}

void gauche(int time_del) {
  np_gauche.setPixelColor(0, sb_orange.r, sb_orange.g, sb_orange.b);

  for (int i=1;i<LED_COUNT;i++)
  {
    np_gauche.setPixelColor(i, orange.r, orange.g, orange.b);
    delay(time_del);
    np_gauche.show();
  }
  delay(time_del);
  clean_leds();
}

void clean_leds(){

  for (int i=0;i<=LED_COUNT;i++)
  {
    np_droite.setPixelColor(i, sb_orange.r, sb_orange.g, sb_orange.b);
    np_gauche.setPixelColor(i, sb_orange.r, sb_orange.g, sb_orange.b);
  }
  delay(10);
  np_droite.show();
  np_gauche.show();
}

void init_leds(){
  np_gauche.begin();
  np_droite.begin();
  mode = 1;
}

void wait_connect() {
  clean_leds();
    for (int i=0;i<=LED_COUNT;i++)
  {
    np_gauche.setPixelColor(i, rouge.r, rouge.g, rouge.b);
    np_droite.setPixelColor(i, rouge.r, rouge.g, rouge.b);
    delay(10);
    
    np_gauche.show();
    np_droite.show();
  }
}
