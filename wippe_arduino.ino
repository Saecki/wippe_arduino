#include <Servo.h>

#define analogMax 1023.0

#define servoMin 20
#define servoMax 160

#define joystickX 4
#define joystickY 5
#define joystickSwitch 6

#define PF 0.8
#define IF 0.05
#define DF 0.3

Servo servoX;
Servo servoY;

long lastModeSwitchTime = 0;
bool calibrating = false;

long currentTime = 0;
long lastUpdateTime = 0;
float calX = 0;
float calY = 0;
float ballX = 0.5;
float ballY = 0.5;
float posX = 0.5;
float posY = 0.5;
float lastPosX = 0.5;
float lastPosY = 0.5;

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

    pinMode(joystickSwitch, INPUT_PULLUP);

    Serial.println("initalizing");

    bool on = false;
    for (int i = 30; i <= 150; i++) {
        servoX.write(i);
        servoY.write(i);
        delay(10);
        
        if (i % 10 == 0) {
            on = !on;
            digitalWrite(LED_BUILTIN, on);
        }
    }
    for (int i = 150; i >= 30; i--) {
        servoX.write(i);
        servoY.write(i);
        delay(10);
        
        if (i % 10 == 0) {
            on = !on;
            digitalWrite(LED_BUILTIN, on);
        }
    }
}

void loop() {
    if (Serial.available() > 0) {
        String strX = Serial.readStringUntil(',');
        String strY = Serial.readStringUntil(';');
        
        while(Serial.available() > 0) {
            char t = Serial.read();
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

        dx = (lastPosX - diffX) * DF / timeDiff;
        dy = (lastPosY - diffY) * DF / timeDiff;
         
        posX = 0.5 + px + ix + dx;
        posY = 0.5 + py + iy + dy;

        lastPosX = posX;
        lastPosY = posY;
    
        lastUpdateTime = millis();
    }


    if (analogRead(joystickSwitch) < 100) {
        long time = millis();

        if (time - lastModeSwitchTime > 1000) {
            calibrating = !calibrating;
            lastModeSwitchTime = time;
            digitalWrite(LED_BUILTIN, calibrating);
        }
    }

    if (calibrating) {
        calX += (analogRead(joystickX) / analogMax - 0.5) / 100.0;
        calY += (analogRead(joystickY) / analogMax - 0.5) / 100.0;
        posX = 0.5;
        posY = 0.5;
    }

    servoX.write((posX + calX) * (servoMax - servoMin) + servoMin);
    servoY.write((posY + calY) * (servoMax - servoMin) + servoMin);

    Serial.print("ballX: ");
    Serial.print(ballX);
    Serial.print(", ballY: ");
    Serial.print(ballY);
    Serial.print(", posX: ");
    Serial.print(posX);
    Serial.print(", posY: ");
    Serial.print(posY);
    Serial.print(", px: ");
    Serial.print(px);
    Serial.print(", py: ");
    Serial.print(py);
    Serial.print(", ix: ");
    Serial.print(ix);
    Serial.print(", iy: ");
    Serial.print(iy);
    Serial.print(", dx: ");
    Serial.print(dx);
    Serial.print(", dy: ");
    Serial.print(dy);
    Serial.print(", calibrating: ");
    Serial.println(calibrating);
}
