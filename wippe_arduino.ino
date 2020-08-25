#include <Servo.h>

#define servoMin 20
#define servoMax 160

#define PF 0.7
#define IF 0
#define DF 180

Servo servoX;
Servo servoY;

long currentTime = 0;
long lastUpdateTime = 0;
float calX = 0;
float calY = 0;
float ballX = 0.5;
float ballY = 0.5;
float posX = 0.5;
float posY = 0.5;
float lastDiffX = 0;
float lastDiffY = 0;

float px = 0;
float py = 0;
float dx = 0;
float dy = 0;
float ix = 0;
float iy = 0;

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

        while (Serial.available()) {
            Serial.read();
        }

        if (strX.length() == 0) ballX = 0.5;
        else ballX = strX.toFloat();

        if (strY.length() == 0) ballY = 0.5;
        else ballY = -strY.toFloat() + 1;

        currentTime = millis();
    
        float diffX = (0.5 - ballX);
        float diffY = (0.5 - ballY);
        float timeDiff = currentTime - lastUpdateTime;
        
        px = diffX * PF;
        py = diffY * PF;

        ix += diffX * IF / timeDiff;
        iy += diffY * IF / timeDiff;

        dx = sq(diffX - lastDiffX) * DF / timeDiff;
        dy = sq(diffY - lastDiffY) * DF / timeDiff;
         
        posX = 0.5 + px + ix + dx;
        posY = 0.5 + py + iy + dy;

        lastDiffX = diffX;
        lastDiffY = diffY;

        lastUpdateTime = millis();
    }

    servoX.write(servoValue(posX));
    servoY.write(servoValue(posY));

    Serial.print("bx:");
    Serial.print(ballX);
    Serial.print("by:");
    Serial.print(ballY);
    Serial.println();
}

int servoValue(float val) {
    return val * (servoMax - servoMin) + servoMin;
}
