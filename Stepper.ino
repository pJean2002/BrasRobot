#include <Stepper.h>


#define limitSwitch1 11
#define limitSwitch2 10
#define limitSwitch3 9
#define limitSwitch4 A3
Stepper PasPas = Stepper(200,12,13);

void setup()
{
    Serial.begin(9600);
    pinMode(limitSwitch1, INPUT_PULLUP);
    pinMode(limitSwitch2, INPUT_PULLUP);
    pinMode(limitSwitch3, INPUT_PULLUP);
    pinMode(limitSwitch4, INPUT_PULLUP);
}

void loop()
{
	
}
