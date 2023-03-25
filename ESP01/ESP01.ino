/*
  Code uploaded to ESP8266
  Tools -> Board -> ESP8266 Boards
  COM19 115200 Bauds

  Libraries:
    LegoPowerFunction: https://drive.google.com/file/d/0B8Bxk35bJ4FPZ3lJNV95M1lVa2c/view?resourcekey=0-J2fHcSlB5B2shtvGF4VTjg
    PubSubClient:https://pubsubclient.knolleary.net/
    ESP8266WiFi & ESP8266WiFiMulti enabled if ESP8266 Boards selected
      source https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi/src
    ArduinoJson: standard library https://arduinojson.org/
    LedFlasher: tutorial https://www.forward.com.au/pfod/ArduinoProgramming/TimingDelaysInArduino.html

  Note:
    Wifi communication: WifiClient + PubSubClient
    UART serial communication: Serial.xxxx
*/
# define TEST true
// Test = true ==> Debug and test messages are sent to Serial --> PC usb
// Test = false ==> Serial send message to Arduino. No test/debug message sent to Arduino

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
//#include "ledflasher.h"

#define MQTT_CLIENTID "Bras_robot-ESP01"

// Version
const char* applicationName = "Bras_robot_ESP01";
const char* filename = "Bras_robot_ESP01";
const char* appVersion = "1.1";


// Connect to multiple Wifi
const char* ssid[] = {"TP-Link_04AC", "Bbox-B49AD598", "?=+haut"};
const char* password[] = {"12806591", "AneRTVjnNTvaJNz6fY", "?=+GrandEstSIR"};
const int wifi_count = 3;
ESP8266WiFiMulti WiFiMulti;

void wifi_connect(bool debug=false) 
{
  Serial.print("Wait for WiFi");
  if(debug) Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA);                    // Station mode
  for (int i = 0; i < wifi_count; i++) {
    WiFiMulti.addAP(ssid[i], password[i]);
  }

  while (WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
      if(debug) WiFi.printDiag(Serial);          // for troubleshooting purpose
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT Broker
const char *mqtt_broker = "192.168.0.152";         // Enter your WiFi or Ethernet IP (192.168.1.87 = Ip du laptop LeNovo)
                          const char *topic_data = "BR/status";
                          const char *topic_cmd = "BR/cmd";
                          const char *topic_err = "BR/error";
                          const char *topic_msg = "BR/msg";
                          const char *topic_esp01 = "BR/esp01";

                          const int mqtt_port = 1883;
                          WiFiClient espClient;
                          PubSubClient client(espClient);

                          // Buffer for json message received from Arduino (serial)
#define MSG_BUFFER_SIZE  (512)
char msgBuffer[MSG_BUFFER_SIZE];

// Led mnagement
//#define LED_PIN 2                    // GPIO2 for ESP-01S
//LEDFlasher flasher;

/* ------ Wifi connection --------------------------*/
// Return true : connected to Wifi
//        false: not connected to Wifi
bool connect2Wifi(char *SSID, char *pwd)
{
  Serial.print("Connecting to "); Serial.println(SSID);
  WiFi.begin(SSID, pwd);
  int count = 0;

  while ((WiFi.status() != WL_CONNECTED) && (count < 20)) {
    delay(500);
    Serial.print(F("."));
    count++;
  }
  return WiFi.status() == WL_CONNECTED;
}

//============ MQTT function =======================================================
void reconnect_MQTT() {
  while (!client.connected()) {
    // client_id += String(WiFi.macAddress());
    // String clientId = "ESP8266Client-";
    // clientId += String(random(0xffff), HEX);

    if (TEST) {
      Serial.printf("Client %s connecting to mosquitto mqtt broker...\n", MQTT_CLIENTID);
    }

    if (client.connect(MQTT_CLIENTID)) {

      // publish and subscribe
      client.publish(topic_esp01, "Hello From Lego Station 01 (ESP-01)");
      //client.subscribe(topic_data); // ben non sinon on reçoit tous les messages envoyés
      client.subscribe(topic_cmd);
      //      if(TEST) {
      //        Serial.println("Connected to MQTT broker");
      //        Serial.print("Subscribe to "); Serial.println(topic_cmd);
      //      }
    } else {
      //      if(TEST) {
      //        Serial.print("failed with state ");
      //        Serial.println(client.state());
      //      }
      delay(2000);
    }
  }
}

// ==============================================================
// MQTT message received and forwarded to Arduino
//
void callback(char *topic, byte * payload, unsigned int length) {
  if (TEST) {
    client.publish(topic_esp01, "Message received with topic: ");
    client.publish(topic_esp01, topic);
    //    client.publish(topic_esp01,"Message:"));
  }

  // Forward to Arduino
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
  }

  Serial.println();
  client.publish(topic_esp01, "forwarded");
}


// ==============================================================
// Receiving message from Arduino
// Global var used by GetJsonMsgFromArduino
uint8_t openedBracket;
bool jsonStarted;
//static char readBuff[512] = "";

