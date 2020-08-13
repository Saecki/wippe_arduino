//#include <ArduinoJson.h>
#include <Servo.h>

#define analogMax 1023.0

#define meanServoValues 3
#define servoMin 20
#define servoMax 160

#define joystickSwitch 6
#define joystickX 4
#define joystickY 5

Servo servoX;
Servo servoY;

long calibratingTime = 0;
bool calibrating = false;

float calX = 0;
float calY = 0;
float x;
float y;
float lastX[meanServoValues];
float lastY[meanServoValues];

void setup() {
    Serial.begin(9600);

    servoX.attach(10);
    servoY.attach(11);

    pinMode(joystickSwitch, INPUT);

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
    int switchValue = analogRead(joystickSwitch);
    if (switchValue < 200) {
        long time = millis();

        if (time - calibratingTime > 1000) {
            calibrating = !calibrating;
            calibratingTime = time;
        }
    }

    if (calibrating) {
        calX += (analogRead(joystickX) / analogMax - 0.5) / 100.0;
        calY += (analogRead(joystickY) / analogMax - 0.5) / 100.0;
        x = 0.5;
        y = 0.5;
    } else {
        x = analogRead(joystickX) / analogMax;
        y = analogRead(joystickY) / analogMax;
    }

    float dx = x;
    float dy = x;

    for (int i = 0; i < meanServoValues; i++) {
        dx += lastX[i];
        dy += lastY[i];
    }

    dx /= meanServoValues + 1;
    dy /= meanServoValues + 1;
    
    servoX.write((dx + calX) * (servoMax - servoMin) + servoMin);
    servoY.write((dy + calY) * (servoMax - servoMin) + servoMin);

    for (int i = 0; i < meanServoValues - 1; i++) {
        lastX[i] = lastX[i + 1];
        lastY[i] = lastY[i + 1];
    }
    lastX[meanServoValues - 1] = x;
    lastY[meanServoValues - 1] = y;

    Serial.print("x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.print(" switchValue: ");
    Serial.print(switchValue);
    Serial.print(" calibrating: ");
    Serial.println(calibrating);
}
