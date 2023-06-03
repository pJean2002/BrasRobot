#include <Stepper.h>

#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3

Stepper PasPas = Stepper(200,3,6);

void setup()
{
    Serial.begin(9600);
    PasPas.setSpeed(200);
}

void loop()
{
	PasPas.step(200);
    delay(1000);
    PasPas.step(-200);
    delay(1000);
}
