#include <WiFi.h>
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include "ArduinoSecrets.h" // save sensetive data in ArduinoSecrets.h file
#include "OwmApi.h"
#define CLIENT1 0
#define CLIENT2 1
#define FINAL 2
#define TEMPERATURE 0
#define HUMIDITY 1
#define TVOC 2
#define CO2 3
#define PM25 4
#define PM10 5

char ssid[] = SECRET_SSID; //  your network SSID (name)
char password[] = SECRET_PASS;
AsyncWebServer server(80);

//EXTERN VARIABLES(from OwmApi.h)
float outdoorTemperature, outdoorPressure, outdoorHumidity, outdoorPm25, outdoorPm10, outdoorTvoc, latitude, longitude;
float outdoorCo2 = 415;
float indoorTemperature[3], indoorHumidity[3], indoorCo2[3], indoorPressure[3], indoorTvoc[3], indoorPm25[3], indoorPm10[3];
String plotMetric = "statistic";
String city = "Langen";
String newCity = "Langen";
String weatherJson, pollutionJson;
String airInformationJson = "empty";
bool ventilationDecision;
//last millisecond variable for 10 Minutes
unsigned long lastMillis10Min;
//last msecond variable for 7 seconds
unsigned long lastMillis7Sec;
int airQualityCategories[6];

