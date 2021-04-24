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

	initAllegro();

	lcd[0] = new LCD2;
	lcd[1] = new ConcreteLcd1;
	lcd[2] = new LCD3; 
		
	for (int i = 0; i < 3 && !error; i++) {	
		if (!lcd[i]->lcdInitOk()) {
			error = true;
		}
	}

	if (!error) {
		runSimulation(lcd);
	}

	for (int i = 0; i < 3; i++) {
		delete lcd[i];
	}

	destroyAllegro();
}