#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
char ssid[] = "Vodafone-262F_T"; //  your network SSID (name)
char password[] = "Tarti.38";

AsyncWebServer server(80);
float temperatureValue, humidityValue, co2Value, airQualityValue, pressureValue, particleValue;
String plotMetric = "temperature";
void setup()
{
  Serial.begin(115200);
  initWiFi();
  initWebserver();
  MDNS.begin("esp32")
}
void loop()
{
  if (plotMetric == "temperature")
  {
    Serial.print("Temperature:");
    Serial.print(temperatureValue);
  }
  else if (plotMetric == "humidity")
  {
    // Serial.print(", ");
    Serial.print("Humidity:");
    Serial.print(humidityValue);
  }
  // Serial.print(", ");
  else if (plotMetric == "co2")
  {
    Serial.print("CO2:");
    Serial.print(co2Value);
    // Serial.print(", ");
  }
  else if (plotMetric == "airquality")
  {
    Serial.print("AirQuality:");
    Serial.print(airQualityValue);
  }
  else if (plotMetric == "pressure")
  {
    // Serial.print(", ");
    Serial.print("Pressure:");
    Serial.print(pressureValue);
  }
  else if (plotMetric == "particle")
  {
    // Serial.print(", ");
    Serial.print("Particle:");
    Serial.print(particleValue);
  }
  Serial.println();
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
  server.on("/connect", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, " text / plain ", "connected"); });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(temperatureValue)); });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(humidityValue)); });
  server.on("/co2", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(co2Value)); });
  server.on("/airQuality", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(airQualityValue)); });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(pressureValue)); });
  server.on("/particle", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(particleValue)); });
  server.on("/readValue", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              temperatureValue = request->getParam("temperature")->value().toFloat();
              humidityValue = request->getParam("humidity")->value().toFloat();
              co2Value = request->getParam("co2")->value().toFloat();
              airQualityValue = request->getParam("airQuality")->value().toFloat();
              pressureValue = request->getParam("pressure")->value().toFloat();
              pm25Value = request->getParam("pm25")->value().toFloat();
              pm10Value = request->getParam("pm10")->value().toFloat();
              tvocValue = request->getParam("tvoc")->value().toFloat();
              request->send(200, "text/plain", "R");
            });
  server.on("/plot", HTTP_GET, [](AsyncWebServerRequest *request) { //will be used to change the plot metric
    plotMetric = request->getParam("plotMetric")->value();
    request->send(200, "text/plain", plotMetric);
  });
  server.begin();
}
