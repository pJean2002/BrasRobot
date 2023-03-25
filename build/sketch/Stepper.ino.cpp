#include <Arduino.h>
#line 1 "/home/equipe2/BrasRobot/Stepper.ino"
#include <Stepper.h>

#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3

Stepper PasPas = Stepper(200,4,7);

#line 10 "/home/equipe2/BrasRobot/Stepper.ino"
void setup();
#line 16 "/home/equipe2/BrasRobot/Stepper.ino"
void loop();
#line 10 "/home/equipe2/BrasRobot/Stepper.ino"
void setup()
{
    Serial.begin(9600);
    PasPas.setSpeed(200);
}

void loop()
{
	PasPas.step(0);
    delay(1000);
    PasPas.step(0);
    delay(1000);
}

