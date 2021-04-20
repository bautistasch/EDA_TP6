#include <iostream>

#include "ConcreteLcd1.h"


int main()
{

	// TEST Lcd 1
	std::string casa;
	ConcreteLcd1 myRealLcd;

	basicLCD * AbstractLcd = &myRealLcd;

	AbstractLcd->lcdInitOk();
	cursorPosition CP;
	CP.row = 0;
	CP.column = 0;
	AbstractLcd->lcdSetCursorPosition(CP);
	*AbstractLcd << 'K' << 'A' << 'C' << 'A';
	std::cin >> casa;
	std::string algo("HolaComoEstasEsperoQueBien");
	const unsigned char* kkk =  (const unsigned char * )algo.c_str();
	*AbstractLcd << kkk;                    // No toma la sobrecarga si paso "Hola"
	std::cin >> casa;
	// End Test Lcd1


}