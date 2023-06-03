
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

#include <Stepper.h>
#include <ArduinoJson.h>

// Version
const char* applicationName = "iot_library";
const char* filename = "iot_device.ino";
const char* appVersion = "1.00";

static String SerialMsgReceived = "";

#define DEBUG

#define limit_socle 11
#define limit_hauteur 10
#define limit_coude 9
#define limit_pince A3
const int Tour_10 = 32000;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper socle(200, 2, 5);
Stepper coude(200, 3, 6);
Stepper rot_pince(200, 4, 7);
Stepper hauteur(200, 12, 13);

void Init(void)
{
  pinMode(limit_socle, INPUT_PULLUP); 
  pinMode(limit_hauteur, INPUT_PULLUP);  
  pinMode(limit_coude, INPUT_PULLUP); 
  pinMode(limit_pince, INPUT_PULLUP); 
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
static  bool jsonStarted = false;

String SerialPortJsonReception()
{
  char c;
  String result = "";
  if (Serial.available() > 0)
  {
    do {
      c = Serial.read();
      Serial.print(c);    // echo
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

  if (jsonStarted && (openedBracket == 0))  // End of message received
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
    if(doc["cmd"] == "get")
    {
      if(doc["d1"] == "angle") 
      {
        Serial.println("{\"log\" : \" Commande de demande d'angle des moteurs recue\"}");
        //Serial.println("{\"msg\" : \" Angle du moteur du socle" + socle.read() + "\"}");
        Serial.println("{\"log\" : \" Angle des Moteurs lu et envoyés\"}");
        // gcadet to finish
      }
    }
    else if(doc["cmd"] == "s1")     // Servo moteur cmd
    {
      //tankStatus.Functional_Mode = CMD_ServoControl; 
      //CMD_ServoSelection = doc["d1"];
      //CMD_Servo_angle = doc["d2"];
      Serial.println("{\"log\" : \" Commande de pince recue\"}");
    }
    
    else if(doc["cmd"] == "m1")
    {
      Serial.println("{\"log\" : \" Commande de moteur de coude recue\"}");
      if(doc["ordre"]=="move")
      {
        Serial.println("{\"log\" : \" Commande de mouvement recue\"}");
        int NbrPas=doc["val"];
        int Sens=doc["sens"];
        socle.step(NbrPas*Sens);
      }
    }
    else if(doc["cmd"] == "m2")
    {
      Serial.println("{\"log\" : \" Commande de moteur de poignet recue\"}");
      if(doc["ordre"]=="move")
      {
        Serial.println("{\"log\" : \" Commande de mouvement recue\"}");
        //coude.step(doc["val"]*doc["sens"]);
      }
    }
    else if(doc["cmd"] == "m3")
    {
      Serial.println("{\"log\" : \" Commande de moteur d'axe Z recue\"}");
      if(doc["ordre"]=="move")
      {
        Serial.println("{\"log\" : \" Commande de mouvement recue\"}");
        //coude.step(doc["val"]*doc["sens"]);
      }
    }
    else if(doc["cmd"] == "m4")
    {
      Serial.println("{\"log\" : \" Commande de moteur d'epaule recue\"}");
      if(doc["ordre"]=="move")
      {
        Serial.println("{\"log\" : \" Commande de mouvement recue\"}");
        //coude.step(doc["val"]*doc["sens"]);
      }
    }  
    else if(doc["cmd"] == "led")
    {
      //CMD_ledMode = doc["d1"];
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
  sprintf(buf, "{\"msg\":\"limit socle: %d\"}", digitalRead(limit_socle));
  Serial.println(buf);
  sprintf(buf, "{\"msg\":\"limit hauteur: %d\"}", digitalRead(limit_hauteur));
  Serial.println(buf);
  sprintf(buf, "{\"msg\":\"limit coude: %d\"}", digitalRead(limit_coude));
  Serial.println(buf);
  sprintf(buf, "{\"msg\":\"limit rot pince: %d\"}", digitalRead(limit_pince));
  Serial.println(buf);
  
}

void setup() {
  Serial.begin(9600);
  Serial.println("{\"msg\" : \"============================================================================\"}");
  Serial.println("{\"msg\" : \"Programme de controle du Bras Robot selon une commande recue en wifi\"}");
  Serial.println("{\"msg\" : \"Date de creation : 25/03/2023\"}");
  Serial.println("{\"msg\" : \"Date de modification : 13/05/2023\"}");
  Serial.println("{\"msg\" : \"Modification effectuee : \"}");
  Serial.println("{\"msg\" : \"Auteur : Groupe\"}");  
  Serial.println("{\"msg\" : \"============================================================================\"}");
    
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
  while (digitalRead(limit_socle) !=1){
   socle.step(-200);
  } 
  Display_LimitSwitch(); 

  Serial.println("init coude");
  while (digitalRead(limit_coude) !=1){
   coude.step(-200);
  }   
  Display_LimitSwitch();

  Serial.println("init pince");
  while (digitalRead(limit_pince) !=1){
   rot_pince.step(-200);
  }  
  Display_LimitSwitch(); 
     
  Serial.println("init hauteur");
  while (digitalRead(limit_hauteur) !=1){
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
    SerialMsgReceived = SerialPortJsonReception();  // Read incoming command from serial port
    if (SerialMsgReceived.length() > 0) { 
      #if defined(DEBUG)
        Serial.print("to be processed:"); Serial.println(SerialMsgReceived);
      #endif
        SerialPortMsgProcessing(SerialMsgReceived);
    }
}