void setup()
{
  Serial.begin(115200);
  initWiFi();
  initWebserver();
  MDNS.begin("esp32");
  lastMillis10Min = millis();
  lastMillis7Sec = millis();
  weatherJson = getWeatherJson(city);
  if (weatherJson != "-1")
  {
    deserialize(weatherJson, 'W');
  }
  pollutionJson = getPollutionJson(latitude, longitude);
  if (pollutionJson != "-1")
  {
    deserialize(pollutionJson, 'P');
  }
}
void loop()
{
  if(city != newCity)
  {
    city = newCity;
    weatherJson = getWeatherJson(city);
    deserialize(weatherJson, 'W');
    pollutionJson = getPollutionJson(latitude, longitude);
    deserialize(pollutionJson, 'P');
  }
  ventilationDecision = decideVentilation();
  airInformationJson = buildAirInformationJson();
  indoorCo2[FINAL] = (indoorCo2[CLIENT1] + indoorCo2[CLIENT2] + 0.01) / 2;
  indoorHumidity[FINAL] = (indoorHumidity[CLIENT1] + indoorHumidity[CLIENT2] + 0.01) / 2;
  indoorPressure[FINAL] = (indoorPressure[CLIENT1] + indoorPressure[CLIENT2] + 0.01) / 2;
  indoorTemperature[FINAL] = (indoorTemperature[CLIENT1] + indoorTemperature[CLIENT2]+ 0.01) / 2;
  indoorTvoc[FINAL] = (indoorTvoc[CLIENT1] + indoorTvoc[CLIENT2] + 0.01) / 2;
  indoorPm25[FINAL] = (indoorPm25[CLIENT1] + indoorPm25[CLIENT2] + 0.01) / 2;
  indoorPm10[FINAL] = (indoorPm10[CLIENT1] + indoorPm10[CLIENT2] + 0.01) / 2;
  //every ten minutes
  if (millis() - lastMillis10Min >= 10 * 60 * 1000UL)
  {
    lastMillis10Min = millis();
    weatherJson = getWeatherJson(city);
    deserialize(weatherJson, 'W');
    pollutionJson = getPollutionJson(latitude, longitude);
    deserialize(pollutionJson, 'P');
  }

  if (millis() - lastMillis7Sec >= 7 * 1000UL)
  {
    lastMillis7Sec = millis();
    plot();
    Serial.println(String(latitude));
    if (ventilationDecision == 1)
    {
      Serial.println("Ventilation required");
    }
    else
    {
      Serial.println("Ventilation not required");
    }
  }
}
void initWiFi()
{
  unsigned long wifiLastMillis = millis();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");

    if (millis() - wifiLastMillis >= 5000UL)
    {
      wifiLastMillis = millis();
      Serial.println();
      initWiFi();
    }
    delay(1000);
  }

  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
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
    newCity = request->getParam("name")->value();
    request->send(200, "text/plain", "Received");
  });
  server.on("/json", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", airInformationJson); });
  //READINGS FROM CLIENT 1
  server.on("/readValue1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              indoorTemperature[CLIENT1] = request->getParam("temperature")->value().toFloat();
              indoorHumidity[CLIENT1] = request->getParam("humidity")->value().toFloat();
              indoorCo2[CLIENT1] = request->getParam("co2")->value().toFloat();
              indoorTvoc[CLIENT1] = request->getParam("tvoc")->value().toFloat();
              indoorPressure[CLIENT1] = request->getParam("pressure")->value().toFloat();
              indoorPm25[CLIENT1] = request->getParam("pm25")->value().toFloat();
              indoorPm10[CLIENT1] = request->getParam("pm10")->value().toFloat();
              request->send(200, "text/plain", "R");
            });
  //READINGS FROM CLIENT 2
  server.on("/readValue2", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              indoorTemperature[CLIENT2] = request->getParam("temperature")->value().toFloat();
              indoorHumidity[CLIENT2] = request->getParam("humidity")->value().toFloat();
              indoorCo2[CLIENT2] = request->getParam("co2")->value().toFloat();
              indoorTvoc[CLIENT2] = request->getParam("tvoc")->value().toFloat();
              indoorPressure[CLIENT2] = request->getParam("pressure")->value().toFloat();
              indoorPm25[CLIENT2] = request->getParam("pm25")->value().toFloat();
              indoorPm10[CLIENT2] = request->getParam("pm10")->value().toFloat();
              request->send(200, "text/plain", "R");
            });
  server.begin();
}
void plot()
{
  if (plotMetric == "temperature")
  {
    Serial.print("Indoor Temperature:");
    Serial.println(indoorTemperature[FINAL]);
    Serial.print("Outdoor Temperature:");
    Serial.println(outdoorTemperature);
  }
  else if (plotMetric == "humidity")
  {
    Serial.print("Indoor Humidity:");
    Serial.println(indoorHumidity[FINAL]);
    Serial.print("Outdoor Humidity:");
    Serial.println(outdoorHumidity);
  }
  else if (plotMetric == "co2")
  {
    Serial.print("Indoor CO2:");
    Serial.println(indoorCo2[FINAL]);
    Serial.print("Outdoor CO2:");
    Serial.println(400);
  }
  else if (plotMetric == "pressure")
  {
    Serial.print("Indoor Pressure:");
    Serial.println(indoorPressure[FINAL]);
    Serial.print("Outdoor Pressure:");
    Serial.println(outdoorPressure);
  }
  else if (plotMetric == "pm25")
  {
    Serial.print("Indoor PM2.5:");
    Serial.println(indoorPm25[FINAL]);
    Serial.print("Outdoor PM2.5:");
    Serial.println(outdoorPm25);
  }
  else if (plotMetric == "pm10")
  {
    Serial.print("Indoor PM10:");
    Serial.println(indoorPm10[FINAL]);
    Serial.print("Outdoor PM10:");
    Serial.println(outdoorPm10);
  }
  else if (plotMetric == "tvoc")
  {
    Serial.print("Indoor TVOC:");
    Serial.println(indoorTvoc[FINAL]);
    Serial.print("Outdoor TVOC:");
    Serial.println(0);
  }
  else if (plotMetric == "statistic")
  {

    Serial.println("LOG  " + String(millis() / (60 * 1000UL)) + " " + city);

    Serial.print("Indoor Temperature: ");
    Serial.print(indoorTemperature[FINAL]);
    Serial.println("°C");
    Serial.print("Outdoor Temperature: ");
    Serial.print(outdoorTemperature);
    Serial.println("°C");
    Serial.println();

    Serial.print("Indoor Humidity: ");
    Serial.print(indoorHumidity[FINAL]);
    Serial.println("%");
    Serial.print("Outdoor Humidity: ");
    Serial.print(outdoorHumidity);
    Serial.println("%");
    Serial.println(" ");

    Serial.print("Indoor CO2: ");
    Serial.print(indoorCo2[FINAL]);
    Serial.println(" ppm");
    Serial.print("Outdoor CO2: ");
    Serial.print(outdoorCo2);
    Serial.println(" ppm");
    Serial.println(" ");

    Serial.print("Indoor Pressure: ");
    Serial.print(indoorPressure[FINAL]);
    Serial.println(" hPa");
    Serial.print("Outdoor Pressure:");
    Serial.print(outdoorPressure);
    Serial.println(" hPa");
    Serial.println(" ");

    Serial.print("Indoor PM2.5: ");
    Serial.print(indoorPm25[FINAL]);
    Serial.println(" μg/m³");
    Serial.print("Outdoor PM2.5: ");
    Serial.print(outdoorPm25);
    Serial.println(" μg/m³");
    Serial.println(" ");

    Serial.print("Indoor PM10: ");
    Serial.print(indoorPm10[FINAL]);
    Serial.println(" μg/m³");
    Serial.print("Outdoor PM10: ");
    Serial.print(outdoorPm10);
    Serial.println(" μg/m³");
    Serial.println(" ");

    Serial.print("Indoor TVOC: ");
    Serial.print(indoorTvoc[FINAL]);
    Serial.println(" mg/m³");
    Serial.print("Outdoor TVOC:");
    Serial.print("<1");
    Serial.println(" mg/m³");
    Serial.println(" ");
  }
  Serial.println(" ");
}
String buildAirInformationJson()
{
  String airInformationJson = "{";
  airInformationJson += "\"indoorTemperature\":\"";
  airInformationJson += String(indoorTemperature[FINAL]);
  airInformationJson += "\",\"outdoorTemperature\":\"";
  airInformationJson += String(outdoorTemperature);
  airInformationJson += "\",\"indoorHumidity\":\"";
  airInformationJson += String(indoorHumidity[FINAL]);
  airInformationJson += "\",\"outdoorHumidity\":\"";
  airInformationJson += String(outdoorHumidity);
  airInformationJson += "\",\"indoorCo2\":\"";
  airInformationJson += String(indoorCo2[FINAL]);
  airInformationJson += "\",\"outdoorCo2\":\"";
  airInformationJson += String(outdoorCo2);
  airInformationJson += "\",\"indoorPressure\":\"";
  airInformationJson += String(indoorPressure[FINAL]);
  airInformationJson += "\",\"outdoorPressure\":\"";
  airInformationJson += String(outdoorPressure);
  airInformationJson += "\",\"indoorPm25\":\"";
  airInformationJson += String(indoorPm25[FINAL]);
  airInformationJson += "\",\"outdoorPm25\":\"";
  airInformationJson += String(outdoorPm25);
  airInformationJson += "\",\"indoorPm10\":\"";
  airInformationJson += String(indoorPm10[FINAL]);
  airInformationJson += "\",\"outdoorPm10\":\"";
  airInformationJson += String(outdoorPm10);
  airInformationJson += "\",\"indoorTvoc\":\"";
  airInformationJson += String(indoorTvoc[FINAL]);
  airInformationJson += "\",\"outdoorTvoc\":\"";
  airInformationJson += String(0);
  airInformationJson += "\",\"notification\":\"";
  airInformationJson += String(ventilationDecision);
  airInformationJson += "\",\"city\":\"";
  airInformationJson += String(city) + "\"";
  airInformationJson += "}";
  return airInformationJson;
}

