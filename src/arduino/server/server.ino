#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoSecrets.h" // save sensetive data in ArduinoSecrets.h file
#include "OwmApi.h"

char ssid[] = SECRET_SSID; //  your network SSID (name)
char password[] = SECRET_PASS;
AsyncWebServer server(80);

//EXTERN VARIABLES(from OwmApi.h)
float outdoorTemperature, outdoorPressure, outdoorHumidity, outdoorPm25, outdoorPm10, outdoorTvoc, latitude, longitude;
float outdoorCo2=415;
float indoorTemperature_1, indoorHumidity_1, indoorCo2_1, indoorPressure_1, indoorTvoc_1, indoorPm25_1, indoorPm10_1;
float indoorTemperature_2, indoorHumidity_2, indoorCo2_2, indoorPressure_2, indoorTvoc_2, indoorPm25_2, indoorPm10_2;
float indoorTemperatureFinal, indoorHumidityFinal, indoorCo2Final, indoorPressureFinal, indoorTvocFinal, indoorPm25Final, indoorPm10Final;
String plotMetric = "statistic";
String city = "Langen";
String wJson, pJson;
String json="empty";
unsigned long lastMillis;
int airQualityCategories[6];
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
  //Serial.println(getCo2());
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
  json = buildJson();
  if(decision()==1){
    Serial.println("Ventilation required");
  }
  else
  {
    Serial.println("Ventilation not required");
  }
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
  //CONFIGURATION
  server.on("/connect", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, " text / plain ", "connected"); });
  server.on("/plot", HTTP_GET, [](AsyncWebServerRequest *request) { //will be used to change the plot metric
    plotMetric = request->getParam("plotMetric")->value();
    request->send(200, "text/plain", plotMetric);
  });
  server.on("/city", HTTP_GET, [](AsyncWebServerRequest *request) { //will be used to change the city
    city = request->getParam("name")->value();
    request->send(200, "text/plain", "Received");
  });
  server.on("/json", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", json);});
  //READINGS FROM CLIENT 1
  server.on("/readValue", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              indoorTemperature_1 = request->getParam("temperature")->value().toFloat();
              indoorHumidity_1 = request->getParam("humidity")->value().toFloat();
              indoorCo2_1 = request->getParam("co2")->value().toFloat();
              indoorTvoc_1 = request->getParam("tvoc")->value().toFloat() * 0.00515;  // convertion from ppb to mg/m³
              indoorPressure_1 = request->getParam("pressure")->value().toFloat();
              indoorPm25_1 = request->getParam("pm25")->value().toFloat();
              indoorPm10_1 = request->getParam("pm10")->value().toFloat();
              request->send(200, "text/plain", "R");
            });
  //READINGS FROM CLIENT 2
  server.on("/readValue2", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              indoorTemperature_2 = request->getParam("temperature")->value().toFloat();
              indoorHumidity_2 = request->getParam("humidity")->value().toFloat();
              indoorCo2_2 = request->getParam("co2")->value().toFloat();
              indoorTvoc_2 = request->getParam("tvoc")->value().toFloat();
              indoorPressure_2 = request->getParam("pressure")->value().toFloat();
              indoorPm25_2 = request->getParam("pm25")->value().toFloat();
              indoorPm10_2 = request->getParam("pm10")->value().toFloat();
              request->send(200, "text/plain", "R");
            });
  server.begin();
}
void plot()
{
  if (plotMetric == "temperature")
  {
    Serial.print("Indoor Temperature:");
    Serial.println(indoorTemperatureFinal); Serial.print("Outdoor Temperature:");
    Serial.print("Outdoor Temperature:");
    Serial.println(outdoorTemperature);
  }
  else if (plotMetric == "humidity")
  {
    Serial.print("Indoor Humidity:");
    Serial.println(indoorHumidityFinal);
    Serial.print("Outdoor Humidity:");
    Serial.println(outdoorHumidity);
  }
  else if (plotMetric == "co2")
  {
    Serial.print("Indoor CO2:");
    Serial.println(indoorCo2Final);
    Serial.print("Outdoor CO2:");
    Serial.println(400);
  }
  else if (plotMetric == "pressure")
  {
    Serial.print("Indoor Pressure:");
    Serial.println(indoorPressureFinal);
    Serial.print("Outdoor Pressure:");
    Serial.println(outdoorPressure);
  }
  else if (plotMetric == "pm25")
  {
    Serial.print("Indoor PM2.5:");
    Serial.println(indoorPm25Final);
    Serial.print("Outdoor PM2.5:");
    Serial.println(outdoorPm25);
  }
  else if (plotMetric == "pm10")
  {
    Serial.print("Indoor PM10:");
    Serial.println(indoorPm10Final);
    Serial.print("Outdoor PM10:");
    Serial.println(outdoorPm10);
  }
  else if (plotMetric == "tvoc")
  {
    Serial.print("Indoor TVOC:");
    Serial.println(indoorTvocFinal);
    Serial.print("Outdoor TVOC:");
    Serial.println(0);
  }
  else if (plotMetric == "statistic")
  {

    Serial.println("LOG  " + String(millis() / (60 * 1000UL)));

    Serial.print("Indoor Temperature: ");
    Serial.print(indoorTemperature_1); Serial.println("°C");
    Serial.print("Outdoor Temperature: "); 
    Serial.print(outdoorTemperature); Serial.println("°C");
    Serial.println();

    Serial.print("Indoor Humidity: ");
    Serial.print(indoorHumidity_1); Serial.println("%");
    Serial.print("Outdoor Humidity: ");
    Serial.print(outdoorHumidity); Serial.println("%");
    Serial.println(" ");

    Serial.print("Indoor CO2: ");
    Serial.print(indoorCo2_1); Serial.println(" ppm");
    Serial.print("Outdoor CO2: ");
    Serial.print(outdoorCo2); Serial.println(" ppm");
    Serial.println(" ");

    Serial.print("Indoor Pressure: ");
    Serial.print(indoorPressure_1); Serial.println(" hPa");
    Serial.print("Outdoor Pressure:");
    Serial.print(outdoorPressure); Serial.println(" hPa");
    Serial.println(" ");

    Serial.print("Indoor PM2.5: ");
    Serial.print(indoorPm25_1);  Serial.println(" ppm");
    Serial.print("Outdoor PM2.5: ");
    Serial.print(outdoorPm25); Serial.println(" ppm");
    Serial.println(" ");

    Serial.print("Indoor PM10: ");
    Serial.print(indoorPm10_1); Serial.println(" ppm");
    Serial.print("Outdoor PM10: ");
    Serial.print(outdoorPm10); Serial.println(" ppm");
    Serial.println(" ");

    Serial.print("Indoor TVOC: ");
    Serial.print(indoorTvoc_1); Serial.println(" mg/m³");
    Serial.print("Outdoor TVOC:");
    Serial.print("<1"); Serial.println(" mg/m³");
    Serial.println(" ");
  }
  Serial.println(" ");
}
String buildJson(){
  String json="{";
  json += "\"indoorTemperature\":\"";
  json += String(indoorTemperature_1);
  json += "\",\"outdoorTemperature\":\"";
  json += String(outdoorTemperature);
  json += "\",\"indoorHumidity\":\"";
  json += String(indoorHumidity_1);
  json += "\",\"outdoorHumidity\":\"";
  json += String(outdoorHumidity);
  json += "\",\"indoorCo2\":\"";
  json += String(indoorCo2_1);
  json += "\",\"outdoorCo2\":\"";
  json += String(outdoorCo2);
  json += "\",\"indoorPressure\":\"";
  json += String(indoorPressure_1);
  json += "\",\"outdoorPressure\":\"";
  json += String(outdoorPressure);
  json += "\",\"indoorPm25\":\"";
  json += String(indoorPm25_1);
  json += "\",\"outdoorPm25\":\"";
  json += String(outdoorPm25);
  json += "\",\"indoorPm10\":\"";
  json += String(indoorPm10_1);
  json += "\",\"outdoorPm10\":\"";
  json += String(outdoorPm10);
  json += "\",\"indoorTvoc\":\"";
  json += String(indoorTvoc_1);
  json += "\",\"outdoorTvoc\":\"";
  json += String(0)+"\"";
  json += "}";
  return json;
}


