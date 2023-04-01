# 1 "/home/equipe2/BrasRobot/Arduino/Arduino.ino"

/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

# 19 "/home/equipe2/BrasRobot/Arduino/Arduino.ino" 2
# 20 "/home/equipe2/BrasRobot/Arduino/Arduino.ino" 2

// Version
const char* applicationName = "iot_library";
const char* filename = "iot_device.ino";
const char* appVersion = "1.00";

static String SerialMsgReceived = "";







const int Tour_10 = 32000; // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper socle(200, 2, 5);
Stepper coude(200, 3, 6);
Stepper rot_pince(200, 4, 7);
Stepper hauteur(200, 12, 13);

void Init(void)
{
  pinMode(11, 0x2);
  pinMode(10, 0x2);
  pinMode(9, 0x2);
  pinMode(A3, 0x2);
    /* initialiser les périphériques
    bool res_error = true;
    DriverMotor.Motor_Init();
    DriverServo.Servo_Init(90);
    DriverVoltage.Voltage_Init();
    DriverUltrasonic.Ultrasonic_Init();
    DriverLedRGB.Init(20);
    byte status = TankMPU6050.Init(); // return init status. If status != 0 then MPU6050 ko
    if(status!=0) {
        Serial.println("MPU6050 Init error");
    }

    tankStatus.Functional_Mode = Standby_mode;
    */
}

/* ====================================================================================
Serial port data read 
  - read data received from ESP32 (json format expected)
  - Deserialize json data received 
  - Execute received command
*/
static String SerialPortBuffer = "";
static uint8_t openedBracket = 0;
static bool jsonStarted = false;

String SerialPortJsonReception()
{
  char c;
  String result = "";
  if (Serial.available() > 0)
  {
    do {
      c = Serial.read();
      Serial.print(c); // echo
      SerialPortBuffer += c;
      switch(c) {
        case '{':
          openedBracket++;
          jsonStarted = true;
          break;
        case '}':
          openedBracket--;
          break;
      }
    } while((jsonStarted || openedBracket >= 0) && Serial.available() > 0);
  }

  if (jsonStarted && (openedBracket == 0)) // End of message received
  {
    //Serial.println("end of received message");
    result = SerialPortBuffer;
    SerialPortBuffer = "";
    openedBracket = 0;
    jsonStarted = false; // Reset message reception
  }
  //Serial.print("Returned value:"); Serial.println(result);
  return result;
}

//
// Process json message received from Serial
//
void SerialPortMsgProcessing(String jsonMsg)
{
  char msg[64];
  //Serial.println("SerialPortMsgProcessing");
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonMsg);

  if (!error) // Vérifier que la désérialisation a réussi
  {
    //char *cmdReceived = doc["cmd"];
    //char *temp = doc["H"];
    //CommandSerialNumber = temp; // Obtient le numéro de séquence de la nouvelle commande
    if(doc["cmd"] == "")
    {
      if(doc["d1"] == "time") {
        /*strcpy(currentDateTime, doc["d2"]);
        Serial.println(currentDateTime); // Debug logging
        hh = atoi(&currentDateTime[11]);
        mm = atoi(&currentDateTime[14]);
        ss = atoi(&currentDateTime[17]);
        //Serial.println(hh);
        //Serial.println(mm);
        //Serial.println(ss);
        */
      }
      if(doc["d1"] == "ultrason") {
        if(doc["d2"] == "true") {
          Serial.println("Ultrasonic_enabled = true;");
          //Ultrasonic_enabled = true;
        } else {
          Serial.println("Ultrasonic_enabled = false;");
          //Ultrasonic_enabled = false;
        }
      }
    }
    else if(doc["cmd"] == "get")
    {
      if(doc["d1"] == "battery") {
        Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 150 "/home/equipe2/BrasRobot/Arduino/Arduino.ino" 3
                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 150 "/home/equipe2/BrasRobot/Arduino/Arduino.ino"
                      "$Get battery received"
# 150 "/home/equipe2/BrasRobot/Arduino/Arduino.ino" 3
                      ); &__c[0];}))
# 150 "/home/equipe2/BrasRobot/Arduino/Arduino.ino"
                      )));
        //SensorDataUpdate(battery, true);
      } else if(doc["d1"] == "ultrason") {
        Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 153 "/home/equipe2/BrasRobot/Arduino/Arduino.ino" 3
                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 153 "/home/equipe2/BrasRobot/Arduino/Arduino.ino"
                      "$Get ultrason received"
# 153 "/home/equipe2/BrasRobot/Arduino/Arduino.ino" 3
                      ); &__c[0];}))
# 153 "/home/equipe2/BrasRobot/Arduino/Arduino.ino"
                      )));
        //SensorDataUpdate(ultrason, true);
      } else if(doc["d1"] == "status") {
        Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 156 "/home/equipe2/BrasRobot/Arduino/Arduino.ino" 3
                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 156 "/home/equipe2/BrasRobot/Arduino/Arduino.ino"
                      "$Get status received"
# 156 "/home/equipe2/BrasRobot/Arduino/Arduino.ino" 3
                      ); &__c[0];}))
