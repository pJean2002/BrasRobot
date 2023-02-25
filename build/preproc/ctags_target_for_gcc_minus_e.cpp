# 1 "/home/equipe2/BrasRobot/Stepper.ino"
# 2 "/home/equipe2/BrasRobot/Stepper.ino" 2






Stepper PasPas = Stepper(200,4,7);

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
