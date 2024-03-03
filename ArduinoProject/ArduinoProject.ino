#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"
#include "Arduino_Wifi_Connect.h"
#include "WiFiS3.h"
#include <WiFiClient.h>
#include <Arduino_JSON.h>
#include <Bridge.h>
#include <HttpClient.h>


ArduinoLEDMatrix matrix;

const char *ssid = WIFI_SSID;
const char *pass = WIFI_PASS;

int keyIndex = 0;  
int led =  LED_BUILTIN;
int status = WL_IDLE_STATUS;

WiFiServer server(80);
WiFiClient wifi_client;
HttpClient http_client;

//const char * server_name = "www.google.com";    // name address for Google (using DNS)
String city_name = "Timisoara";
String country_code = "RO";
String api_key = "xxx";
String server_url = "http://api.openweathermap.org/data/2.5/weather?q=" + city_name + "," + country_code + "&APPID=" + api_key;
int loop_counter = 0;

String json_buffer;

void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  init_matrix("UNO", 1000);

  while (!Serial);
     // wait for serial port to connect. Needed for native USB port only
  
  setup_wifi_connection();

  //print_initial_wifi_status();

  setup_http_connection();
}

void loop() 
{
  // check the network connection once every 10 seconds:
  delay(10000);

  print_loop_counter();
  //print_current_wifi_status();

  continue_http_connection();

  matrix_print_text("    DECI EU NU CRED CA MERGE ASTA    ", 50, 0);
}



void setup_http_connection()
{
  Serial.println("\nStarting connection to server...");

  http_client.begin(server_url);

  if(http_client.available())
  {
    Serial.println("Sa mori tu ca merge");
  }
  else
  {
    Serial.println("Ai belit pl vere");
    while(true);;
  }
}


void continue_http_connection()
{
  unsigned int http_code = http_client.get(server_url);

  if(http_client.available())
  {
    Serial.println("Connection to server is stable");
    
    Serial.println("Getting data from server...");

    json_buffer = http_get_json_string();

    Serial.println(server_url);
  }
  else 
  {
    Serial.println("Connection to server lost!");
    while(true);
  }
}


String http_get_json_string()
{
  return "";
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
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
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



