#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "ajay" //hotspot name 
#define WLAN_PASS       "12345678" //hotspot password

#define AIO_SERVER      "io.adafruit.com" //server name
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME  "Ajay_Devil" //Adafruit username
#define AIO_KEY       "768e605824cb4cc18375163949b80589" //Session key

// Create an ESP8266 WiFiClient class to connect to the MQTT server.

WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY); //MQTT client


// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

Adafruit_MQTT_Publish Airquality = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Air_Quality"); //Air quality value publish


// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug)
.
void MQTT_connect(); //connecting to MQTT server

void setup() {
  Serial.begin(115200); //setting up baud rate 
  delay(10);

  // Connect to WiFi access point.
  
  Serial.println(); Serial.println();
  Serial.print("Connecting to "); //connecting to WLAN
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) { //checking whether the WiFi is connected or not
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

}

uint32_t x=0;

void loop() {
  MQTT_connect();
  x = analogRead(A0);
  
  // Now we can publish stuff!
  
  Serial.print(F("\nSending Air Quality value :  ")); //sending out air quality value 
  Serial.println(x);
  Serial.print("...");
  if (!Airquality.publish(x)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  delay(5000); //delay of 5 seconds
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
                while (1);
       }
  }
  Serial.println("MQTT Connected!"); //checking whether it is connected or not
}