//
// Read json message fromo serial port (Arduino)
// Return true when message is completed
//        false when no message or incompleted message
//
String stringBuffer = "";
bool GetJsonMsgFromArduino(void)
{
  bool result = false;
  if (Serial.available() > 0)
  {

    do {
      uint8_t c = Serial.read();
      if (!jsonStarted && (c == '{')) {
        stringBuffer = "";  // Clear buffer
        stringBuffer += (char)c;
        jsonStarted = true;
        openedBracket++;

      } else {
        stringBuffer += (char)c;        // json already started

        switch (c) {
          case '{':
            openedBracket++;
            break;
          case '}':
            openedBracket--;
            break;
        }
      }
    } while ((jsonStarted || openedBracket >= 0) && Serial.available() > 0);

    // End of json message from arduino ??
    if (jsonStarted && (openedBracket == 0))
    {
      stringBuffer.toCharArray(msgBuffer, MSG_BUFFER_SIZE);
      client.publish(topic_msg, msgBuffer);
      result = true;      // json message completed
      openedBracket = 0;
      jsonStarted = false;
      stringBuffer = "";
    }
  }
  return result;
}


void ProcessJsonMsgFromArduino(char* jsonMsg)
{
  client.publish(topic_msg, jsonMsg);
  /*
    //Serial.println(F("[start processing msg...]"));
    if (true == jsonMsg.equals("{BT_detection}"))
    {
    Serial.print("{BT_OK}");
    //Serial.println("Factory...");
    } else {
    // Deserialize to get the command type
    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, jsonMsg);
    if (err) {
      Serial.print(F("deserializeJson failed: "));
      Serial.println(err.c_str());
      Serial.println(jsonMsg);
    } else {
      // is it a get time command from Arduino ?
      // {"cmd":"get","d1":"time"}
      if ((doc["cmd"] == "get") && (doc["d1"] == "time"))
      {
        // Forward to IotEdge
        //Serial.println(F("[cmd get time received from Arduino]"));
        String response = Request2SocketServer(jsonMsg);
        Serial.print(F("[Response forwarded to Arduino: ")); Serial.println(response);
        serial2Arduino.print(response);
      }
      else if (doc["type"] == "telemetry" ) {
        //{"device":"ElegooTank","type":"telemetry","time":"tbc","sensor":"battery","data":4.70}
        //-------- Send telemetry to iotHub -----------
        int str_len = jsonMsg.length() + 1;
        char char_array[str_len]; // Prepare the character array (the buffer)
        jsonMsg.toCharArray(char_array, str_len);
        if(mqttClient.publish(topic_data, char_array)){
          Serial.println(F("[Telemetry mqttClient.publish ok]"));
        } else {
          Serial.println(F("[mqttClient.publish failed]"));
          // It will return false if:
          //    the client was not currently connected to the server, or
          //    the resulting MQTT packet to exceeded the libraries maximum packet size
        }
        //-------- Update twin reported property -------------
        // get the JsonObject in the JsonDocument
        JsonObject root = doc.as<JsonObject>();
        setTwinProperty(root["sensor"], doc["data"]);
      } else if (doc["type"] == "status" ) {
        JsonObject root = doc.as<JsonObject>();
        setTwinProperty(root["prop"], root["data"]);
      } else {
        Serial.println(F("[Arduino unknown message type]"));
      }

    }
    //Serial.println(jsonMsg);         // Send command reponse to socket client
    }
  */
}


// Transform strign to json message and publish it to broker
void publishErrorMsg(char* errMsg)
{
  char jsonMsg[128];
  strcpy(jsonMsg, "{""err"":""");
  strcat(jsonMsg, errMsg);
  strcat(jsonMsg, """}");
  client.publish(topic_err, jsonMsg);
}

void publishJsonMsg(char* msg)
{
  char jsonMsg[128];
  strcpy(jsonMsg, "{""msg"":""");
  strcat(jsonMsg, msg);
  strcat(jsonMsg, """}");
  client.publish(topic_msg, jsonMsg);
}


//==================================================================================
void setup() {

  Serial.begin(9600);

  if (TEST)  {
    Serial.print(applicationName); Serial.print(" v"); Serial.print(appVersion); Serial.print(" ("); Serial.print(filename); Serial.println(")");
    // We start by connecting to a WiFi network
    Serial.print(wifi_count, DEC); Serial.println(F(" wifi networks"));
  }
  

  wifi_connect();

  if (TEST)  {
    Serial.println(F("Connected to the WiFi network"));
    Serial.print(F("IP address: ")); Serial.println(WiFi.localIP());
  }

  msgBuffer[0] = '\0';      // Empty Arduino message buffer

  //------------ connecting to a mqtt broker -------------------
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  client.publish(topic_esp01, "Hello from ESP-01");
  // Led
  //flasher.begin(LED_PIN);
  //flasher.flash(5);
}



//==================================================================================
void loop() {
  if (WiFiMulti.run() != WL_CONNECTED) {
    wifi_connect();
  }
  if (!client.connected()) {
    reconnect_MQTT();
  }
  client.loop();

  //flasher.update();   // should call this often, atleast every loop()

  // Get and process messages from Arduino
  if (GetJsonMsgFromArduino()) {
    ProcessJsonMsgFromArduino(msgBuffer);
    msgBuffer[0] = '\0';
  }

  /*
    Send data to broker

    char buffer[4];
    DynamicJsonDocument doc(1024);
    doc["sensor"] = "DHT11";
    doc["time"]   = millis();

    itoa((int)temperature, buffer, 10);
    doc["temp"] = buffer;
    itoa((int)humidity, buffer, 10);
    doc["humidity"] = buffer;

    serializeJson(doc, msg);

    //int temp = 25;
    //snprintf (msg, MSG_BUFFER_SIZE, "{ temp: %ld }", temp);
    client.publish(topic_data, msg);

    // DHT11 sampling rate is 1HZ.

  */

}
