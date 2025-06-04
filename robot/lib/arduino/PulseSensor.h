

#ifndef PulseSensor_h
#define PulseSensor_h

#include "Arduino.h"


void setupPulseSensor(int port);// initializing interrupt  routine 

int pulseBPM(void); //returns pulse in beats per minute
bool pulseBeat(void);// returns 1 when there is a heart beat detected
int pulseWave(void); //returns raw wave after some filtration and amplification


#endif