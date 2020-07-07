#include <AccelStepper.h>

// All the motor pins
#define m1 8
#define m2 9
#define m3 10
#define m4 11 
#define STEPS_PER_TURN 2048

int knob = 0;
int motorSpeed = 500;
int motorAccel = 400;
int old = 0;
// True = left
// false = right
bool direction = true;

AccelStepper stepper(4, m1, m3, m2, m4);

void setup() {
    Serial.begin(9600);
    stepper.setMinPulseWidth(20);
    stepper.setMaxSpeed(motorSpeed);
    stepper.setSpeed(motorSpeed);
    stepper.setAcceleration(motorAccel);
    stepper.setCurrentPosition(stepper.currentPosition());
    stepper.runToPosition();
    Serial.println("Ready to rotate");
}

void loop() {
    int read = analogRead(knob);
    Serial.println(read);

    // right turn
    if (read < 500 && old >= 500){
        stepper.stop();
        stepper.runToPosition();
        stepper.setCurrentPosition(stepper.currentPosition());
        stepper.setSpeed(motorSpeed);
        stepper.moveTo(-10000);
        Serial.println("Right");
    // left turn otherwise
    } else if (read >= 500 && old < 500){
        stepper.stop();
        stepper.runToPosition();
        stepper.setCurrentPosition(stepper.currentPosition());
        stepper.runToPosition();
        stepper.setSpeed(motorSpeed);
        stepper.moveTo(10000);
        Serial.println("Left");
    }
    stepper.run();
    old = read;
}
