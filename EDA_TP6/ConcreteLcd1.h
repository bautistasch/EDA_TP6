#pragma once
#include <iostream>
#include "basicLCD.h"
//********************                             
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>       
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_color.h>   
//********************

class charAttributes{
public:
	unsigned char character;
};
typedef charAttributes LcdMatrix[2][16];

class ConcreteLcd1 : public basicLCD{
public:

	ConcreteLcd1();
	~ConcreteLcd1();
	bool lcdInitOk();
	lcdError lcdGetError();
	bool lcdClear();
	bool lcdClearToEOL();
	basicLCD& operator<<(const unsigned char c);
	basicLCD& operator<<(const unsigned char* c);
	bool lcdMoveCursorUp();
	bool lcdMoveCursorDown();
	bool lcdMoveCursorRight();
	bool lcdMoveCursorLeft();
	bool lcdSetCursorPosition(const cursorPosition pos);
	cursorPosition lcdGetCursorPosition();
private:
	lcdError error;
	bool initError;
	std::string errorName;
	std::string errorDescription;
	unsigned long errorCode;
	LcdMatrix matrix;
	void DeleteAllegroResources();
	bool initAllegroLCD1();
	bool AllegroInitialized;
	void lcdSetChar(int i, int j, unsigned char a);
	void drawDisplay();
	int getFreeChars();
	cursorPosition lcdCursor;          
	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT* font;
};
