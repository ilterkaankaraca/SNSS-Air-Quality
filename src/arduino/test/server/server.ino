# include <WiFi.h>
# include "ESPAsyncWebServer.h"
char ssid[] = "";       //  your network SSID (name)
char password[] = "";  
typedef enum {
    START,
    READ,
    TRANSMIT,
    WAIT
    } AppState_t ;
    AppState_t state = START ;


AsyncWebServer server (80) ;
float tempValue ;
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
  // switch ( state ){
  //       case START:
  //           conf_register &= -MCP9808_CONFIG_SHUTDOWN;
  //           state = READ_TEMP_MCP9808 ;
  //           break ;
  //       case READ:
  //           conf_register &= -MCP9808_CONFIG_SHUTDOWN;
  //           state = READ_TEMP_MCP9808 ;
  //           break ;
  //       case TRANSMIT:
  //           conf_register &= -MCP9808_CONFIG_SHUTDOWN;
  //           state = READ_TEMP_MCP9808 ;
  //           break ;
  //       case WAIT:
  //           conf_register &= -MCP9808_CONFIG_SHUTDOWN;
  //           state = READ_TEMP_MCP9808 ;
  //           break ;
  acceptClient(request)
  }
  });
  server.begin ();
}
void acceptClient(AsyncWebServerRequest *request){
  String message = String(request -> getParam ("message")->value());
  Serial.println ("Rx from client : " + request -> client () -> remoteIP().toString ());
  Serial.println ("Client connected: "+message);
  request -> send (200 , "text / plain ", "connected");
}
void WiFiStationConnected ( WiFiEvent_t event , WiFiEventInfo_t info ) {
  Serial.print ( String ( info.sta_connected.mac [0] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [1] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [2] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [3] , HEX ) + ":");
  Serial.print ( String ( info.sta_connected.mac [4] , HEX ) + ":");
  Serial.println ( String ( info.sta_connected.mac [5] , HEX ) + " connected ");
}
