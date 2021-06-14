# include <WiFi.h>
# include "ESPAsyncWebServer.h"
char ssid[] = "";       //  your network SSID (name)
char password[] = "";  


AsyncWebServer server (80) ;
String temperatureValue, humidityValue, co2Value, airQualityValue, pressureValue, particleValue;
void setup () {
  Serial.begin (115200) ;
  initWiFi ();
  initWebserver ();
}
void loop () {
}
void initWiFi () {
    WiFi.mode ( WIFI_STA );
    WiFi.begin (ssid , password );
    Serial.print (" Connecting to WiFi ..");
    while ( WiFi.status () != WL_CONNECTED ) {
      Serial.print (".");
        delay (1000) ;
    }
    Serial.println ( WiFi.localIP ());
    Serial.println ( WiFi.macAddress ());
}
void initWebserver ( void ) {
  server.on("/connect", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , " text / plain ", "connected");
  });
  server.on("/temperature", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , "text/plain", temperatureValue);
  });
  server.on("/humidity", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , "text/plain", humidityValue);
  });
  server.on("/co2", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , "text/plain", co2Value);
  });
  server.on("/airQuality", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , "text/plain", airQualityValue);
  });
  server.on("/pressure", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , "text/plain", pressureValue);
  });
  server.on("/particle", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , "text/plain", particleValue);
  });
  server.on("/readValue", HTTP_GET, []( AsyncWebServerRequest *request ) {
    temperatureValue = String(request -> getParam ("temperature")->value());
    humidityValue = String(request -> getParam ("humidity")->value());
    co2Value = String(request -> getParam ("co2")->value());
    airQualityValue = String(request -> getParam ("airQuality")->value());
    pressureValue = String(request -> getParam ("pressure")->value());
    particleValue = String(request -> getParam ("particle")->value());
    request -> send (200 , "text/plain", "R");
  });
  server.begin ();
}

void WiFiStationConnected ( WiFiEvent_t event , WiFiEventInfo_t info ){
  Serial.print ( String ( info.sta_connected.mac [0] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [1] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [2] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [3] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [4] , HEX ) + ":");
  Serial.println ( String ( info.sta_connected.mac [5] , HEX ) + " connected ");
}