float calculateIndoorHumidity(float _outdoorTemperatureLocal, float _outdoorHumidityLocal, float _indoorTemperatureLocal)
{
  //This function is to calculate the outdoor humidity if the outdoor air comes inside.
  float maxWaterVapor[71] = {0.9, 0.99, 1.08, 1.180, 1.29, 1.405, 1.53, 1.67, 1.82, 1.98, 2.15,
                             2.34, 2.55, 2.77, 3.005, 3.26, 3.53, 3.82, 4.14, 4.475, 4.84, 5.205, 5.590, 5.985, 6.395, 6.825,
                             7.28, 7.76, 8.27, 8.82, 9.4, 10, 10.65, 11.35, 12.1, 12.85, 13.65, 14.5, 15.4, 16.3, 17.3, 18.35,
                             19.4, 20.55, 21.8, 23.05, 24.35, 25.75, 27.2, 28.7, 30.35, 32.05, 33.85, 35.7, 37.65, 39.6, 41.7,
                             43.9, 46.2, 48.6, 51.15, 53.8, 56.7, 59.6, 62.5, 65.4, 68.5, 71.8, 75.3, 79, 83};
  float maxAbsHumidityOutdoor = maxWaterVapor[(int)(roundf(_outdoorTemperatureLocal) + 20)];
  float absHumidityOutdoor = (_outdoorHumidityLocal / 100) * maxAbsHumidityOutdoor;
  float maxAbsHumidityIndoor = maxWaterVapor[(int)(roundf(_indoorTemperatureLocal) + 20)];
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

bool decideVentilation()
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
  categorizeAirQualityMetrics(indoorTemperature[FINAL], indoorHumidity[FINAL], indoorTvoc[FINAL], indoorCo2[FINAL], indoorPm25[FINAL], indoorPm10[FINAL]);
  maxValueAqIndoor = getHighestElement();
  numberOfHighestElementAqIndoor = getNumberOfHighestElement(maxValueAqIndoor);

  float extrapolatedHumidityIndoor = calculateIndoorHumidity(outdoorTemperature, outdoorHumidity, indoorTemperature[FINAL]);
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
    airQualityCategories[TEMPERATURE] = 1;
  }
  else if (temperature <= 39)
  {
    airQualityCategories[TEMPERATURE] = 2;
  }
  else if (temperature <= 46)
  {
    airQualityCategories[TEMPERATURE] = 3;
  }
  else if (temperature > 46)
  {
    airQualityCategories[TEMPERATURE] = 4;
  }

  if (humidity >= 30 || humidity <= 60)
  {
    airQualityCategories[HUMIDITY] = 1;
  }
  else if (humidity < 30 || humidity > 60)
  {
    airQualityCategories[HUMIDITY] = 2;
  }

  if (tvoc <= 1)
  {
    airQualityCategories[TVOC] = 1;
  }
  else if (tvoc <= 10)
  {
    airQualityCategories[TVOC] = 2;
  }
  else if (tvoc <= 25)
  {
    airQualityCategories[TVOC] = 3;
  }
  else if (tvoc > 25)
  {
    airQualityCategories[TVOC] = 4;
  }

  if (co2 <= 1000)
  {
    airQualityCategories[CO2] = 1;
  }
  else if (co2 <= 2000)
  {
    airQualityCategories[CO2] = 2;
  }
  else if (co2 <= 5000)
  {
    airQualityCategories[CO2] = 3;
  }
  else if (co2 > 5000)
  {
    airQualityCategories[CO2] = 4;
  }

  if (pm2_5 <= 10)
  {
    airQualityCategories[PM25] = 1;
  }
  else if (pm2_5 <= 25)
  {
    airQualityCategories[PM25] = 2;
  }
  else if (pm2_5 <= 50)
  {
    airQualityCategories[PM25] = 3;
  }
  else if (pm2_5 > 50)
  {
    airQualityCategories[PM25] = 4;
  }

  if (pm10 <= 20)
  {
    airQualityCategories[PM10] = 1;
  }
  else if (pm10 <= 50)
  {
    airQualityCategories[PM10] = 2;
  }
  else if (pm10 <= 100)
  {
    airQualityCategories[PM10] = 3;
  }
  else if (pm10 > 100)
  {
    airQualityCategories[PM10] = 4;
  }
}
