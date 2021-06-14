/***************************************************
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 *This is a library for the CCS811 air

  This sketch reads the sensor

  Designed specifically to work with the Adafruit CCS811 breakout
  ----> http://www.adafruit.com/products/3566

  These sensors use I2C to communicate. The device's I2C address is 0x5A

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  This is a library for the BME680 gas, humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME680 Breakout
  ----> http://www.adafruit.com/products/3660

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These pressure and temperature sensors use I2C to communicate, 2 pins
  are required to interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
  SHT
  ----
  the default I2C address is 0x44 and you can also select address 0x45
  by connecting the ADDR pin to a high voltage signal.
  BME - SDI -> SDA / SDO -> GND for address 0x76
  
 ****************************************************/


#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <Adafruit_BMP085.h>
#include "Adafruit_CCS811.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <SensirionI2CScd4x.h>
#include <sps30.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define I2C_SDA 33
#define I2C_SCL 32

#define SEALEVELPRESSURE_HPA (1013.25)


uint8_t loopCnt = 0;

Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_BMP085 bmp;
Adafruit_CCS811 ccs;
Adafruit_BME680 bme; // I2C
SensirionI2CScd4x scd4x;
void setup() {
   
  Serial.begin(9600);
  Wire.begin();
  setupSHT31();
  setupBMP180();
  setupCCS811();
  setupBME680();
  setupSPS30();
  setupSCD41();
  Serial.println();
}

void loop() {
  readSHT31();
  readBMP180();
  readCCS811();
  readBME680();
  readSPS30();
  readSCD41();
  delay(5000);
}

void setupSHT31() {
  Serial.println("SHT31");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}
void readSHT31() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  Serial.println("SHT31");
  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.print(t); Serial.print("\t\t");
  } else {
    Serial.println("Failed to read temperature");
  }
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
  } else {
    Serial.println("Failed to read humidity");
  }
  Serial.println();
}
void setupBMP180() {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }
}
void readBMP180() {
  Serial.println("BMP180"); 
  Serial.print("Temperature = " + String(bmp.readTemperature()) + " *C"); Serial.print("\t\t");
  Serial.print("Pressure = "+String(bmp.readPressure())+" Pa");

  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  // Serial.print("Altitude = ");
  // Serial.print(bmp.readAltitude());
  // Serial.println(" meters");
  // Serial.print("Pressure at sealevel (calculated) = ");
  // Serial.print(bmp.readSealevelPressure());
  // Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  // Serial.print("Real altitude = ");
  // Serial.print(bmp.readAltitude(101500));
  // Serial.println(" meters");
  // Serial.println();
  Serial.println("\n");
}
void setupCCS811() {
  Serial.println("CCS811 test");
  if (!ccs.begin()) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (1);
    // Wait for the sensor to be ready
    while (!ccs.available());
  }
}
void setupBME680() {
 Serial.println(F("BME680 test"));

  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }
  
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  Serial.println();
}
void readCCS811() {
  if (ccs.available()) {
    Serial.println("CCS811");
    if (!ccs.readData()) {
      Serial.print("eCO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(ccs.getTVOC());
    }
    else {
      Serial.println("ERROR!");
      while (1);
    }
  }
Serial.println();
}
void readBME680() {
 if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.println("BME680");
  Serial.print("Temperature = " + String(bme.temperature) +  " *C");

  Serial.print("  Pressure = "+ String(bme.pressure / 100.0)+ " hPa");

  Serial.print("  Humidity = "+String(bme.humidity)+" %");

  Serial.print("  Gas = "+String(bme.gas_resistance / 1000.0)+" KOhms");

  // Serial.print("Approx. Altitude = ");
  // Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  // Serial.println(" m");

  Serial.println("\n");
}
void setupSPS30() {
  int16_t ret;
  uint8_t auto_clean_days = 4;
  uint32_t auto_clean;
  delay(2000);
  sensirion_i2c_init();

  while (sps30_probe() != 0) {
    Serial.print("SPS sensor probing failed\n");
    delay(500);
  }
#ifndef PLOTTER_FORMAT
  Serial.print("SPS sensor probing successful\n");
#endif /* PLOTTER_FORMAT */

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    Serial.print("error setting the auto-clean interval: ");
    Serial.println(ret);
  }

  ret = sps30_start_measurement();
  if (ret < 0) {
    Serial.print("error starting measurement\n");
  }

#ifndef PLOTTER_FORMAT
  Serial.print("measurements started\n");
#endif /* PLOTTER_FORMAT */

#ifdef SPS30_LIMITED_I2C_BUFFER_SIZE
  Serial.print("Your Arduino hardware has a limitation that only\n");
  Serial.print("  allows reading the mass concentrations. For more\n");
  Serial.print("  information, please check\n");
  Serial.print("  https://github.com/Sensirion/arduino-sps#esp8266-partial-legacy-support\n");
  Serial.print("\n");
  delay(2000);
#endif

  delay(1000);
  
}


