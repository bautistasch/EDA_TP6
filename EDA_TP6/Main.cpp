/*



		 $$$$$$\                                                 $$\   $$\
		$$  __$$\                                                $$ | $$ |
		$$ / \__ | $$$$$$\  $$\   $$\  $$$$$$\    $$$$$$\        $$ | $$ |
		$$ |$$$$\ $$  __$$\ $$ | $$ | $$  __$$\  $$  __$$\       $$$$$$$$|
		$$ |\_$$| $$ | \__| $$ | $$ | $$ / $$  | $$ /  $$ |		 \_____$$|
		$$ | $$ | $$ |    | $$ | $$ | $$ | $$  | $$ |  $$ |			   $$|
		\$$$$$$ | $$ |      \$$$$$$ | $$$$$$$  | \$$$$$$  |			   $$|
		 \______/ \__|       \______/ $$  ____/   \______/		      \__|
									  $$ |
									  $$ |
									  \__|

*/

#include <iostream>
#include <stdexcept>
#include "allegroResources.h"
#include "ConcreteLcd1.h"
#include "lcd2.h"
#include "lcd3.h"

using namespace std;

int main()
{
	basicLCD* lcd[3];		
	bool error = false;

	cout<<"Instrucciones:\n\nPara escribir, utiliza el teclado y presiona la tecla ENTER para que se vea en pantalla.Aclaracion, si la palabra que escribiste es muy larga solo apareceran los ultimos 32 caracteres "<< endl;
	cout << "Para mover el cursor, utiliza la flechas derecha, izquierda, arriba y abajo." << endl;
	cout << "Para borrar todos los caracteres, utiliza la tecla BACKSPACE." << endl;
	cout << "Para borrar desde la posicion actual hasta el final, utiliza la tecla TAB." << endl;
	cout << "Para alternar entre los diferentes lcd, utiliza la tecla CTRL.\n" << endl;
	

	try {
		initAllegro();

		lcd[0] = new LCD2;
		lcd[1] = new ConcreteLcd1;
		lcd[2] = new LCD3;

		for (int i = 0; i < 3 && !error; i++) {
			if (!lcd[i]->lcdInitOk()) {
				cout << "Code: " << lcd[i]->lcdGetError().getErrorCode() << " "
					<< lcd[i]->lcdGetError().getErrorName() << " "
					<< lcd[i]->lcdGetError().getErrorDescription() << " " << endl;
				error = true;
			}
		}

		if (!error) {
			runSimulation(lcd);
		}
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}

	for (int i = 0; i < 3; i++) {
		delete lcd[i];
	}

	destroyAllegro();
}