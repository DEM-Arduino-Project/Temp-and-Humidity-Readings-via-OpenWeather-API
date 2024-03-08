// LIBRARIES
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include <WiFiS3.h>
#include <ArduinoJson.h>
#include "Arduino_Wifi_Connect.h"
#include <string.h>
// DEBUG SWITCH

#define WIFI_DEBUG 0
#define SHOW_SERVER_DATA 0

ArduinoLEDMatrix matrix;
WiFiClient wifi_client;

String http_string;
String json_buffer;
StaticJsonDocument<200> json_doc;
DeserializationError err;

int led =  LED_BUILTIN;
int wifi_status = WL_IDLE_STATUS;
int loop_counter = 0;

void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // init the LED matrix
  init_matrix("UNO", 1000);

  while (!Serial); // wait for serial port to connect. Needed for native USB port only
  
  // init wifi and check connection
  setup_wifi_connection();

  // print wifi connection data
  if(WIFI_DEBUG == 1)
    print_initial_wifi_status();
}

void loop() 
{
  delay(10000); 

  matrix_print_text("    Hello World!    ", 50, 0);

  // initialize server connection
  setup_http_connection();

  // get data
  parse_json_buffer();

  // print api data

  String temp = "Temperature : " + String(get_temp()) + "C";
  String humidity = "Humidity : " + String(get_humidity()) + "%";

  Serial.println(temp);
  Serial.println(humidity);

  matrix_print_text("    " + temp, 50, 0);

  matrix_print_text("    " + humidity, 50, 0);

  Serial.println();
  
  // reset connection
  wifi_client.stop();
}

void parse_json_buffer()
{
  // get the api key data
  http_string = get_json();

  // get the standalone json string
  json_buffer = http_string.substring(http_string.indexOf('{'));

  if (SHOW_SERVER_DATA == 1)
    Serial.println(json_buffer);
  
  // check for json parse error
  err = deserializeJson(json_doc, json_buffer);

  if(err)
  {
    Serial.println("Error parsing JSON");
    Serial.println(err.f_str());
    return;
  }

  Serial.println("Data read successfully");
  Serial.println();
}

float get_temp()
{
  return (float(json_doc["main"]["temp"]) - 273.15);
} 

float get_humidity()
{
  return (json_doc["main"]["humidity"]);
}

void setup_http_connection()
{
  Serial.println("Attempting to connect to " + String(HOST_NAME));

  // connect to web server on port 80:
  if (wifi_client.connect(HOST_NAME, HTTP_PORT)) 
  {
    // if connected:
    Serial.println("Connected to server succesfully.");

    // make a HTTP request:
    // send HTTP header
    wifi_client.println(HTTP_METHOD + " " + PATH_NAME + " HTTP/1.1");
    wifi_client.println("Host: " + String(HOST_NAME));
    wifi_client.println("Connection: close");
    wifi_client.println();  // end HTTP header
  } 
  else 
  {  
    // if not connected:
    Serial.println("Connection to server failed.");
  }
}

String get_json()
{
  String buffer;
  while (wifi_client.connected()) 
  {
    if (wifi_client.available()) 
    {
      // read an incoming byte from the server and print it to serial monitor:
      buffer = wifi_client.readString();
    }
  }

  return buffer;
}

void setup_wifi_connection()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.print("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
  {
    Serial.print("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (wifi_status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_SSID);

    // Connect to WPA/WPA2 network:
    wifi_status = WiFi.begin(WIFI_SSID, WIFI_PASS);

    // wait 5 seconds for connection:
    delay(5000);
  }

  // you're connected now, so print out the data:
  Serial.println("You're connected to the network!");

  if(WIFI_DEBUG == 1)
    print_initial_wifi_status();
}

void init_matrix(String text, unsigned long delay_ms)
{
  matrix.begin();

  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  // add some static text
  // will only show "UNO" (not enough space on the display)
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText();

  matrix.endDraw();

  delay(delay_ms);
}

void matrix_print_text(String text, unsigned long speed, unsigned long delay_ms)
{
  // Make it scroll!
  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(speed);

  // add the text
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
  delay(delay_ms);
}

void print_loop_counter()
{
  Serial.println("Loop: " + String(loop_counter++));
}

void print_initial_wifi_status() 
{
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  print_mac_address(mac);
  Serial.println();
}

void print_current_wifi_status() 
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  print_mac_address(bssid);
  Serial.println();
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal Strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void print_mac_address(byte mac[]) 
{
  for (int i = 0; i < 6; i++) {
    if (i > 0) {
      Serial.print(":");
    }
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
  }
}