float calculateIndoorHumidity(float outdoorTemperature, float outdoorHumidity, float indoorTemperature)
{
  //This function is to calculate the outdoor humidity if the outdoor air comes inside.
  float maxWaterVapor[71] = {0.9, 0.99, 1.08, 1.180, 1.29, 1.405, 1.53, 1.67, 1.82, 1.98, 2.15,
                             2.34, 2.55, 2.77, 3.005, 3.26, 3.53, 3.82, 4.14, 4.475, 4.84, 5.205, 5.590, 5.985, 6.395, 6.825,
                             7.28, 7.76, 8.27, 8.82, 9.4, 10, 10.65, 11.35, 12.1, 12.85, 13.65, 14.5, 15.4, 16.3, 17.3, 18.35,
                             19.4, 20.55, 21.8, 23.05, 24.35, 25.75, 27.2, 28.7, 30.35, 32.05, 33.85, 35.7, 37.65, 39.6, 41.7,
                             43.9, 46.2, 48.6, 51.15, 53.8, 56.7, 59.6, 62.5, 65.4, 68.5, 71.8, 75.3, 79, 83};
  float maxAbsHumidityOutdoor = maxWaterVapor[(int)(roundf(outdoorTemperature) + 20)];
  float absHumidityOutdoor = (outdoorHumidity / 100) * maxAbsHumidityOutdoor;
  float maxAbsHumidityIndoor = maxWaterVapor[(int)(roundf(indoorTemperature) + 20)];
  float absHumidityIndoor = (absHumidityOutdoor / maxAbsHumidityIndoor) * 100;
  return absHumidityIndoor;
}

