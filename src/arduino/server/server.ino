#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoSecrets.h" // save sensetive data in ArduinoSecrets.h file
#include "OwmApi.h"


char ssid[] = SECRET_SSID; //  your network SSID (name)
char password[] = SECRET_PASS;
float indoorTemperatureValue, indoorHumidityValue, indoorCo2Value, indoorPressureValue, indoorTvocValue, indoorPm25Value, indoorPm10Value;
String plotMetric = "statistic";
String city = "Langen";
String wJson, pJson;
unsigned long lastMillis;
AsyncWebServer server(80);
//EXTERN VARIABLES(from OwmApi.h)
float outdoorTemperatureValue, outdoorPressureValue, outdoorHumidityValue, outdoorPm25Value, outdoorPm10Value, latitude, longitude;

void setup()
{
  Serial.begin(115200);
  initWiFi();
  initWebserver();
  MDNS.begin("esp32");
  lastMillis = millis();
  wJson = getWeatherJson(city);
  deserialize(wJson, 'W');
  pJson = getPollutionJson(latitude, longitude);
  deserialize(pJson, 'P');
}
void loop()
{
  //every ten minutes
  if (millis() - lastMillis >= 10 * 60 * 1000UL)
  {
    lastMillis = millis();
    wJson = getWeatherJson(city);
    deserialize(wJson, 'W');
    pJson = getPollutionJson(latitude, longitude);
    deserialize(pJson, 'P');
  }
  plot();
  delay(7000);
}
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Serial.print ("Connecting to WiFi...");
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
    request->send(200, "text/plain", String(outdoorTemperatureValue));
  });
  server.on("/outdoorHumidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(outdoorHumidityValue));
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
    request->send(200, "text/plain", String(outdoorPressureValue));
  });
  server.on("/outdoorPm25", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(outdoorPm25Value));
  });
  server.on("/outdoorPm10", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(outdoorPm10Value));
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
    Serial.print("Indoor Temperature:");
    Serial.println(indoorTemperatureValue);
    Serial.print("Outdoor Temperature:");
    Serial.println(outdoorTemperatureValue);
  }
  else if (plotMetric == "humidity")
  {
    Serial.print("Indoor Humidity:");
    Serial.println(indoorHumidityValue);
    Serial.print("Outdoor Humidity:");
    Serial.println(outdoorHumidityValue);
  }
  else if (plotMetric == "co2")
  {
    Serial.print("Indoor CO2:");
    Serial.println(indoorCo2Value);
    Serial.print("Outdoor CO2:");
    Serial.println(400);
  }
  else if (plotMetric == "pressure")
  {
    Serial.print("Indoor Pressure:");
    Serial.println(indoorPressureValue);
    Serial.print("Outdoor Pressure:");
    Serial.println(outdoorPressureValue);
  }
  else if (plotMetric == "pm25")
  {
    Serial.print("Indoor PM2.5:");
    Serial.println(indoorPm25Value);
    Serial.print("Outdoor PM2.5:");
    Serial.println(outdoorPm25Value);
  }
  else if (plotMetric == "pm10")
  {
    Serial.print("Indoor PM10:");
    Serial.println(indoorPm10Value);
    Serial.print("Outdoor PM10:");
    Serial.println(outdoorPm10Value);
  }
  else if (plotMetric == "tvoc")
  {
    Serial.print("Indoor TVOC:");
    Serial.println(indoorTvocValue);
    Serial.print("Outdoor TVOC:");
    Serial.println(0);
  }
  else if (plotMetric == "statistic")
  {

    Serial.println("LOG  " + String(millis() * 60 * 1000UL));
    
    Serial.print("Indoor Temperature: ");
    Serial.println(indoorTemperatureValue);
    Serial.print("Outdoor Temperature: ");
    Serial.println(outdoorTemperatureValue);
    Serial.println();
    
    Serial.print("Indoor Humidity: ");
    Serial.println(indoorHumidityValue);
    Serial.print("Outdoor Humidity: ");
    Serial.println(outdoorHumidityValue);
    Serial.println(" ");
    
    Serial.print("Indoor CO2: ");
    Serial.println(indoorCo2Value);
    Serial.print("Outdoor CO2: ");
    Serial.println(400);
    Serial.println(" ");
    
    Serial.print("Indoor Pressure: ");
    Serial.println(indoorPressureValue);
    Serial.print("Outdoor Pressure:");
    Serial.println(outdoorPressureValue);
    Serial.println(" ");
    
    Serial.print("Indoor PM2.5: ");
    Serial.println(indoorPm25Value);
    Serial.print("Outdoor PM2.5: ");
    Serial.println(outdoorPm25Value);
    Serial.println(" ");
    
    Serial.print("Indoor PM10: ");
    Serial.println(indoorPm10Value);
    Serial.print("Outdoor PM10: ");
    Serial.println(outdoorPm10Value);
    Serial.println(" ");
    
    Serial.print("Indoor TVOC:");
    Serial.println(indoorTvocValue);
    Serial.print("Outdoor TVOC:");
    Serial.println("0");
    Serial.println(" ");
  }
  Serial.println(" ");
}
