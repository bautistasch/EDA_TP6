
#include "LCD2.h"
#include <stdexcept>

#define MAXROWS 1
#define MAXCOLS 15
#define IMGPATH ("LCD2.png")
#define FONTPATH ("digital-7.ttf")
#define FONTCOLOR al_map_rgb(0, 0, 0)
#define OFFSETY 55
#define OFFSETX 34

using namespace std;

LCD2::LCD2() {

	initializedGood = false;

	msg.resize(32);

	cursor.row = 0;
	cursor.column = 0;

	lcdImg = al_load_bitmap(IMGPATH);
	if (lcdImg == nullptr) {
		throw bad_alloc();
	}

	lcdFont = al_load_font(FONTPATH, 40, 0);
	if (lcdFont == nullptr) {
		throw bad_alloc();
	}

	initializedGood = true;

	redraw();

}

LCD2::~LCD2() {

	al_destroy_bitmap(lcdImg);
	al_destroy_font(lcdFont);

}

bool LCD2::lcdInitOk() {
	return initializedGood;
}

cursorPosition LCD2::lcdGetCursorPosition() {
	return cursor;
}

bool LCD2::lcdSetCursorPosition(const cursorPosition pos) {
	
	bool exitState = false;

	if (0 <= pos.column && pos.column < MAXCOLS && 0 <= pos.column && pos.column < MAXROWS) {
		cursor.row = pos.row;
		cursor.column = pos.column;
		exitState = true;
	}

	redraw();

	return exitState;
}

bool LCD2::lcdMoveCursorLeft() {

	bool exitState = false;

	if (cursor.row == 0) {
		if (cursor.column == 0) {
			exitState = false;
		}
		else {
			cursor.column--;
			exitState = true;
		}
	}
	else if (cursor.row == 1) {
		if (cursor.column == 0) {
			cursor.row = 0;
			cursor.column = MAXCOLS;
			exitState = true;
		}
		else {
			cursor.column--;
			exitState = true;
		}
	}
	
	redraw();

	return exitState;
}

bool LCD2::lcdMoveCursorRight() {

	bool exitState = false;

	if (cursor.row == 0) {
		if (cursor.column == MAXCOLS) {
			lcdMoveCursorDown();
			cursor.column = 0;
			exitState = true;
		}
		else {
			cursor.column++;
			exitState = true;
		}
	}
	else if (cursor.row == 1) {
		if (cursor.column == MAXCOLS) {
			exitState = false;
		}
		else {
			cursor.column++;
			exitState = true;
		}
	}
	
	redraw();

	return exitState;
}

bool LCD2::lcdMoveCursorDown() {

	bool exitState = false;

	if (cursor.row == 0) {
		cursor.row++;
		exitState = true;
	}
	redraw();

	return exitState;
}

bool LCD2::lcdMoveCursorUp() {

	bool exitState = false;

	if (cursor.row == 1) {
		cursor.row--;
		exitState = true;
	}
	redraw();

	return exitState;
}

bool LCD2::lcdClear() {

	msg.clear();
	msg.resize(32);
	cursor.row = 0;
	cursor.column = 0;

	redraw();

	return true;
}

bool LCD2::lcdClearToEOL() {

	int from = cursor.row * (MAXCOLS+1) + cursor.column;
	int to = (from > MAXCOLS) ? (2*MAXCOLS) : (MAXCOLS);

	msg.erase(msg.begin() + from, msg.begin() + to); //Catch the exception if it fails

	redraw();

	return true;
}

basicLCD& LCD2::operator<<(const unsigned char* c) {
	
	string str((const char*)c);
	
	int msgPos = cursor.row * (MAXCOLS+1) + cursor.column;

	if (msgPos + str.size() - 1 >= 30) {				//El -1 es para que no tome en cuenta el terminador
		msg = msg.substr(str.size(), msg.size());
		string trimmedStr = str.substr(msg.size());
		msg.append(trimmedStr);

		cursor.row = MAXROWS;
		cursor.column = MAXCOLS;
	}
	else {
		for (int i = 0; i < str.size(); i++) {
			msg[msgPos+i] = str[i];
			lcdMoveCursorRight();
		}
	}

	redraw();

	return *this;
}

basicLCD& LCD2::operator<<(const unsigned char c) {

	int msgPos = cursor.row * (MAXCOLS+1) + cursor.column;
	
	msg[msgPos] = c;
	lcdMoveCursorRight();
	
	redraw();

	return *this;
}

void LCD2::redraw() {

	bool finished = false;

	al_clear_to_color(FONTCOLOR);

	al_draw_bitmap(lcdImg, 0, 0, 0);

	for (int i = 0; i <= MAXROWS && !finished; i++){
		for (int j = 0; j <= MAXCOLS && !finished; j++){

			int currentPosition = i * (MAXCOLS+1) + j;

			if (currentPosition < msg.size()) {
				al_draw_textf(lcdFont, FONTCOLOR, OFFSETX * j + OFFSETX / 2, OFFSETY * i + OFFSETY / 2, 0, "%c", msg[currentPosition]);
			}
			else {
				finished = true;
			}
		}
	}

	al_draw_text(lcdFont, FONTCOLOR, OFFSETX * cursor.column + OFFSETX / 2, OFFSETY * cursor.row + OFFSETY / 2 + 10, 0, "_");

	al_flip_display();
}