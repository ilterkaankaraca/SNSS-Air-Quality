/* ESP32 as Station */
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>

# define MCP9808_I2C_ADDRESS 0x18
# define MCP9808_TEMPERATURE_REGISTER 0x05
# define MCP9808_CONFIG_REGISTER 0x01
# define MCP9808_CONFIG_SHUTDOWN 0x0100

char ssid[] = "";       //  your network SSID (name)
char password[] = "";  
uint16_t conf_register ;
String serverName = "http ://192.168.0.247/";
float tempValue =23.75;

uint8_t upperByte, lowerByte ;
float temperatur;
void setup () {
  Serial.begin (115200) ;
  initWiFi ();
   while(!Serial);
    Wire.begin () ; // activate the i2c function on the micrcontroller
    Wire.beginTransmission ( MCP9808_I2C_ADDRESS ) ;
    Wire.write ( MCP9808_CONFIG_REGISTER ) ;
    Wire.write ( conf_register >>8) ;
    Wire.write ( conf_register ) ;
    Wire.endTransmission () ;
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

long lastTime ;
long waitTime = 5000; // Waittime in ms
int httpCode=0;
typedef enum {
    WAKEUP_MCP9808,
    READ_TEMP_MCP9808,
    SHUTDOWN_MCP9808,
    CALCULATE_TEMP,
    TRANSMIT_TEMP ,
    WAIT
    } AppState_t ;
    AppState_t state = WAKEUP_MCP9808 ;

void loop () {
    switch ( state ){
        case WAKEUP_MCP9808 :
            conf_register &= -MCP9808_CONFIG_SHUTDOWN;
            state = READ_TEMP_MCP9808 ;
            break ;
        case READ_TEMP_MCP9808 :
            Wire.requestFrom ( MCP9808_I2C_ADDRESS , 2) ;
            Wire.beginTransmission ( MCP9808_I2C_ADDRESS ) ;
            Wire.write ( MCP9808_TEMPERATURE_REGISTER ) ;
            Wire.endTransmission () ;
            Wire.requestFrom ( MCP9808_I2C_ADDRESS , 2) ;
            if (2 <= Wire.available () ) {
              upperByte = Wire.read () ;
              lowerByte = Wire.read () ;
            }
            state = SHUTDOWN_MCP9808 ;
            break ;
        case SHUTDOWN_MCP9808 :
            conf_register |= MCP9808_CONFIG_SHUTDOWN;
            state = CALCULATE_TEMP ;
            break ;
        case CALCULATE_TEMP :
            conf_register |= MCP9808_CONFIG_SHUTDOWN;
            temperatur =( upperByte & 0x0F) *16+ float ( lowerByte ) /16;
            if (( upperByte & 0x10 ) == 0x10 ) { // temperature < 0 degree C
              temperatur =256 - temperatur ;
            }
            Serial.print ( temperatur ,4) ;
            Serial.println ("*C") ;
            state = TRANSMIT_TEMP ;
            break ;
        case TRANSMIT_TEMP :
            if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
                HTTPClient http;
                http.begin("http://192.168.0.247/getValue/?temperature="+ String(temperatur)); //Specify the URL
                httpCode = http.GET();                                        //Make the request
                if (httpCode > 0) { //Check for the returning code
                    String payload = http.getString();
                }
                else {
                    Serial.println("Error on HTTP request");
                }
                http.end(); //Free the resources
            }
            lastTime = millis ();
            state = WAIT ;
            break ;
        case WAIT :
            if ( millis () > waitTime + lastTime ) {
                state = WAKEUP_MCP9808 ;
            }
    }
}
