#include "Arduino.h"
#include "Display.h"

volatile int _uart;
HardwareSerial *_port;
volatile int cursorY, cursorX;
String linesholder[9];
int linepointer = 0;

void enddisplaycommand()
{
	switch (_uart)
	{

	case 1:
	{
		Serial1.write(255);
		Serial1.write(255);
		Serial1.write(255);
		break;
	}

	case 2:
	{
		Serial2.write(255);
		Serial2.write(255);
		Serial2.write(255);
		break;
	}

	case 3:
	{
		Serial3.write(255);
		Serial3.write(255);
		Serial3.write(255);
		break;
	}
	}
}

Display::Display(int uart)
{
	switch (uart)
	{

	case 1:
	{
		Serial1.begin(115200);
		_port = &Serial1;
		break;
	}

	case 2:
	{
		Serial2.begin(115200);
		_port = &Serial2;
		break;
	}

	case 3:
	{
		Serial3.begin(115200);
		_port = &Serial3;
		break;
	}

		cursorX = 0;
		cursorY = 0;
	}
	_uart = uart;
	_port->print("cls BLACK");
	enddisplaycommand();
	_port->print("cls BLACK");
	enddisplaycommand();
	_port->print("cls BLACK");
	enddisplaycommand();
}

void Display::clear()
{
	// _port->print("fill 0,0,320,240,BLACK");
	_port->print("cls BLACK");
	enddisplaycommand();
}

void Display::line(int x1, int y1, int x2, int y2, unsigned int color)
{
	color = Display::convertColorToNextionSystem(color);

	_port->print("line ");
	_port->print(x1);
	_port->print(",");
	_port->print(y1);
	_port->print(",");
	_port->print(x2);
	_port->print(",");
	_port->print(y2);
	_port->print(",");
	_port->print(color);
	enddisplaycommand();
}

void Display::circle(int x, int y, int r, unsigned int color)
{
	color = Display::convertColorToNextionSystem(color);

	_port->print("cir ");
	_port->print(x);
	_port->print(",");
	_port->print(y);
	_port->print(",");
	_port->print(r);
	_port->print(",");
	_port->print(color);
	enddisplaycommand();
}

void Display::circleFilled(int x, int y, int r, unsigned int color)
{
	color = Display::convertColorToNextionSystem(color);

	_port->print("cirs ");
	_port->print(x);
	_port->print(",");
	_port->print(y);
	_port->print(",");
	_port->print(r);
	_port->print(",");
	_port->print(color);
	enddisplaycommand();
}

void Display::rectangle(int x1, int y1, int x2, int y2, unsigned int color)
{
	color = Display::convertColorToNextionSystem(color);

	_port->print("draw ");
	_port->print(x1);
	_port->print(",");
	_port->print(y1);
	_port->print(",");
	_port->print(x2);
	_port->print(",");
	_port->print(y2);
	_port->print(",");
	_port->print(color);
	enddisplaycommand();
}

void Display::fill(int x1, int y1, int x2, int y2, unsigned int color)
{
	color = Display::convertColorToNextionSystem(color);

	_port->print("fill ");
	_port->print(x1);
	_port->print(",");
	_port->print(y1);
	_port->print(",");
	_port->print(x2 - x1);
	_port->print(",");
	_port->print(y2 - y1);
	_port->print(",");
	_port->print(color);
	enddisplaycommand();
}

void Display::brightness(int brightness)
{

	_port->print("dim=");
	_port->print(brightness);

	enddisplaycommand();
}

void Display::enableDrawing()
{

	_port->print("thdra=1");
	enddisplaycommand();
	_port->print("thdra=1");
	enddisplaycommand();
	_port->print("thdra=1");
	enddisplaycommand();
}

void Display::disableDrawing()
{
	_port->print("thdra=0");
	enddisplaycommand();
	_port->print("thdra=0");
	enddisplaycommand();
	_port->print("thdra=0");
	enddisplaycommand();
}

void Display::drawingColor(int color)
{
	color = Display::convertColorToNextionSystem(color);
	_port->print("thc=");
	_port->print(color);

	enddisplaycommand();
}

// void Display::test(int x, int y, int font, unsigned int color, String text)
// {

//  //_port->print("xstr 0,0,320,430,0,RED,NULL,1,1,1,\"China\"");
// color = Display::convertColorToNextionSystem(color);

//  _port->print("xstr ");
//   _port->print(x);
//   _port->print(",");
//   _port->print(y);
//   _port->print(",320,240,");
//   _port->print(font);
// 	_port->print(",");
// 	_port->print(color);
// 	_port->print(",NULL,0,0,1,\"");
// 	_port->print(text);
// 	_port->print("\"");
//   enddisplaycommand();

// }

void Display::text(int x, int y, int font, unsigned int color, String text)
{
	//_port->print("xstr 0,0,320,430,0,RED,NULL,1,1,1,\"China\"");
	color = Display::convertColorToNextionSystem(color);

	_port->print("xstr ");
	_port->print(x);
	_port->print(",");
	_port->print(y);
	_port->print(",320,240,");
	_port->print(font);
	_port->print(",");
	_port->print(color);
	_port->print(",0,0,0,3,\"");
	_port->print(text);
	_port->print("\"");
	enddisplaycommand();
}

