#include "ArduinoSecrets.h"
#include <HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>

extern float outdoorTemperature, outdoorPressure, outdoorHumidity, outdoorPm25, outdoorPm10, latitude, longitude;

StaticJsonDocument<1024> weatherMetrics;
StaticJsonDocument<1024> pollutionMetrics;
String getWeatherJson(String city)
{
  String payload;
  int httpCode;
  if ((WiFi.status() == WL_CONNECTED))
  { //Check the current connection status
    HTTPClient http;
    http.begin("http://pro.openweathermap.org/data/2.5/weather?q=" + city + "&APPID=" + API_KEY+"&units=metric");
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
  return payload;
  //return "{\"coord\":{\"lon\":8.6728,\"lat\":49.9933},\"weather\":[{\"id\":801,\"main\":\"Clouds\",\"description\":\"few clouds\",\"icon\":\"02d\"}],\"base\":\"stations\",\"main\":{\"temp\":23.31,\"feels_like\":23.37,\"temp_min\":18.82,\"temp_max\":25.79,\"pressure\":1012,\"humidity\":64},\"visibility\":10000,\"wind\":{\"speed\":0.45,\"deg\":149,\"gust\":2.68},\"clouds\":{\"all\":20},\"dt\":1624908040,\"sys\":{\"type\":2,\"id\":2005640,\"country\":\"DE\",\"sunrise\":1624850314,\"sunset\":1624909102},\"timezone\":7200,\"id\":2881279,\"name\":\"Langen\",\"cod\":200}";
}

String getPollutionJson(float lat, float lon)
{
  String payload;
  int httpCode;
  if ((WiFi.status() == WL_CONNECTED))
  { //Check the current connection status
    HTTPClient http;
    http.begin("http://pro.openweathermap.org/data/2.5/air_pollution?lat="+String(lat)+"&lon="+lon+"&appid="+API_KEY);
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
  return payload;
  // return "{\"coord\":{\"lon\":49.99,\"lat\":8.67},\"list\":[{\"main\":{\"aqi\":5},\"components\":{\"co\":191.93,\"no\":0,\"no2\":0.06,\"o3\":23.96,\"so2\":0.08,\"pm2_5\":66.94,\"pm10\":359.45,\"nh3\":0.08},\"dt\":1624910400}]}";
}

void deserialize(String metricJson, char flag)
{
  if (flag == 'W')
  {
    deserializeJson(weatherMetrics, metricJson);

    longitude = weatherMetrics["coord"]["lon"].as<float>();
    latitude = weatherMetrics["coord"]["lat"].as<float>();
    outdoorTemperature = weatherMetrics["main"]["temp"].as<float>();
    outdoorHumidity = weatherMetrics["main"]["humidity"].as<float>();
    outdoorPressure = weatherMetrics["main"]["pressure"].as<float>();
  }
  else if (flag == 'P')
  {
    deserializeJson(pollutionMetrics, metricJson);
    outdoorPm25 = pollutionMetrics["list"][0]["components"]["pm2_5"].as<float>();
    outdoorPm10 = pollutionMetrics["list"][0]["components"]["pm10"].as<float>();
  }
}
