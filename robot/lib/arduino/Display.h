

#ifndef Display_h
#define Display_h

#include "Arduino.h"

#define UART1 1
#define UART2 2
#define UART3 3

class Display
{
  public:
    Display(int uart);
    void clear();
	void line(int x1, int y1, int x2, int y2, unsigned int color);
    void circle(int x, int y, int r, unsigned int color);
    void circleFilled(int x, int y, int r, unsigned int color);
	void rectangle(int x1, int y1, int x2, int y2,unsigned int color);
	void fill(int x1, int y1, int x2, int y2, unsigned int color);
	
	void text(int x, int y, int font, unsigned int color, String text);
	void text(int x, int y, int font, unsigned int color, int text);
	void text(int x, int y, int font, unsigned int color, float text);
	void text(int x, int y, int font, unsigned int color, double text);
	void text(int x, int y, int font, unsigned int color, char text);
	void text(int x, int y, int font, unsigned int color, long text);
	void text(int x, int y, int font, unsigned int color, unsigned long text);
	
	// void test(int x, int y, int font, unsigned int color, String text);

	void print(void);
	void print(String text);
	void print(int text);
	void print(float text);
	void print(double text);
	void print(char text);
	void print(long text);
	void print(unsigned long text);
	
	void println(void);
	void println(String text);
	void println(int text);
	void println(float text);
	void println(double text);
	void println(char text);
	void println(long text);
	void println(unsigned long text);
	
	void printlnlinesholder(String text);
	
	void brightness(int brightness);
	void enableDrawing();
	void disableDrawing();
	void drawingColor(int color);
	unsigned int convertColorToNextionSystem(unsigned int color);
	
	private:
	int cursorX;
	int cursorY;
	
};

void enddisplaycommand ();

#endif