
#include "LCD2.h"
#include <stdexcept>
#include "configuration.h"

#define MAXROWS 1
#define MAXCOLS 15
#define IMGPATH ("LCD2.png")
#define FONTPATH ("digital-7.ttf")
#define FONTCOLOR al_map_rgb(0, 0, 0)
#define OFFSETY 55
#define OFFSETX 34

using namespace std;

LCD2::LCD2() : error("No errors", "LCD2 is working right", 0ul) {

	msg.resize(32);		//Creamos 32 espacios en el string
	cursor.row = 0;		//Reiniciamos la posicion del cursor
	cursor.column = 0;

	lcdImg = al_load_bitmap(IMGPATH);			//Cargamos la imagen
	if (lcdImg == nullptr) {	
		error = lcdError("Error 1", "Could not load LCD1 image", 1ul);
	}
	else {
		lcdFont = al_load_font(FONTPATH, 40, 0);		//Cargamos la fuente de texto
		if (lcdFont == nullptr) {
			error = lcdError("Error 2", "Could not load LCD1 font", 2ul);
		}
		else {
			redraw();	//En caso de que no haya habido errores, dibujamos el display en pantalla
		}
	}
}

LCD2::~LCD2() {

	al_destroy_bitmap(lcdImg);	//Destruimos los recursos de allegro utilizados
	al_destroy_font(lcdFont);
}

bool LCD2::lcdInitOk() {

	if (error.getErrorCode() == 0) {	//Si no hubo errores en la inicializacion, devolvemos true
		return true;
	}
	else {
		return false;
	}
}

lcdError LCD2::lcdGetError() {	

	return error;
}

cursorPosition LCD2::lcdGetCursorPosition() {

	return cursor;
}

bool LCD2::lcdSetCursorPosition(const cursorPosition pos) {
	
	bool exitState = false;

	if (0 <= pos.column && pos.column < MAXCOLS && 0 <= pos.column && pos.column < MAXROWS) {	//Mientra sea posible ir a la posicion del LCD solicitada...
		cursor.row = pos.row;			//Movemos el cursor a esa posicion
		cursor.column = pos.column;		
		exitState = true;
	}

	redraw();	//Redibujamos el cambio

	return exitState;
}

bool LCD2::lcdMoveCursorLeft() {

	bool exitState = false;

	if (cursor.row == 0) {			//Si estamos en la primera fila
		if (cursor.column == 0) {	//Si estamos en la primera columna
			exitState = false;
		}
		else {
			cursor.column--;
			exitState = true;
		}
	}
	else if (cursor.row == 1) {		//Si estamos en la segunda fila
		if (cursor.column == 0) {	//Si estamos en la primera columna
			cursor.row = 0;
			cursor.column = MAXCOLS;
			exitState = true;
		}
		else {
			cursor.column--;
			exitState = true;
		}
	}
	
	redraw();		//Redibujamos el cambio

	return exitState;
}

