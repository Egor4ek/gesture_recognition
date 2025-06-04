#include "SoftGripper.h"


SoftGripper gripper1;
SoftGripper gripper2;
SoftGripper gripper3;
SoftGripper gripper4;


void SoftGripper::stop() {
    digitalWrite(out1, LOW);
    digitalWrite(out2, LOW);
}


void SoftGripper::setup(int pin1, int pin2) {
    out1 = pin1;
    out2 = pin2;
    pinMode(out1, OUTPUT);
    pinMode(out2, OUTPUT);
}


// Метод захвата
void SoftGripper::grip(unsigned long delayTime) {
    digitalWrite(out1, HIGH);
    digitalWrite(out2, LOW);
    if (delayTime > 0) {
        delay(delayTime);
        stop();
    }

}

// Метод разжима
void SoftGripper::ungrip(unsigned long delayTime) {
    digitalWrite(out1, LOW);
    digitalWrite(out2, HIGH);
    if (delayTime > 0) {
        delay(delayTime);
        stop();
    }
}


void setup_gripper(int id, int pin1, int pin2) {
    switch (id) {
        case 1:
            gripper1.setup(pin1, pin2);
            break;
        case 2:
            gripper2.setup(pin1, pin2);
            break;
        case 3:
            gripper3.setup(pin1, pin2);
            break;
        case 4:
            gripper4.setup(pin1, pin2);
            break;
    }
}

void grip(int id, unsigned long delayTime) {
    switch (id) {
        case 1:
            gripper1.grip(delayTime);
            break;
        case 2:
            gripper2.grip(delayTime);
            break;
        case 3:
            gripper3.grip(delayTime);
            break;
        case 4:
            gripper4.grip(delayTime);
            break;
    }
}


void ungrip(int id, unsigned long delayTime) {
    switch (id) {
        case 1:
            gripper1.ungrip(delayTime);
            break;
        case 2:
            gripper2.ungrip(delayTime);
            break;
        case 3:
            gripper3.ungrip(delayTime);
            break;
        case 4:
            gripper4.ungrip(delayTime);
            break;
    }
}


void stop_gripper(int id) {
    switch (id) {
        case 1:
            gripper1.stop();
            break;
        case 2:
            gripper2.stop();
            break;
        case 3:
            gripper3.stop();
            break;
        case 4:
            gripper4.stop();
            break;
    }
}


