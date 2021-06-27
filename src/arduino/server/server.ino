#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoSecrets.h" // save sensetive data in ArduinoSecrets.h file
#include "OwmApi.h"


char ssid[] = SECRET_SSID; //  your network SSID (name)
char password[] = SECRET_PASS;
float indoorTemperatureValue, indoorHumidityValue, indoorCo2Value, indoorAirQualityValue, indoorPressureValue, indoorTvocValue, indoorPm25Value, indoorPm10Value;
String plotMetric = "temperature";
String city = "Langen";
AsyncWebServer server(80);
float temperature, pressure, humidity;
// COORD
float latitude, longitude;

void setup()
{
  Serial.begin(115200);
  initWiFi();
  initWebserver();
  MDNS.begin("esp32");
  deserialize(getWeatherJson("langen"));

}
void loop()
{
  Serial.println(temperature);
  Serial.println(humidity);
  Serial.println(pressure);
  Serial.println(longitude);
  Serial.println(latitude);
 // Serial.println(country);
  //plot();
  delay(7000);
}
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Serial.print (" Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    //Serial.print (".");
    delay(1000);
  }
  // Serial.println ( WiFi.localIP ());
  // Serial.println ( WiFi.macAddress ());
}
void initWebserver(void)
{
  server.on("/connect", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, " text / plain ", "connected");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorTemperatureValue));
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorHumidityValue));
  });
  server.on("/co2", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorCo2Value));
  });
  server.on("/tvoc", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorTvocValue));
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPressureValue));
  });
  server.on("/pm25", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPm25Value));
  });
  server.on("/pm10", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPm10Value));
  });
  server.on("/readValue", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    indoorTemperatureValue = request->getParam("temperature")->value().toFloat();
    indoorHumidityValue = request->getParam("humidity")->value().toFloat();
    indoorCo2Value = request->getParam("co2")->value().toFloat();
    indoorTvocValue = request->getParam("tvoc")->value().toFloat();
    indoorPressureValue = request->getParam("pressure")->value().toFloat();
    indoorPm25Value = request->getParam("pm25")->value().toFloat();
    indoorPm10Value = request->getParam("pm10")->value().toFloat();
    request->send(200, "text/plain", "R");
  });
  server.on("/plot", HTTP_GET, [](AsyncWebServerRequest * request) { //will be used to change the plot metric
    plotMetric = request->getParam("plotMetric")->value();
    request->send(200, "text/plain", plotMetric);
  });
  server.on("/city", HTTP_GET, [](AsyncWebServerRequest * request) { //will be used to change the plot metric
    plotMetric = request->getParam("plotMetric")->value();
    request->send(200, "text/plain", plotMetric);
  });
  server.begin();
}
void plot() {
  if (plotMetric == "temperature")
  {
    Serial.print("Temperature:");
    Serial.print(indoorTemperatureValue);
  }
  else if (plotMetric == "humidity")
  {
    Serial.print("Humidity:");
    Serial.print(indoorHumidityValue);
  }
  else if (plotMetric == "co2")
  {
    Serial.print("CO2:");
    Serial.print(indoorCo2Value);
  }
  else if (plotMetric == "airquality")
  {
    Serial.print("AirQuality:");
    Serial.print(indoorAirQualityValue);
  }
  else if (plotMetric == "pressure")
  {
    Serial.print("Pressure:");
    Serial.print(indoorPressureValue);
  }
  else if (plotMetric == "pm25")
  {
    Serial.print("PM2.5:");
    Serial.print(indoorPm25Value);
  }
  else if (plotMetric == "pm10")
  {
    Serial.print("PM10:");
    Serial.print(indoorPm10Value);
  }
  else if (plotMetric == "tvoc")
  {
    Serial.print("TVOC:");
    Serial.print(indoorTvocValue);
  }
  Serial.println();
}
