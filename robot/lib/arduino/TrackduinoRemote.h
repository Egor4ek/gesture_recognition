

#ifndef TrackduinoRemote_h
#define TrackduinoRemote_h

#include "Arduino.h"
//#include <ArduinoJson.h>

// #define UART1 1
// #define UART2 2
// #define UART3 3
#define ATTENTION 1
#define MEDITATION 2
#define BLINK 3

#define IR_SENSOR_ANALOG 0
#define IR_SENSOR_WHITE 1
#define IR_SENSOR_BLACK 2
#define COLOR_SENSOR 3
#define DISTANCE_SENSOR 4
#define LIGHT_SENSOR_ANALOG 5
#define LIGHT_SENSOR_LIGHT 6
#define LIGHT_SENSOR_DARK 7
#define BUTTON_SENSOR 8
#define TEMPERATURE_SENSOR 9
#define MICROPHONE_SENSOR_ANALOG 10
#define MICROPHONE_SENSOR_DIGITAL 11
#define GYROACCEL_SENSOR 12
#define ENCODER_SENSOR 13
#define TILT_SENSOR 14
#define FLAME_SENSOR 15
#define MAGNETIC_SENSOR 16
#define BEND_SENSOR 17
#define PULSE_SENSOR_BPM 18
#define SKIN_SENSOR 19
#define IRRC_SENSOR 20
#define METAL_SENSOR 21
#define BUILT_IN 22//buttons
#define PULSE_SENSOR_BEAT 23
#define PULSE_SENSOR_WAVE 24


#define UNKNOWN_ANALOG_SENSOR 99

int requestNeuroData(int data);
int parseNeuroData(int requestedNeuroData);

int requestRCData(int sensorType);
int requestRCData(int sensorType, int port);

int receiveRCData(void);

class remoteSensor
{
public:
	//remoteSensor(int sensorType, int port);
	remoteSensor(int sensorType, int port);
	remoteSensor(int sensorType);
	void send(int data);
	void send(bool data);
	void send(void);
	void send(float data);
	void send(double data);
	void send(long data);
	
	
	// void line(int x1, int y1, int x2, int y2, unsigned int color);
 //    void circle(int x, int y, int r, unsigned int color);
 //    void circleFilled(int x, int y, int r, unsigned int color);
	// void rectangle(int x1, int y1, int x2, int y2,unsigned int color);
	// void fill(int x1, int y1, int x2, int y2, unsigned int color);
	
	// void text(int x, int y, int font, unsigned int color, String text);
	// void text(int x, int y, int font, unsigned int color, int text);
	// void text(int x, int y, int font, unsigned int color, float text);
	// void text(int x, int y, int font, unsigned int color, double text);
	// void text(int x, int y, int font, unsigned int color, char text);
	
	// // void test(int x, int y, int font, unsigned int color, String text);

	// void print(void);
	// void print(String text);
	// void print(int text);
	// void print(float text);
	// void print(double text);
	// void print(char text);
	
	// void println(void);
	// void println(String text);
	// void println(int text);
	// void println(float text);
	// void println(double text);
	// void println(char text);
	
	// void printlnlinesholder(String text);
	
	// void brightness(int brightness);
	// void enableDrawing();
	// void disableDrawing();
	// void drawingColor(int color);
	// unsigned int convertColorToNextionSystem(unsigned int color);
	
private:
	int port;
	int sensorType;
	int test;
	int data;
};

// void enddisplaycommand ();

#endif