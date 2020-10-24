#include <Servo.h>

#define servoMin 20
#define servoMax 160

#define PF 0
#define IF 0
#define DF 180

Servo servoX;
Servo servoY;

float posX = 0.5;
float posY = 0.5;

void setup() {
    Serial.begin(9600);

    servoX.attach(11);
    servoY.attach(10);

    Serial.println("initalizing");

    bool on = false;
    for (float i = -2; i <= 8.28; i += 0.06) {
        servoX.write(servoValue((sin(i) + 1.5) / 4));
        servoY.write(servoValue((cos(i) + 1) / 2));
        delay(5);
    }
}

void loop() {
    if (Serial.available() > 0) {
        String strX = Serial.readStringUntil(',');
        String strY = Serial.readStringUntil('\n');

        if (strX.length() == 0) posX = 0.5;
        else posX = strX.toFloat();

        if (strY.length() == 0) posY = 0.5;
        else posY = -strY.toFloat() + 1;
    }

    servoX.write(servoValue(posX));
    servoY.write(servoValue(posY));
}

int servoValue(float val) {
    return val * (servoMax - servoMin) + servoMin;
}
