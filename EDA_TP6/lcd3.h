#include "basicLCD.h"
#include <iostream>
#include <stdexcept>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>      
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> 

#define SCREEN_H 150
#define SCREEN_W 650
#define CHAR_LINE_SPACING (SCREEN_H/2)
#define CHAR_SPACING 40
#define CHAR_H 45

class LCD3 : public basicLCD {

public:

	LCD3();
	virtual ~LCD3();

	bool lcdMoveCursorLeft();
	bool lcdMoveCursorRight();
	bool lcdMoveCursorDown();
	bool lcdMoveCursorUp();
	bool lcdInitOk();
	bool lcdClear();
	bool lcdClearToEOL();
	bool lcdSetCursorPosition(const cursorPosition pos);
	cursorPosition lcdGetCursorPosition();

	basicLCD& operator<<(const unsigned char* c);
	basicLCD& operator<<(const unsigned char c);

private:

	void printlcd(void);

	ALLEGRO_DISPLAY* display3;
	ALLEGRO_FONT* font3;
	cursorPosition pos;
	char mat[32];
	
	bool finished_printing;
	bool end_of_lcd;
	bool allegro_error;

};
