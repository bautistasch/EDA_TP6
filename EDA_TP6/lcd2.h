#pragma once

#include "basicLCD.h"
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

class LCD2 : public basicLCD {

	public:

		LCD2();				//Constructor
		virtual ~LCD2();	//Destructor

		//La funcion devuelve true si se inicializo bien el display, en caso contrario false
		bool lcdInitOk();	

		//Devuelve el estado de error del display
		lcdError lcdGetError();

		//Borra completamente el display
		bool lcdClear();
		//Borra los espacios desde el cursor hasta el fin de la linea actual
		bool lcdClearToEOL();

		//Escribimos una nueva posicion para el cursor
		bool lcdSetCursorPosition(const cursorPosition pos);
		//Devolvemos el estado del cursor del display
		cursorPosition lcdGetCursorPosition();

		//Funciones para desplazar el cursor del display
		bool lcdMoveCursorLeft();
		bool lcdMoveCursorRight();
		bool lcdMoveCursorDown();
		bool lcdMoveCursorUp();

		//Sobrecarga que permite escribir una cadena de caracteres a partir de la posicion actual del cursor
		basicLCD& operator<<(const unsigned char* c);
		//Sobrecarga que habilita al usuario a escribir un unico char en la posicion actual del cursor
		basicLCD& operator<<(const unsigned char c);

	private:
		cursorPosition cursor;	//Cursor actual
		std::string msg;		//Mensaje mostrado en el display
		ALLEGRO_BITMAP* lcdImg;	//Imagen del LCD2
		ALLEGRO_FONT* lcdFont;	//Fuente de texto del LCD2
		lcdError error;			//Objeto de almacenamiento de error del display

		//La funcion redibuja la imagen y el texto del LCD en la pantalla
		void redraw();			
};