bool LCD2::lcdMoveCursorRight() {

	bool exitState = false;

	if (cursor.row == 0) {				//Si estamos en la primera fila
		if (cursor.column == MAXCOLS) {	//Si estamos al final de la columna
			lcdMoveCursorDown();		//Desplazamos el cursor para abajo y al principio de la fila
			cursor.column = 0;
			exitState = true;
		}
		else {
			cursor.column++;			//Sino lo desplazamos para la derecha
			exitState = true;
		}
	}
	else if (cursor.row == 1) {			//Si estamos en la segunda fila
		if (cursor.column == MAXCOLS) {	//Si estamos al final de la columna
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

	if (cursor.row == 0) {	//Si estamos en la primera fila, movemos el cursor para abajo
		cursor.row++;
		exitState = true;
	}

	redraw();

	return exitState;
}

bool LCD2::lcdMoveCursorUp() {

	bool exitState = false;

	if (cursor.row == 1) {	//Si estamos en la segunda fila, movemos el cursor para arriba
		cursor.row--;
		exitState = true;
	}
	redraw();

	return exitState;
}

bool LCD2::lcdClear() {

	msg.clear();		//Borramos completamente el string
	msg.resize(32);		//Le asignamos 32 espacios nuevamente
	cursor.row = 0;		//Reiniciamos la posicion del cursor
	cursor.column = 0;

	redraw();			//Redibujamos los cambios

	return true;
}

bool LCD2::lcdClearToEOL() {

	int from = cursor.row * (MAXCOLS+1) + cursor.column;		//Calculamos la posicion actual del cursor dentro del string
	int to = (from > MAXCOLS) ? (2*(MAXCOLS+1)) : (MAXCOLS+1);	//Determinamos hasta que posicion hay que borrar

	for (int i = from; i < to; i++) {		//Borramos las posiciones del display desde la posicion actual del cursor hasta la posicion a borrar
		msg[i] = ' ';
	}

	redraw();

	return true;
}

basicLCD& LCD2::operator<<(const unsigned char* c) {
	
	string str((const char*)c);	//Convertimos el c string a string c++
	
	int msgPos = cursor.row * (MAXCOLS+1) + cursor.column; //Calculamos la posicion actual del cursor dentro del string

	if (msgPos + str.size() > 2*(MAXCOLS+1)) {	//Si se pasa de la cantidad maxima de caracteres que entran desde la posicion actual del cursor...

		int overflow = msgPos + str.size() - 32;	//Calculamos el exceso de caracteres

		for (int i = 0; i < str.size() - overflow; i++) {	//Dibujamos en el display los ultimos caracteres que entran en el espacio disponible
			msg[msgPos + i] = str[i + overflow];
			lcdMoveCursorRight();		//Avanzamos el cursor hacia la derecha
		}
	}
	else {
		for (int i = 0; i < str.size(); i++) {		//Si no se para de la cantidad maxima de caracteres, dibujamos los caracteres en orden y movemos el cursor
			msg[msgPos + i] = str[i];
			lcdMoveCursorRight();
		}
	}

	redraw();

	return *this;
}

basicLCD& LCD2::operator<<(const unsigned char c) {

	int msgPos = cursor.row * (MAXCOLS+1) + cursor.column;
	
	msg[msgPos] = c;		//Escribimos el caracter en la posicion que indica msgPos
	lcdMoveCursorRight();	//Movemos el cursor hacia la derecha
	
	redraw();

	return *this;
}

void LCD2::redraw() {

	bool finished = false;

	al_clear_to_color(FONTCOLOR);	//Llenamos el fondo de negro

	int lcdImgX = al_get_bitmap_width(lcdImg);	//Obtenemos las dimensiones de la imagen del display...
	int lcdImgY = al_get_bitmap_height(lcdImg);	
	float xCorrection = SCREENRATIOX(lcdImgX);	//Y de sus correcciones en cada eje
	float yCorrection = SCREENRATIOY(lcdImgY);
	al_draw_scaled_bitmap(lcdImg, 0, 0, lcdImgX, lcdImgY, 0, 0, lcdImgX * xCorrection, lcdImgY * yCorrection, 0);	//Escribimos la imagen del display en el buffer

	for (int i = 0; i <= MAXROWS && !finished; i++){
		for (int j = 0; j <= MAXCOLS && !finished; j++){

			int currentPosition = i * (MAXCOLS+1) + j;

			if (currentPosition < msg.size()) {
				al_draw_textf(lcdFont, FONTCOLOR, (OFFSETX*j + OFFSETX/2) * xCorrection, (OFFSETY*i + OFFSETY/2) * yCorrection, 0, "%c", msg[currentPosition]); //Escribimos cada caracter en su posicion correspodiente
			}
			else {
				finished = true;
			}
		}
	}

	al_draw_text(lcdFont, FONTCOLOR, (OFFSETX*cursor.column + OFFSETX/2) * xCorrection, (OFFSETY*cursor.row + OFFSETY/2 + 10) * yCorrection, 0, "_");	//Escribimos el cursor en el buffer

	al_flip_display();	//Dibujamos el buffer en la pantalla
}