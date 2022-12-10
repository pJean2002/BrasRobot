
/*
   Arduino based SCARA Robot 
   by Dejan, www.HowToMechatronics.com
   AccelStepper: http://www.airspayce.com/mikem/arduino/AccelStepper/index.html

*/


#include <Stepper.h>
#include <Servo.h>

#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3


// initialize the stepper library on pins 8 through 11:
Stepper alex = Stepper(200, 12, 13);
uint8_t compteur=0;
void setup() {
   Serial.begin(9600);
  pinMode(limitSwitch1, INPUT_PULLUP);
  pinMode(limitSwitch2, INPUT_PULLUP);
  pinMode(limitSwitch3, INPUT_PULLUP);
  pinMode(limitSwitch4, INPUT_PULLUP);
  Serial.println("testino V1");
  alex.setSpeed(200);
}

void loop() {

  Serial.println(digitalRead(limitSwitch1));
if (compteur==0) {
  alex.step(50);
  alex.step(-50);
  compteur++;
}
delay(500);
}