# 156 "/home/equipe2/BrasRobot/Arduino/Arduino.ino"
                      )));
        //SensorDataUpdate(ultrason, true);
        // gcadet to finish
      }
    }
    else if(doc["cmd"] == "s1") // Servo moteur cmd
    {
      //tankStatus.Functional_Mode = CMD_ServoControl; 
      //CMD_ServoSelection = doc["d1"];
      //CMD_Servo_angle = doc["d2"];
      Serial.println("$ok");
    }
    else if(doc["cmd"] == "s2")
    {
      uint8_t temp_Set_Servo = doc["d1"];
      Serial.println(temp_Set_Servo);
      Serial.println("$ok");
      //ServoStepMove(temp_Set_Servo);
    }
    else if(doc["cmd"] == "m1")
    {
      //tankStatus.Functional_Mode = CMD_MotorControl; 
      //CMD_MotorSelection = doc["d1"];
      //CMD_MotorSpeed = doc["d2"];
      //CMD_MotorDirection = doc["d3"];
      Serial.println("$ok");
    }
    else if(doc["cmd"] == "led")
    {
      //CMD_ledMode = doc["d1"];
      /*
      switch (CMD_ledMode) {
        case 1:
          //DriverLedRGB.Set(CRGB::Green, LED_BLINK, CRGB::Black, 500, 500);
          break;
        case 2:
          //DriverLedRGB.Set(CRGB::Red, LED_BLINK, CRGB::Black, 500, 500);
          break;
        case 3:
          //DriverLedRGB.Set(CRGB::Red, LED_BLINK, CRGB::Black, 100, 900);
          break;
        case 4:
          //DriverLedRGB.Set(CRGB::Violet, LED_BLINK, CRGB::Yellow, 1000, 1000);
          break;   
        case 5:
          //DriverLedRGB.Set(CRGB::Orange, LED_BRIGHT, CRGB::Orange, 1000, 1000);
          break;    
        case 6:
          //DriverLedRGB.Set(CRGB::Brown, LED_FIX, CRGB::Orange, 1000, 1000);
          break;
        default:
          //DriverLedRGB.Set(CRGB::Black, LED_FIX, CRGB::Black, 500, 0); // off & no blink
          break;
          
      }*/
      Serial.println("$ok");
    }
    else
    {
      const char *cmd = "xxxxxxxxxx";
      cmd = doc["cmd"];
      Serial.print("$Unknown cmd: ");Serial.println(cmd);
    }
  }
  else {
    Serial.println("$Error: Json deserialize");
  }
}

void Display_LimitSwitch()
{
  char buf[64];
  sprintf(buf, "{\"msg\":\"limit socle: %d\"}", digitalRead(11));
  Serial.println(buf);
  sprintf(buf, "{\"msg\":\"limit hauteur: %d\"}", digitalRead(10));
  Serial.println(buf);
  sprintf(buf, "{\"msg\":\"limit coude: %d\"}", digitalRead(9));
  Serial.println(buf);
  sprintf(buf, "{\"msg\":\"limit rot pince: %d\"}", digitalRead(A3));
  Serial.println(buf);

}

void setup() {

  Serial.println("{\"msg\" : \"============================================================================\"}");
  Serial.println("{\"msg\" : \"Programme de contrôle du Bras Robot selon une commande reçue en wifi\"}");
  Serial.println("{\"msg\" : \"Date de création : 25/03/2023\"}");
  Serial.println("{\"msg\" : \"Date de modification : 25/03/2023\"}");
  Serial.println("{\"msg\" : \"Modification effectuée : \"}");
  Serial.println("{\"msg\" : \"Auteur : Groupe\"}");
  Serial.println("{\"msg\" : \"============================================================================\"}");
    Serial.begin(9600);
    while (Serial.read() >= 0)
    {
        /*Vider le cache du port série...*/
    }
  Init(); // Init all sensors 
/*
  #if defined(DEBUG)
      Serial.println();
      Serial.print(F()"Application name: ")); Serial.println(applicationName); 
      Serial.print(F("  v")); Serial.println(appVersion);
      Serial.print(F("  file name: ")); Serial.println(filename);
      Serial.println(F("Starting in DEBUG mode.")); 
  #endif
  */
  Serial.println("");
  Display_LimitSwitch();
  socle.setSpeed(500);
  coude.setSpeed(500);
  rot_pince.setSpeed(500);
  hauteur.setSpeed(1500);




  Serial.println("init socle");
  while (digitalRead(11) !=1){
   socle.step(-200);
  }
  Display_LimitSwitch();

  Serial.println("init coude");
  while (digitalRead(9) !=1){
   coude.step(-200);
  }
  Display_LimitSwitch();

  Serial.println("init pince");
  while (digitalRead(A3) !=1){
   rot_pince.step(-200);
  }
  Display_LimitSwitch();

  Serial.println("init hauteur");
  while (digitalRead(10) !=1){
   hauteur.step(+200);
  }
  Display_LimitSwitch();
 }

void loop() {
/*  
  // step one revolution  in one direction:
  Serial.println("sens des aiguilles d'une montre");
  coude.step(Tour_10);
  delay(1000);

  // step one revolution in the other direction:
  Serial.println("sens inverse des aiguilles d'une montre");
  coude.step(-Tour_10);
  delay(1000);
  */
    SerialMsgReceived = SerialPortJsonReception(); // Read incoming command from serial port

        Serial.print("to be processed:"); Serial.println(SerialMsgReceived);


    if (SerialMsgReceived.length() > 0) {
        SerialPortMsgProcessing(SerialMsgReceived);
    }
}
