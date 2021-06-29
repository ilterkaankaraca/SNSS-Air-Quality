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
//EXTERN VARIABLES
float outdoorTemperature, outdoorPressure, outdoorHumidity, outdoorPm25, outdoorPm10, latitude, longitude;

void setup()
{
  Serial.begin(115200);
  initWiFi();
  initWebserver();
  MDNS.begin("esp32");
  Serial.println(getWeatherJson("langen"));
  Serial.println(getPollutionJson(latitude, longitude));
  deserialize(getWeatherJson("langen"),getPollutionJson(latitude, longitude));

}
void loop()
{
  plot();
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
  //INDOOR
  server.on("/indoorTemperature", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorTemperatureValue));
  });
  server.on("/indoorHumidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorHumidityValue));
  });
  server.on("/indoorCo2", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorCo2Value));
  });
  server.on("/indoorTvoc", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorTvocValue));
  });
  server.on("/indoorPressure", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPressureValue));
  });
  server.on("/indoorPm25", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPm25Value));
  });
  server.on("/indoorPm10", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPm10Value));
  });
  //OUTDOOR
  server.on("/outdoorTemperature", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(outdoorTemperature));
  });
  server.on("/outdoorHumidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(outdoorHumidity));
  });
  server.on("/outdoorCo2", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", "400");
  });
  server.on("/outdoorTvoc", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", "0");
  });
  server.on("/outdoorPressure", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(outdoorPressure));
  });
  server.on("/outdoorPm25", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(outdoorPm25));
  });
  server.on("/outdoorPm10", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(outdoorPm10));
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
