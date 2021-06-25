/***************************************************
  Notes about hardware

  SHT
  the default I2C address is 0x44 and you can also select address 0x45   by connecting the ADDR pin to a high voltage signal.

  BME
  - SDI -> SDA / SDO -> GND for address 0x76
 ****************************************************/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Arduino.h>
#include "Adafruit_SHT31.h"
#include <Adafruit_BMP085.h>
#include "Adafruit_CCS811.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <SensirionI2CScd4x.h>
#include <sps30.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_BMP085 bmp;
Adafruit_CCS811 ccs;
Adafruit_BME680 bme;
SensirionI2CScd4x scd4x;

char ssid[] = "Vodafone-262F_T"; //  your network SSID (name)
char password[] = "Tarti.38";
float temperature, humidity, co2, airQuality, pressure, pm25, pm10, tvoc;
int httpCode = 0;
typedef enum
{
  READ_SHT31,
  READ_BMP180,
  READ_CCS811,
  READ_BME680,
  READ_SPS30,
  READ_SCD41,
  CALCULATE_TEMPERATURE,
  CALCULATE_HUMIDITY,
  CALCULATE_CO2,
  CALCULATE_AIR_QUALITY,
  CALCULATE_PRESSURE,
  CALCULATE_PM25,
  CALCULATE_PM10,
  CALCULATE_TVOC,
  TRANSMIT_VALUES,
  WAIT
} AppState_t;
AppState_t state = READ_SHT31;
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  initWiFi();
  setupSHT31();
  setupBMP180();
  setupCCS811();
  setupBME680();
  setupSPS30();
  setupSCD41();
}
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print(" Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
}
void loop()
{
  switch (state)
  {
  case READ_SHT31:
    readSHT31();
    state = READ_BMP180;
    break;
  case READ_BMP180:
    readBMP180();
    state = READ_CCS811;
    break;
  case READ_CCS811:
    readCCS811();
    state = READ_BME680;
    break;
  case READ_BME680:
    readBME680();
    state = READ_SPS30;
    break;
  case READ_SPS30:
    readSPS30();
    state = READ_SCD41;
    break;
  case READ_SCD41:
    readSCD41();
    state = CALCULATE_TEMPERATURE;
    break;
  case CALCULATE_TEMPERATURE:
    //find a average
    temperature = temperature / 4;
    state = CALCULATE_HUMIDITY;
    break;
  case CALCULATE_HUMIDITY:
    //find a average
    humidity = humidity / 3;
    state = CALCULATE_CO2;
    break;
  case CALCULATE_CO2:
    state = CALCULATE_AIR_QUALITY;
    break;
  case CALCULATE_AIR_QUALITY:
    //TODO: decide a calculation way for air quality
    state = CALCULATE_PRESSURE;
    break;
  case CALCULATE_PRESSURE:
    //find a average
    pressure = pressure / 2;
    state = CALCULATE_PM25;
    break;
  case CALCULATE_PM25:
    state = CALCULATE_PM10;
    break;
  case CALCULATE_PM10:
    state = TRANSMIT_VALUES;
    break;  
  case TRANSMIT_VALUES:
    transmitValues();
    state = WAIT;
    break;
  case WAIT:
    temperature = 0;
    humidity = 0;
    co2 = 0;
    airQuality = 0;
    pressure = 0;
    pm10 = 0;
    pm25 = 0;
    tvoc = 0;
    delay(5000);
    state = READ_SHT31;
  }
}
void setupSHT31()
{
  Serial.println("SHT31");
  if (!sht31.begin(0x44))
  { // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1)
      delay(1);
  }
}
void readSHT31()
{
  float t = sht31.readTemperature();

  float h = sht31.readHumidity();
  Serial.println("SHT31 Readings");
  if (!isnan(t))
  { // check if 'is not a number'
    Serial.print("Temp *C = ");
    Serial.print(t);
    Serial.print("\t\t");
  }
  else
  {
    Serial.println("Failed to read temperature");
  }
  if (!isnan(h))
  { // check if 'is not a number'
    Serial.print("Hum. % = ");
    Serial.println(h);
  }
  else
  {
    Serial.println("Failed to read humidity");
  }
  temperature += t;
  humidity += h;
  Serial.println();
}
void setupBMP180()
{
  if (!bmp.begin())
  {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1)
    {
    }
  }
}
void readBMP180()
{
  // 1pa/100=1hpa
  float bmpPressure = bmp.readPressure() / 100;
  float bmpTemperature = bmp.readTemperature();
  Serial.println("BMP180");
  Serial.print("Temperature = ");
  Serial.print(bmpTemperature);
  Serial.println(" *C");
  Serial.print("Pressure = ");
  Serial.print(bmpPressure);
  Serial.println(" hPa");
  temperature += bmpTemperature;
  pressure += bmpPressure;
}
void setupCCS811()
{
  Serial.println("CCS811 test");
  if (!ccs.begin())
  {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (1)
      ;
    // Wait for the sensor to be ready
    while (!ccs.available())
      ;
  }
}
void setupBME680()
{
  Serial.println(F("BME680 test"));
  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }
  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  Serial.println();
}
void readCCS811()
{
  if (ccs.available())
  {
    tvoc = ccs.getTVOC();
    Serial.println("CCS811");
    if (!ccs.readData())
    {
      Serial.print("CO2: ");
      //Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(tvoc);
    }
    else
    {
      Serial.println("ERROR!");
      while (1)
        ;
    }
  }
}
void readBME680()
{
  if (!bme.performReading())
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.println("BME680");
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  // Serial.print("Gas = ");
  // Serial.print(bme.gas_resistance / 1000.0);
  // Serial.println(" KOhms");

  Serial.println();
  temperature += bme.temperature;
  humidity += bme.humidity;
  pressure += bme.pressure / 100;
}
void setupSPS30()
{
  int16_t ret;
  uint8_t auto_clean_days = 4;
  uint32_t auto_clean;
  delay(2000);

  sensirion_i2c_init();

  while (sps30_probe() != 0)
  {
    Serial.print("SPS sensor probing failed\n");
    delay(500);
  }
#ifndef PLOTTER_FORMAT
  Serial.print("SPS sensor probing successful\n");
#endif /* PLOTTER_FORMAT */

  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret)
  {
    Serial.print("error setting the auto-clean interval: ");
    Serial.println(ret);
  }

  ret = sps30_start_measurement();
  if (ret < 0)
  {
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
void readSPS30()
{
  struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;

  do
  {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0)
    {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    }
    else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0)
  {
    Serial.print("error reading measurement\n");
  }
  else
  {

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
    Serial.print(m.nc_1p0 - m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_2p5 - m.nc_1p0);
    Serial.print(" ");
    Serial.print(m.nc_4p0 - m.nc_2p5);
    Serial.print(" ");
    Serial.print(m.nc_10p0 - m.nc_4p0);
    Serial.println();

#endif /* PLOTTER_FORMAT */
  }
  pm25=m.mc_2p5;
  pm10=m.mc_10p0;
  //delay(1000);
}
void setupSCD41()
{

  uint16_t error;
  char errorMessage[256];

  scd4x.begin(Wire);

  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }

  uint16_t serial0;
  uint16_t serial1;
  uint16_t serial2;
  error = scd4x.getSerialNumber(serial0, serial1, serial2);
  if (error)
  {
    Serial.print("Error trying to execute getSerialNumber(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  else
  {
    printSerialNumber(serial0, serial1, serial2);
  }

  // Start Measurement
  error = scd4x.startPeriodicMeasurement();
  if (error)
  {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  delay(5000);
}
void readSCD41()
{
  Serial.println("SCD41");
  uint16_t error;
  char errorMessage[256];
  // Read Measurement
  uint16_t scdCo2;
  uint16_t scdTemperature;
  uint16_t scdHumidity;
  error = scd4x.readMeasurement(scdCo2, scdTemperature, scdHumidity);
  if (error)
  {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }
  else if (scdCo2 == 0)
  {
    Serial.println("Invalid sample detected, skipping.");
  }
  else
  {
    Serial.print("Co2:");
    Serial.print(scdCo2);
    Serial.print("\t");
    Serial.print("Temperature:");
    Serial.print(scdTemperature * 175.0 / 65536.0 - 45.0);
    Serial.print("\t");
    Serial.print("Humidity:");
    Serial.println(scdHumidity * 100.0 / 65536.0);
  }
  Serial.println();
  temperature += (scdTemperature * 175.0 / 65536.0 - 45.0);
  humidity += (scdHumidity * 100.0 / 65536.0);
  co2 = scdCo2;
}
void printUint16Hex(uint16_t value)
{
  Serial.print(value < 4096 ? "0" : "");
  Serial.print(value < 256 ? "0" : "");
  Serial.print(value < 16 ? "0" : "");
  Serial.print(value, HEX);
}
void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2)
{
  Serial.print("Serial: 0x");
  printUint16Hex(serial0);
  printUint16Hex(serial1);
  printUint16Hex(serial2);
  Serial.println();
}
void transmitValues()
{
  if ((WiFi.status() == WL_CONNECTED))
  { //Check the current connection status
    HTTPClient http;
    http.begin("http://esp32.local/readValue/?temperature=" + String(temperature) + "&humidity=" + String(humidity) + "&co2=" + String(co2) + "&airQuality=" + String(tvoc) + "&pressure=" + String(pressure) + "&pm25=" + String(pm25) + "&pm10=" + String(pm10)); //Specify the URL
    httpCode = http.GET();                                                                                                                                                                                                                              //Make the request
    if (httpCode > 0)
    { //Check for the returning code
      String payload = http.getString();
    }
    else
    {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
}