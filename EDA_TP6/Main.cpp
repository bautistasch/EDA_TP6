#include <iostream>
#include <stdexcept>
#include "allegroResources.h"
#include "ConcreteLcd1.h"
#include "lcd2.h"

using namespace std;

int main()
{
	/*
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
	*/

	basicLCD* lcd[3];		

	try {
		initAllegro();
		lcd[0] = new LCD2;		
		lcd[1] = new ConcreteLcd1;
		lcd[2] = new LCD2; //Cambiar LCD2 por LCDMati
		
		for (int i = 0; i < 3; i++) {	
			lcd[i]->lcdInitOk();
		}
		
		runSimulation(lcd);

		for (int i = 0; i < 3; i++) {
			delete lcd[i];
		}
		
	}
	catch(const exception& e){
		cout << "Exception: " << e.what() << endl;
	}

	destroyAllegro();
}