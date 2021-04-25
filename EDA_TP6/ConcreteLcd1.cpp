#include <iostream>
#include "ConcreteLcd1.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_color.h>   

#define D_WIDTH		650
#define D_HEIGTH	250

#define FILS	2
#define COLS	16

#define OFFSETY		((double)D_HEIGTH / FILS)  		
#define OFFSETX		((double)D_WIDTH / COLS)

#define FONT		"OpenSans-Semibold.ttf"



ConcreteLcd1::ConcreteLcd1() : error("", "", 0) {
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			matrix[i][j].character = '\0';
		}
	}
	AllegroInitialized = false;
	lcdCursor.column = 0;
	lcdCursor.row = 0;
	display = nullptr;
	font = nullptr;
	errorName = "";
	errorDescription = "";
	errorCode = 0;
	if (!initAllegroLCD1())
	{
		errorName = "Failed to initalize LCD1";
		errorDescription = "Unable to create allegro resources";
		errorCode = 2005;
		this->error = lcdError(errorName, errorDescription, errorCode);
	}
};


ConcreteLcd1::~ConcreteLcd1()
{
	if (AllegroInitialized)
	{
		DeleteAllegroResources();
	}
}


lcdError ConcreteLcd1::lcdGetError()
{
	return error;
}


void ConcreteLcd1::lcdSetChar(int i, int j, unsigned char a)
{
	matrix[i][j].character =  a;
	drawDisplay();
}

bool ConcreteLcd1::lcdInitOk()
{
	return AllegroInitialized;
}
bool ConcreteLcd1::lcdClear()
{
	for (int i = 0; i < FILS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			lcdSetChar(i, j, '\0');
		}
	}
	drawDisplay();
	return true;
}
bool ConcreteLcd1::lcdClearToEOL()
{
	for (int j = lcdCursor.column; j < COLS; j++)
	{
		matrix[lcdCursor.row][j].character = '\0';
	}
	drawDisplay();
	return true;
}

bool ConcreteLcd1::lcdMoveCursorUp()
{
	if (lcdCursor.row > 0)
	{
		lcdCursor.row--;
	}
	drawDisplay();
	return true;
}
bool ConcreteLcd1::lcdMoveCursorDown()
{
	if (lcdCursor.row < FILS - 1)
	{
		lcdCursor.row++;
	}
	drawDisplay();
	return true;
}
bool ConcreteLcd1::lcdMoveCursorRight()
{
	if (lcdCursor.column == COLS - 1 && lcdCursor.row == 0)
	{
		lcdCursor.row = 1;
		lcdCursor.column = 0;
	}
	else if (lcdCursor.column != COLS - 1 || lcdCursor.row != FILS - 1)
	{
		lcdCursor.column++;
	}

	drawDisplay();
	return true;
}
bool ConcreteLcd1::lcdMoveCursorLeft()
{
	if (lcdCursor.column == 0 && lcdCursor.row > 0)
	{
		lcdCursor.row--;
		lcdCursor.column = COLS - 1;
	}
	else if (lcdCursor.row != 0 || lcdCursor.column != 0)
	{
		lcdCursor.column--;
	}
	drawDisplay();
	return true;
}
bool ConcreteLcd1::lcdSetCursorPosition(const cursorPosition pos)
{
	lcdCursor = pos;
	drawDisplay();
	return true;
}
cursorPosition ConcreteLcd1::lcdGetCursorPosition()
{
	return lcdCursor;
}

basicLCD& ConcreteLcd1::operator<<(const unsigned char c)
{
	lcdSetChar(lcdCursor.row, lcdCursor.column, c);
	lcdMoveCursorRight();
	drawDisplay();
	return *this;
}
basicLCD& ConcreteLcd1::operator<<(const unsigned char* c)
{
	int freeChars = getFreeChars();
	int size = 0;
	for (int i = 0; c[i] != '\0'; i++)
	{
		size++;
	}
	if (freeChars - size < 0) 
	{
		int indexStartCopyingFrom = size - freeChars;
		for (int i = 0; i < freeChars; i++)
		{
			matrix[lcdCursor.row][lcdCursor.column].character = c[indexStartCopyingFrom + i];
			lcdMoveCursorRight();
		}
	}
	else 
	{
		for (int i = 0; i < size; i++)
		{
			matrix[lcdCursor.row][lcdCursor.column].character = c[i];
			lcdMoveCursorRight();
		}
	}
	return *this;
}

int ConcreteLcd1::getFreeChars()
{
	int freeChars = COLS - lcdCursor.column;
	for (int i = lcdCursor.row + 1; i < FILS; i++)
	{
		freeChars += COLS;
	}
	return freeChars;
}



bool ConcreteLcd1::initAllegroLCD1()
{
	if (!al_init()) {
		return false;
	}

	if (!al_init_primitives_addon())
		return false;
	if (!al_init_font_addon())
		return false;
	if (!al_init_ttf_addon())
		return false;
	font = al_load_font(FONT, 20, 0);
	if (font == nullptr)
		return false;
	/*display = al_create_display(D_WIDTH, D_HEIGTH);
	if(display == nullptr)
	{
		al_destroy_font(font);
		return false;
	}*/
	drawDisplay();
	AllegroInitialized = true;
	return true;
}

void ConcreteLcd1::DeleteAllegroResources()
{
	//al_destroy_display(display);
	al_destroy_font(font);
}


void ConcreteLcd1::drawDisplay()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			al_draw_filled_rectangle( OFFSETX * j, OFFSETY * i, OFFSETX * j + OFFSETX,  OFFSETY * i + OFFSETY , al_color_name("green"));
			al_draw_textf(font, al_map_rgb(0, 0, 0), OFFSETX * j + OFFSETX / 2, OFFSETY * i + OFFSETY / 2, 0, "%c", matrix[i][j].character);
		}
	}

	al_draw_filled_rectangle(OFFSETX * lcdCursor.column, OFFSETY * lcdCursor.row, OFFSETX * lcdCursor.column + OFFSETX, OFFSETY * lcdCursor.row + OFFSETY, al_color_name("white"));
	al_draw_textf(font, al_map_rgb(0, 0, 0), OFFSETX * lcdCursor.column + OFFSETX / 2, OFFSETY * lcdCursor.row + OFFSETY / 2, 0, "%c", matrix[lcdCursor.row][lcdCursor.column].character);


	for (int j = 1; j < 16; j++)
	{
		al_draw_line( j * (D_WIDTH / 16.0), D_HEIGTH, j * (D_WIDTH / 16.0), 0, al_color_name("black"), 1.0);
	}
	for (int i = 1; i < 2; i++)
	{
		al_draw_line(0, i * (D_HEIGTH / 2.0), D_WIDTH, i * (D_HEIGTH / 2.0), al_color_name("black"), 1.0);
	}

	al_flip_display();
}