void Display::text(int x, int y, int font, unsigned int color, int text)
{
	String text2 = String(text);
	Display::text(x, y, font, color, text2);
}

void Display::text(int x, int y, int font, unsigned int color, float text)
{
	String text2 = String(text);
	Display::text(x, y, font, color, text2);
}

void Display::text(int x, int y, int font, unsigned int color, double text)
{
	String text2 = String(text);
	Display::text(x, y, font, color, text2);
}

void Display::text(int x, int y, int font, unsigned int color, char text)
{
	String text2 = String(text);
	Display::text(x, y, font, color, text2);
}

void Display::text(int x, int y, int font, unsigned int color, long text)
{
	String text2 = String(text);
	Display::text(x, y, font, color, text2);
}

void Display::text(int x, int y, int font, unsigned int color, unsigned long text)
{
	String text2 = String(text);
	Display::text(x, y, font, color, text2);
}

void Display::print(String text)
{

	if (linepointer > 8)
	{

		for (int i = 0; i < 8; i++)
		{
			linesholder[i] = linesholder[i + 1];
		}

		Display::clear();
		cursorY = 0;
		for (int i = 0; i < 8; i++)
		{
			Display::printlnlinesholder(linesholder[i]);
		}
		linepointer = 8;
		linesholder[linepointer] = "";
	}
	//_port->print("xstr 0,0,320,430,0,RED,NULL,1,1,1,\"China\"");
	linesholder[linepointer] += text;
	_port->print("xstr ");
	_port->print(cursorX);
	_port->print(",");
	_port->print(cursorY);
	_port->print(",320,26,1,WHITE,0,0,0,3,\"");
	// _port->print(text);
	_port->print(linesholder[linepointer]);
	_port->print("\"");
	enddisplaycommand();
	//int charnumber = text.length();
	//cursorX=cursorX+(charnumber*12);
}

void Display::print(void)
{

	//do nothing
}

void Display::print(int text)
{

	String text2 = String(text);
	Display::print(text2);
}

void Display::print(float text)
{

	String text2 = String(text);
	Display::print(text2);
}

void Display::print(double text)
{

	String text2 = String(text);
	Display::print(text2);
}

void Display::print(char text)
{

	String text2 = String(text);
	Display::print(text2);
}

void Display::print(long text) {
String text2 = String(text);
	Display::print(text2);
}

void Display::print(unsigned long text) {
String text2 = String(text);
	Display::print(text2);
}

void Display::println(void)
{

	Display::println("");
}

void Display::println(String text)
{
	//_port->print("xstr 0,0,320,240,");

	if (linepointer > 8)
	{

		for (int i = 0; i < 8; i++)
		{
			linesholder[i] = linesholder[i + 1];
		}

		Display::clear();
		cursorY = 0;
		for (int i = 0; i < 8; i++)
		{
			Display::printlnlinesholder(linesholder[i]);
		}
		linepointer = 8;
		linesholder[linepointer] = "";
	}
	linesholder[linepointer] += text;
	_port->print("xstr ");
	_port->print(cursorX);
	_port->print(",");
	_port->print(cursorY);
	_port->print(",320,26,1,WHITE,0,0,0,3,\"");
	_port->print(linesholder[linepointer]);
	_port->print("\"");
	enddisplaycommand();
	//int charnumber = text.length();
	// cursorX=cursorX+(charnumber*12);

	cursorX = 0;
	cursorY = cursorY + 26;

	//linesholder[linepointer] = text;
	// Serial.print(linepointer);
	// Serial.println(linesholder[linepointer]);
	linepointer++;
}

void Display::println(int text)
{

	String text2 = String(text);
	Display::println(text2);
}

void Display::println(float text)
{

	String text2 = String(text);
	Display::println(text2);
}

void Display::println(double text)
{

	String text2 = String(text);
	Display::println(text2);
}

void Display::println(char text)
{

	String text2 = String(text);
	Display::println(text2);
}

void Display::println(long text) {
	String text2 = String(text);
	Display::println(text2);
}

void Display::println(unsigned long text) {
	String text2 = String(text);
	Display::println(text2);
}

void Display::printlnlinesholder(String text)
{
	_port->print("xstr ");
	_port->print(cursorX);
	_port->print(",");
	_port->print(cursorY);
	_port->print(",320,26,1,WHITE,0,0,0,3,\"");
	_port->print(text);
	_port->print("\"");
	enddisplaycommand();
	cursorY = cursorY + 26;
	//linepointer++;
}

unsigned int Display::convertColorToNextionSystem(unsigned int color)
{
	switch (color)
	{
	case 0: //black
	{
		return 0;
		break;
	}

	case 1: //white
	{
		return 65535;
		break;
	}

	case 2: //red
	{
		return 63488;
		break;
	}

	case 3: //green
	{
		return 2016;
		break;
	}

	case 4: //blue
	{
		return 31;
		break;
	}

	case 5: //brown
	{
		return 48192;
		break;
	}

	case 6: //yellow
	{
		return 65504;
		break;
	}

	case 7: //grey
	{
		return 33840;
		break;
	}
	}
}