int getHighestElement()
{
  int max = airQualityCategories[0];
  for (int i = 1; i <= 5; i++)
  {
    if (airQualityCategories[i] > max)
      max = airQualityCategories[i];
  }
  return max;
}

int getNumberOfHighestElement(int max)
{
  int numberOfMax = 0;
  for (int i = 0; i <= 5; i++)
  {
    if (airQualityCategories[i] == max)
      numberOfMax++;
  }
  return numberOfMax;
}

bool decision()
{

  bool ventilate = false;
  int maxValueAqIndoor = 1;
  int numberOfHighestElementAqIndoor = 0;
  int maxValueAqOutdoor = 1;
  int numberOfHighestElementAqOutdoor = 0;

  for (int i = 0; i <= 5; i++)
  {
    airQualityCategories[i] = 1;
  }
  categorizeAirQualityMetrics(indoorTemperature_1, indoorHumidity_1, indoorTvoc_1, indoorCo2_1, indoorPm25_1, indoorPm10_1);
  maxValueAqIndoor = getHighestElement();
  numberOfHighestElementAqIndoor = getNumberOfHighestElement(maxValueAqIndoor);

  float extrapolatedHumidityIndoor = calculateIndoorHumidity(outdoorTemperature, outdoorHumidity, indoorTemperature_1);
  for (int i = 0; i <= 5; i++)
  {
    airQualityCategories[i] = 1;
  }
  categorizeAirQualityMetrics(outdoorTemperature, extrapolatedHumidityIndoor, outdoorTvoc, outdoorCo2, outdoorPm25, outdoorPm10);
  maxValueAqOutdoor = getHighestElement();
  numberOfHighestElementAqOutdoor = getNumberOfHighestElement(maxValueAqOutdoor);

  if (maxValueAqIndoor > maxValueAqOutdoor)
  {
    ventilate = true;
  }
  else if (maxValueAqIndoor == maxValueAqOutdoor)
  {
    if (numberOfHighestElementAqIndoor > numberOfHighestElementAqOutdoor)
    {
      ventilate = true;
    }
  }
  return ventilate;
}

void categorizeAirQualityMetrics(float temperature, float humidity, float tvoc, float co2, float pm2_5, float pm10)
{
  if (temperature <= 26)
  {
    airQualityCategories[0] = 1;
  }
  else if (temperature <= 39)
  {
    airQualityCategories[0] = 2;
  }
  else if (temperature <= 46)
  {
    airQualityCategories[0] = 3;
  }
  else if (temperature > 46)
  {
    airQualityCategories[0] = 4;
  }

  if (humidity >= 30 || humidity <= 60)
  {
    airQualityCategories[1] = 1;
  }
  else if (humidity < 30 || humidity > 60)
  {
    airQualityCategories[1] = 2;
  }

  if (tvoc <= 1)
  {
    airQualityCategories[2] = 1;
  }
  else if (tvoc <= 10)
  {
    airQualityCategories[2] = 2;
  }
  else if (tvoc <= 25)
  {
    airQualityCategories[2] = 3;
  }
  else if (tvoc > 25)
  {
    airQualityCategories[2] = 4;
  }

  if (co2 <= 1000)
  {
    airQualityCategories[3] = 1;
  }
  else if (co2 <= 2000)
  {
    airQualityCategories[3] = 2;
  }
  else if (co2 <= 5000)
  {
    airQualityCategories[3] = 3;
  }
  else if (co2 > 5000)
  {
    airQualityCategories[3] = 4;
  }

  if (pm2_5 <= 10)
  {
    airQualityCategories[4] = 1;
  }
  else if (pm2_5 <= 25)
  {
    airQualityCategories[4] = 2;
  }
  else if (pm2_5 <= 50)
  {
    airQualityCategories[4] = 3;
  }
  else if (pm2_5 > 50)
  {
    airQualityCategories[4] = 4;
  }

  if (pm10 <= 20)
  {
    airQualityCategories[5] = 1;
  }
  else if (pm10 <= 50)
  {
    airQualityCategories[5] = 2;
  }
  else if (pm10 <= 100)
  {
    airQualityCategories[5] = 3;
  }
  else if (pm10 > 100)
  {
    airQualityCategories[5] = 4;
  }
}
