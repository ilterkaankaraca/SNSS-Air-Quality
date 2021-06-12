# include <WiFi.h>
# include "ESPAsyncWebServer.h"
char ssid[] = "Vodafone-262F_T";       //  your network SSID (name)
char password[] = "Tarti.38";  


AsyncWebServer server (80) ;
String tempValue ;
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
  server.on("/", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , " text / plain ", "hey");
  });
  server.on("/connect", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , " text / plain ", "hey1");
  });
  server.on("/temperature", HTTP_GET, []( AsyncWebServerRequest *request ) {
    request -> send (200 , "text/plain", tempValue);
  });
  server.on("/getValue", HTTP_GET, []( AsyncWebServerRequest *request ) {
    tempValue = String(request -> getParam ("temperature")->value());
    request -> send (200 , "text/plain", "R");
  });
  server.begin ();
}
void acceptClient(AsyncWebServerRequest *request){
  String message = String(request -> getParam ("message")->value());
  //Serial.println ("Rx from client : " + request -> client () -> remoteIP().toString ());
  //Serial.println ("Client connected: "+message);
  request -> send (200 , " text / plain ", " data received ");
}
void WiFiStationConnected ( WiFiEvent_t event , WiFiEventInfo_t info ){
  Serial.print ( String ( info.sta_connected.mac [0] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [1] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [2] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [3] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [4] , HEX ) + ":");
  Serial.println ( String ( info.sta_connected.mac [5] , HEX ) + " connected ");
}