void readSPS30(){
  struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;
  
  Serial.println("SPS30");
  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    } else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("error reading measurement\n");
  } else {

#ifndef PLOTTER_FORMAT
    Serial.print("PM  1.0: ");
    Serial.println(m.mc_1p0);
    Serial.print("PM  2.5: ");
    Serial.println(m.mc_2p5);
    Serial.print("PM  4.0: ");
    Serial.println(m.mc_4p0);
    Serial.print("PM 10.0: ");
    Serial.println(m.mc_10p0);

#ifndef SPS30_LIMITED_I2C_BUFFER_SIZE
    Serial.print("NC  0.5: ");
    Serial.println(m.nc_0p5);
    Serial.print("NC  1.0: ");
    Serial.println(m.nc_1p0);
    Serial.print("NC  2.5: ");
    Serial.println(m.nc_2p5);
    Serial.print("NC  4.0: ");
    Serial.println(m.nc_4p0);
    Serial.print("NC 10.0: ");
    Serial.println(m.nc_10p0);

    Serial.print("Typical partical size: ");
    Serial.println(m.typical_particle_size);
#endif

    Serial.println();

#else
    // since all values include particles smaller than X, if we want to create buckets we 
    // need to subtract the smaller particle count. 
    // This will create buckets (all values in micro meters):
    // - particles        <= 0,5
    // - particles > 0.5, <= 1
    // - particles > 1,   <= 2.5
    // - particles > 2.5, <= 4
    // - particles > 4,   <= 10

    Serial.print(m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_1p0  - m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_2p5  - m.nc_1p0);
    Serial.print(" ");
    Serial.print(m.nc_4p0  - m.nc_2p5);
    Serial.print(" ");
    Serial.print(m.nc_10p0 - m.nc_4p0);
    Serial.println();


#endif /* PLOTTER_FORMAT */

  }

  delay(1000);
}
void setupSCD41(){
  

    uint16_t error;
    char errorMessage[256];

    scd4x.begin(Wire);

    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    error = scd4x.getSerialNumber(serial0, serial1, serial2);
    if (error) {
        Serial.print("Error trying to execute getSerialNumber(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        printSerialNumber(serial0, serial1, serial2);
    }

    // Start Measurement
    error = scd4x.startPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
}
void readSCD41(){
 Serial.println("SCD41");
  uint16_t error;
    char errorMessage[256];

    delay(5000);

    // Read Measurement
    uint16_t co2;
    uint16_t temperature;
    uint16_t humidity;
    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        Serial.println("Invalid sample detected, skipping.");
    } else {
        Serial.print("Co2:");
        Serial.print(co2);
        Serial.print("ppm \t");
        Serial.print("Temperature:");
        Serial.print(temperature * 175.0 / 65536.0 - 45.0);
        Serial.print("°C \t");
        Serial.print("Hum. % :");
        Serial.println((humidity * 100.0 / 65536.0));
        
    }
     Serial.println();
}
void printUint16Hex(uint16_t value) {
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}

void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
    Serial.print("Serial: 0x");
    printUint16Hex(serial0);
    printUint16Hex(serial1);
    printUint16Hex(serial2);
    Serial.println();
} 
