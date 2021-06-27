#include "ArduinoSecrets.h"
#include <HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>

extern float temperature, pressure, humidity, latitude, longitude;
StaticJsonDocument<1024> doc;
String getWeatherJson(String city)
{
  String payload;
  int httpCode;
  if ((WiFi.status() == WL_CONNECTED))
  { //Check the current connection status
    HTTPClient http;
    //http.begin("http://pro.openweathermap.org/data/2.5/weather?q=" + city + "&APPID=" + API_KEY);
    httpCode = http.GET(); //Make the request

    if (httpCode > 0)
    { //Check for the returning code
      payload = http.getString();
    }
    else
    {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
  return "{\"coord\": {\"lon\": 8.6728,\"lat\": 49.9933},\"weather\": [{\"id\": 801,\"main\": \"Clouds\",\"description\": \"few clouds\",\"icon\": \"02d\"}],\"base\": \"stations\",\"main\": {\"temp\": 298.85,\"feels_like\": 298.86,\"temp_min\": 296.97,\"temp_max\": 300.46,\"pressure\": 1016,\"humidity\": 53},\"visibility\": 10000,\"wind\": {\"speed\": 0.89,\"deg\": 101,\"gust\": 4.47},\"clouds\": {\"all\": 20},\"dt\": 1624787941,\"sys\": {\"type\": 2,\"id\": 2005640,\"country\": \"DE\",\"sunrise\": 1624763884,\"sunset\": 1624822709},\"timezone\": 7200,\"id\": 2881279,\"name\": \"Langen\",\"cod\": 200}";
}
void deserialize(String json)
{
  deserializeJson(doc, json);
  longitude = doc["coord"]["lon"].as<float>();
  latitude = doc["coord"]["lat"].as<float>();
  temperature = doc["main"]["temp"].as<float>();
  humidity = doc["main"]["humidity"].as<float>();
  pressure = doc["main"]["pressure"].as<float>();
}
