#include "lcd3.h"

using namespace std;

LCD3::LCD3()
{
	allegro_error = false;

	display3 = al_create_display(SCREEN_W, SCREEN_H);
	if (!display3) {
		fprintf(stderr, "Could not start display.\n");
		allegro_error = true;
	}

	font3 = al_load_font("Minecraft.ttf", 50, 0);
	if (!font3) {
		fprintf(stderr, "Could not load 'MInecraft.ttf'.\n");
		allegro_error = true;
	}

	pos.column = 0;
	pos.row = 0;

	finished_printing = true;
	end_of_lcd = false;

	for (int i = 0; i <= 31; i++)
	{
		mat[i]=' ';	
	}

	printlcd();
}


LCD3::~LCD3() {
	al_destroy_display(display3);
	al_destroy_font(font3);
}

bool LCD3::lcdInitOk() {
	return !(allegro_error);
}

cursorPosition LCD3::lcdGetCursorPosition() {
	cursorPosition actual;
	actual.column = pos.column;
	actual.row = pos.row;
	return actual;
}

bool LCD3::lcdSetCursorPosition(const cursorPosition pos) {

	return false;
}

bool LCD3::lcdMoveCursorLeft() {
	if (pos.column == 0 && pos.row == 1)
	{
		if (pos.row == 1)
			pos.row--;
		pos.column = 15;
	}
	else if (pos.column > 0)
		pos.column--;
	printlcd();
	return true;

}

bool LCD3::lcdMoveCursorRight() {
	
	if (pos.column == 15 && pos.row == 0)
	{
		pos.row++;
		pos.column = 0;
	}
	else if (pos.column == 15 && pos.row == 1)
	{
		end_of_lcd = true;
	}
	else if (pos.column<15)
		pos.column++;

	if (finished_printing == true)
	printlcd();
	return true;
}

bool LCD3::lcdMoveCursorDown() {
	if(pos.row==0)
		pos.row++;
		printlcd();
	return true;
}

bool LCD3::lcdMoveCursorUp() {
	if (pos.row == 1)
		pos.row--;
		printlcd();
	return true;
}

bool LCD3::lcdClear() {
	
	for (int i = 0; i <= 31; i++)
	{
		mat[i] = ' ';
	}
	pos.column = 0;
	pos.row = 0;
	printlcd();
	return true;
}

bool LCD3::lcdClearToEOL() {

	return true;
}

basicLCD& LCD3::operator<<(const unsigned char* c) {
	end_of_lcd = false;
	finished_printing = false;
	for (int i = 0; c[i]!='\0' && end_of_lcd == false; i++)
	{
		*this << c[i];
	}
	finished_printing = true;
	printlcd();
	return *this;
}

basicLCD& LCD3::operator<<(const unsigned char c) {
	mat[(pos.column +(16* pos.row)) ] = c;
;
	lcdMoveCursorRight();
	if (finished_printing == true)
		printlcd();
	return *this;
}

void LCD3::printlcd() {

	al_clear_to_color(al_map_rgb(0,0,255));


	for (int j = 0; j<= 1; j++)
	{
		for (int i = 0; i<=15; i++)
		{
			al_draw_textf(font3, al_map_rgb(255, 255, 255), i*CHAR_SPACING, j * CHAR_LINE_SPACING, 0,"%c", j==0?mat[i]:mat[i+16] );			
		}
	}

	if (pos.row == 0)
		al_draw_line(pos.column * CHAR_SPACING, CHAR_H, (pos.column * CHAR_SPACING) + CHAR_SPACING - 10, CHAR_H, al_map_rgb(255, 0, 0), 5);
	else if (pos.row == 1)
		al_draw_line(pos.column * CHAR_SPACING, CHAR_LINE_SPACING+ CHAR_H, (pos.column * CHAR_SPACING) + CHAR_SPACING - 10, CHAR_LINE_SPACING + CHAR_H, al_map_rgb(255, 0, 0), 5);
	else cout << "ERROR\n" << endl;

	al_flip_display();
}