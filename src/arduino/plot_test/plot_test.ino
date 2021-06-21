/*************************************************** 
  This is an example for the SHT31-D Humidity & Temp Sensor

  Designed specifically to work with the SHT31-D sensor from Adafruit
  ----> https://www.adafruit.com/products/2857

  These sensors use I2C to communicate, 2 pins are required to  
  interface
 ****************************************************/
 
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

bool enableHeater = false;
uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(9600);

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  sht31.begin(0x44);
  


}


void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  Serial.print(t);
  Serial.print(" ");
  Serial.println(h);
  delay(1000);

}
