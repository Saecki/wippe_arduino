//#include <ArduinoJson.h>
#include <Servo.h>

#define joystickSwitch 9
#define joystickX 4
#define joystickY 5
#define lastLength 3

Servo servoX;
Servo servoY;

bool calibrating = false;

float calX = 0;
float calY = 0;
float x;
float y;
float lastX[lastLength];
float lastY[lastLength];

void setup() {
    Serial.begin(9600);

    servoX.attach(10);
    servoY.attach(11);

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

    if (calibrating) { calX += (float) (90 - analogRead(joystickX)) / 1024.0 / 1000.0;
        calY += (float) (90 - analogRead(joystickY)) / 1024.0 / 1000.0;
        x = 0;
        y = 0;
    } else {
        x = ((float) analogRead(joystickX)) / 1024;
        y = ((float) analogRead(joystickY)) / 1024;
    }

    float dx = x;
    float dy = x;

    for (int i = 0; i < lastLength; i++) {
        dx += lastX[i];
        dy += lastY[i];
    }

    dx /= lastLength + 1;
    dy /= lastLength + 1;
    
    servoX.write((dx + calX) * 180);
    servoY.write((dy + calY) * 180);

    for (int i = 0; i < lastLength - 1; i++) {
        lastX[i] = lastX[i + 1];
        lastY[i] = lastY[i + 1];
    }
    lastX[lastLength - 1] = x;
    lastY[lastLength - 1] = y;

    Serial.print("x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.print(" calibrating: ");
    Serial.println(calibrating);
}
