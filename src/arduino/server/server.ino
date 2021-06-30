#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoSecrets.h" // save sensetive data in ArduinoSecrets.h file
#include "OwmApi.h"


char ssid[] = SECRET_SSID; //  your network SSID (name)
char password[] = SECRET_PASS;
AsyncWebServer server(80);

//EXTERN VARIABLES(from OwmApi.h)
float outdoorTemperature, outdoorPressure, outdoorHumidity, outdoorPm25, outdoorPm10, outdoorCo2, latitude, longitude;
float indoorTemperature, indoorHumidity, indoorCo2, indoorPressure, indoorTvoc, indoorPm25, indoorPm10;
String plotMetric = "statistic";
String city = "Langen";
String wJson, pJson;
unsigned long lastMillis;

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
    request->send(200, "text/plain", String(indoorTemperature));
  });
  server.on("/indoorHumidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorHumidity));
  });
  server.on("/indoorCo2", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorCo2));
  });
  server.on("/indoorTvoc", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorTvoc));
  });
  server.on("/indoorPressure", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPressure));
  });
  server.on("/indoorPm25", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPm25));
  });
  server.on("/indoorPm10", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(indoorPm10));
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
    request->send(200, "text/plain", String(outdoorCo2));
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
    indoorTemperature = request->getParam("temperature")->value().toFloat();
    indoorHumidity = request->getParam("humidity")->value().toFloat();
    indoorCo2 = request->getParam("co2")->value().toFloat();
    indoorTvoc = request->getParam("tvoc")->value().toFloat();
    indoorPressure = request->getParam("pressure")->value().toFloat();
    indoorPm25 = request->getParam("pm25")->value().toFloat();
    indoorPm10 = request->getParam("pm10")->value().toFloat();
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
    Serial.println(indoorTemperature);
    Serial.print("Outdoor Temperature:");
    Serial.println(outdoorTemperature);
  }
  else if (plotMetric == "humidity")
  {
    Serial.print("Indoor Humidity:");
    Serial.println(indoorHumidity);
    Serial.print("Outdoor Humidity:");
    Serial.println(outdoorHumidity);
  }
  else if (plotMetric == "co2")
  {
    Serial.print("Indoor CO2:");
    Serial.println(indoorCo2);
    Serial.print("Outdoor CO2:");
    Serial.println(400);
  }
  else if (plotMetric == "pressure")
  {
    Serial.print("Indoor Pressure:");
    Serial.println(indoorPressure);
    Serial.print("Outdoor Pressure:");
    Serial.println(outdoorPressure);
  }
  else if (plotMetric == "pm25")
  {
    Serial.print("Indoor PM2.5:");
    Serial.println(indoorPm25);
    Serial.print("Outdoor PM2.5:");
    Serial.println(outdoorPm25);
  }
  else if (plotMetric == "pm10")
  {
    Serial.print("Indoor PM10:");
    Serial.println(indoorPm10);
    Serial.print("Outdoor PM10:");
    Serial.println(outdoorPm10);
  }
  else if (plotMetric == "tvoc")
  {
    Serial.print("Indoor TVOC:");
    Serial.println(indoorTvoc);
    Serial.print("Outdoor TVOC:");
    Serial.println(0);
  }
  else if (plotMetric == "statistic")
  {

    Serial.println("LOG  " + String(millis() * 60 * 1000UL));
    
    Serial.print("Indoor Temperature: ");
    Serial.println(indoorTemperature);
    Serial.print("Outdoor Temperature: ");
    Serial.println(outdoorTemperature);
    Serial.println();
    
    Serial.print("Indoor Humidity: ");
    Serial.println(indoorHumidity);
    Serial.print("Outdoor Humidity: ");
    Serial.println(outdoorHumidity);
    Serial.println(" ");
    
    Serial.print("Indoor CO2: ");
    Serial.println(indoorCo2);
    Serial.print("Outdoor CO2: ");
    Serial.println(400);
    Serial.println(" ");
    
    Serial.print("Indoor Pressure: ");
    Serial.println(indoorPressure);
    Serial.print("Outdoor Pressure:");
    Serial.println(outdoorPressure);
    Serial.println(" ");
    
    Serial.print("Indoor PM2.5: ");
    Serial.println(indoorPm25);
    Serial.print("Outdoor PM2.5: ");
    Serial.println(outdoorPm25);
    Serial.println(" ");
    
    Serial.print("Indoor PM10: ");
    Serial.println(indoorPm10);
    Serial.print("Outdoor PM10: ");
    Serial.println(outdoorPm10);
    Serial.println(" ");
    
    Serial.print("Indoor TVOC:");
    Serial.println(indoorTvoc);
    Serial.print("Outdoor TVOC:");
    Serial.println("0");
    Serial.println(" ");
  }
  Serial.println(" ");
}
