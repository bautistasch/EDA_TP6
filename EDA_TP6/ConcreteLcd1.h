#pragma once
#include <iostream>
#include "basicLCD.h"
//********************                             
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>       // meter lo de allegro en otro lado
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

	ConcreteLcd1() {
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				matrix[i][j].character = '\0';            
			}
		}
		AllegroInitialized = false;
	};
	~ConcreteLcd1();
	bool lcdInitOk();
	//lcdError lcdGetError();
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
	LcdMatrix matrix;
	void DeleteAllegroResources();
	bool initAllegroLCD1();
	bool AllegroInitialized;
	void lcdSetChar(int i, int j, unsigned char a);
	void drawDisplay();
	int getFreeChars();
	cursorPosition lcdCursor;          // meter lo de allegro en otra clase?
	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT* font;
};
