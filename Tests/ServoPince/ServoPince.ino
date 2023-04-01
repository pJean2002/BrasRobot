/*
    Test Servomoteur de la pince
    Auteur: Pierre
*/

#include <Servo.h>

Servo Pince

void setup()
{
    Pince.attach(A0);
}

void loop()
{
    Pince.write(10);
    delay(2000)
    Pince.write(170);
    delay(2000);
    Pince.write(90);
    Pince.detach();
}