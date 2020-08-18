#include <Servo.h>

#define servoMin 20
#define servoMax 160

#define PF 0.3
#define IF 0.01
#define DF 120

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
    for (int i = servoMin + 30; i <= servoMax - 30; i++) {
        servoX.write(i);
        servoY.write(i);
        delay(5);
    }
    for (int i = servoMax - 30; i >= servoMin + 30; i--) {
        servoX.write(i);
        servoY.write(i);
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

        dx = (diffX - lastDiffX) * DF / timeDiff;
        dy = (diffY - lastDiffY) * DF / timeDiff;
         
        posX = 0.5 + px + ix + dx;
        posY = 0.5 + py + iy + dy;

        lastDiffX = diffX;
        lastDiffY = diffY;
    
        lastUpdateTime = millis();
    }

    servoX.write((posX) * (servoMax - servoMin) + servoMin);
    servoY.write((posY) * (servoMax - servoMin) + servoMin);

    Serial.print("bx:");
    Serial.print(ballX);
    Serial.print("by:");
    Serial.print(ballY);
    Serial.println();
}
