#include <Stepper.h>

#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3

Stepper Hauteur = Stepper(200,12,13);
Stepper Epaule = Stepper(200,2,5);
Stepper Coude = Stepper(200,3,6);
Stepper Poignet = Stepper(200,4,7);

void setup()
{
    Serial.begin(9600);
    Hauteur.setSpeed(2000);
    Epaule.setSpeed(2000);
    Coude.setSpeed(2000);
    Poignet.setSpeed(2000);

    homing();
}

void loop(){}

void homing()
{
    // Homing Stepper4
    while (digitalRead(limitSwitch4) != 1) 
    {
        stepper4.setSpeed(1500);
        stepper4.runSpeed();
        stepper4.setCurrentPosition(17000); // When limit switch pressed set position to 0 steps
    }
    delay(20);
    stepper4.moveTo(10000);
    while (stepper4.currentPosition() != 10000) 
    {
        stepper4.run();
    }

    // Homing Stepper3
    while (digitalRead(limitSwitch3) != 1) 
    {
        stepper3.setSpeed(-1100);
        stepper3.runSpeed();
        stepper3.setCurrentPosition(-1662); // When limit switch pressed set position to 0 steps
    }
    delay(20);

    stepper3.moveTo(0);
    while (stepper3.currentPosition() != 0) 
    {
        stepper3.run();
    }

    // Homing Stepper2
    while (digitalRead(limitSwitch2) != 1) 
    {
        stepper2.setSpeed(-1300);
        stepper2.runSpeed();
        stepper2.setCurrentPosition(-5420); // When limit switch pressed set position to -5440 steps
    }
    delay(20);

    stepper2.moveTo(0);
    while (stepper2.currentPosition() != 0) 
    {
        stepper2.run();
    }

    // Homing Stepper1
    while (digitalRead(limitSwitch1) != 1) 
    {
        stepper1.setSpeed(-1200);
        stepper1.runSpeed();
        stepper1.setCurrentPosition(-3955); // When limit switch pressed set position to 0 steps
    }
    delay(20);
    stepper1.moveTo(0);
    while (stepper1.currentPosition() != 0) 
    {
        stepper1.run();
    }
}