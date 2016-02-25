#define NEO_PIN 2 // RedBear Blend & RFduino


// Import libraries (BLEPeripheral depends on SPI)
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <BLEPeripheral.h>




/*NEOPIXELS*/
Adafruit_NeoPixel strip = Adafruit_NeoPixel(169, NEO_PIN, NEO_GRB + NEO_KHZ800);



// define pin\\s (varies per shield/board)
// https://github.com/sandeepmistry/arduino-BLEPeripheral#pinouts
// Blend
#define BLE_REQ     -1
#define BLE_RDY     -1
#define BLE_RST     -1

// create peripheral instance, see pinouts above
BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);

// create service
BLEService neoService = BLEService("AB10");

// create switch characteristic
BLECharCharacteristic switchCharacteristic = BLECharCharacteristic("AB11", BLERead | BLEWrite);
BLEDescriptor switchDescriptor = BLEDescriptor("2901", "Switch");
BLEUnsignedCharCharacteristic redCharacteristic = BLEUnsignedCharCharacteristic("AB12", BLERead | BLEWrite);
BLEDescriptor redDescriptor = BLEDescriptor("2901", "Red");
BLEUnsignedCharCharacteristic greenCharacteristic = BLEUnsignedCharCharacteristic("AB13", BLERead | BLEWrite);
BLEDescriptor greenDescriptor = BLEDescriptor("2901", "Green");
BLEUnsignedCharCharacteristic blueCharacteristic = BLEUnsignedCharCharacteristic("AB14", BLERead | BLEWrite);
BLEDescriptor blueDescriptor = BLEDescriptor("2901", "Blue");

boolean painting = false;
byte red, green, blue;

void setup() {
  Serial.begin(9600);

  strip.begin();  
  strip.show();  


  // set LED pin to output mode
  pinMode(NEO_PIN, OUTPUT);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("NEO");
  blePeripheral.setDeviceName("NEO");
  blePeripheral.setAdvertisedServiceUuid(neoService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(neoService);
  blePeripheral.addAttribute(switchCharacteristic);
  blePeripheral.addAttribute(switchDescriptor);
  blePeripheral.addAttribute(redCharacteristic);
  blePeripheral.addAttribute(redDescriptor);
  blePeripheral.addAttribute(greenCharacteristic);
  blePeripheral.addAttribute(greenDescriptor);
  blePeripheral.addAttribute(blueCharacteristic);
  blePeripheral.addAttribute(blueDescriptor);



  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);
  redCharacteristic.setEventHandler(BLEWritten, redCharacteristicWritten);
  greenCharacteristic.setEventHandler(BLEWritten, greenCharacteristicWritten);
  blueCharacteristic.setEventHandler(BLEWritten, blueCharacteristicWritten);


  Serial.println("Passed event handler");


  // begin initialization
  blePeripheral.begin();

  Serial.println(F("Bluetooth LED"));
}




void loop() {
  // Tell the bluetooth radio to do whatever it should be working on
  blePeripheral.poll();
}




void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  
  // central wrote new value to characteristic, update LED
  Serial.print(F("Characteristic event, written: "));

  if (switchCharacteristic.value()) {
    Serial.println(F("NEO ON"));
    colorWipe(strip.Color(255, 255, 255), 100); // Red
  } else {
    Serial.println(F("NEO OFF"));
    colorWipe(strip.Color(0, 0, 0), 0); // Green
  }

  Serial.println(switchCharacteristic.value());
}




void redCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.print(F("Red set to: "));
  Serial.println(redCharacteristic.value());
  red = redCharacteristic.value();
  paint(10);
}


void greenCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.print(F("Green set to: "));
  Serial.println(greenCharacteristic.value());
  green = greenCharacteristic.value();
  paint(10);
}


void blueCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.print(F("Blue set to: "));
  Serial.println(blueCharacteristic.value());
  blue = blueCharacteristic.value();
  paint(10);
}




void paint(uint8_t wait) {

  if (painting) {
    return;
  }

  painting = true;
  uint32_t c = strip.Color(red, green, blue);
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  
  strip.show();
  painting = false;
      
}



void colorWipe(uint32_t c, uint8_t wait) {

  long previousMillis = 0;  // stores the last time the lights were updated

  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    while (millis() - previousMillis < wait) {
            // do nothing, this is like a delay but allows the Bluetooth Radio to work
   }
      previousMillis = millis();
      strip.setPixelColor(i, c);
      strip.show();
  }
      
}







/*
void paint2(uint8_t wait) {

  if (painting) {
    return;
  }

  painting = true;
  long previousMillis = 0;  // stores the last time the lights were updated
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    while (millis() - previousMillis < wait) {
            // do nothing, this is like a delay but allows the Bluetooth Radio to work
    }
      previousMillis = millis();
      //uint32_t c = strip.Color(redCharacteristic.value(), greenCharacteristic.value(), blueCharacteristic.value());
      uint32_t c = strip.Color(red, green, blue);

      strip.setPixelColor(i, c);
      strip.show();
  }
  painting = false;
      
}*/



