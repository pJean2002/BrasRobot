/*
    Test Servomoteur de la pince
    Auteur: Pierre
*/

#include <Servo.h>

Servo Pince;

void setup()
{
    Serial.begin(115200);
    Pince.attach(A0);
}

void loop()
{
    Pince.write(0);
    Serial.println("Sens1");
    delay(5000);
    Pince.write(180);
    Serial.println("Sens2");
    delay(5000);
}