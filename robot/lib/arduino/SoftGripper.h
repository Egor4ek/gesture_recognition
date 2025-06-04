#ifndef ARDUINO_SOFTGRIPPER_H
#define ARDUINO_SOFTGRIPPER_H
#include <Arduino.h>


class SoftGripper {
public:
    uint8_t out1;
    uint8_t out2;

    void setup(int pin1, int pin2);
    void stop();
    void grip(unsigned long delayTime = 0);
    void ungrip(unsigned long delayTime = 0);
};

void grip(int id, unsigned long =0);
void ungrip(int id, unsigned long =0);
void setup_gripper(int id,int pin1,int pin2);
void stop_gripper(int id);
#endif //ARDUINO_SOFTGRIPPER_H

