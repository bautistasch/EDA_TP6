#pragma once

#include "basicLCD.h"
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

class LCD2 : public basicLCD {

	public:

		LCD2();
		virtual ~LCD2();

		bool lcdInitOk();

		bool lcdClear();
		bool lcdClearToEOL();

		bool lcdSetCursorPosition(const cursorPosition pos);
		cursorPosition lcdGetCursorPosition();

		bool lcdMoveCursorLeft();
		bool lcdMoveCursorRight();
		bool lcdMoveCursorDown();
		bool lcdMoveCursorUp();

		basicLCD& operator<<(const unsigned char* c);
		basicLCD& operator<<(const unsigned char c);

	private:
		bool initializedGood;
		cursorPosition cursor;
		std::string msg;
		ALLEGRO_BITMAP* lcdImg;
		ALLEGRO_FONT* lcdFont;

		void redraw();
};