
#include "allegroResources.h"
#include <stdexcept>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>      
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> 

#define ISAPHABETIC(c) ((ALLEGRO_KEY_A <= (c) && (c) <= ALLEGRO_KEY_Z) ? (1) : (0))
#define ISNUMERIC(n) ((ALLEGRO_KEY_0 <= (n) && (n) <= ALLEGRO_KEY_9) ? (1) : (0))
#define ISALLEGROALPHANUMERIC(c) (( ISAPHABETIC(c) || ISNUMERIC(c) ) ? (1) : (0))
#define KEYCODETOCHAR(k) ( ISAPHABETIC(k) ? ((k)+64) : ((k+21)))

using namespace std;

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* queue;

void initAllegro() {

	if (!al_init()) {
		throw exception("Error al inicializar allegro");
	}

	if (!al_install_mouse()) {
		throw exception("Error al inicializar el mouse");
	}

	if (!al_install_keyboard()) {
		throw exception("Error al inicializar el teclado");
	}

	if (!al_init_primitives_addon()) {
		throw exception("Error al inicializar las primitivas de allegro");
	}

	if (!al_init_font_addon()) {
		throw exception("Error al inicializar font addon de allegro");
	}

	if (!al_init_image_addon()) {
		throw exception("Error al inicializar image addon de allegro");
	}

	if (!al_init_ttf_addon()) {
		throw exception("Error al inicializar ttf addon de allegro");
	}

	display = al_create_display(650, 250);
	if (display == nullptr)
	{
		throw exception("Error al crear la ventana");
	}

	queue = al_create_event_queue();
	if (queue == nullptr)
	{
		throw exception("Error al crear la cola de eventos");
	}

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_mouse_event_source());      

}

void runSimulation(basicLCD* lcd[3]) {

	ALLEGRO_EVENT e;
	string keyboardIn;
	bool isRunning = true;
	int LCDSelected = 0;
	basicLCD* currentLCD = lcd[LCDSelected];

	while (isRunning) {

		if (!al_is_event_queue_empty(queue)) {

			al_wait_for_event(queue, &e);

			switch (e.type)
			{
				case ALLEGRO_EVENT_KEY_DOWN:

					switch (e.keyboard.keycode) {

						case ALLEGRO_KEY_ESCAPE:
							isRunning = false;
							break;

						case ALLEGRO_KEY_UP:
							currentLCD->lcdMoveCursorUp();
							break;

						case ALLEGRO_KEY_DOWN:
							currentLCD->lcdMoveCursorDown();
							break;

						case ALLEGRO_KEY_LEFT:
							currentLCD->lcdMoveCursorLeft();
							break;

						case ALLEGRO_KEY_RIGHT:
							currentLCD->lcdMoveCursorRight();
							break;

						case ALLEGRO_KEY_BACKSPACE:
							currentLCD->lcdClear();
							break;

						case ALLEGRO_KEY_TAB:
							currentLCD->lcdClearToEOL();
							break;

						case ALLEGRO_KEY_ENTER:

							if (keyboardIn.size() == 1) {
								const char c = keyboardIn[0];
								*currentLCD << c;
							}
							else if(keyboardIn.size() > 1){
								const unsigned char* c = (const unsigned char*)keyboardIn.c_str();
								*currentLCD << c;
							}
							keyboardIn.clear();

							break;

						case ALLEGRO_KEY_LCTRL:
						case ALLEGRO_KEY_RCTRL:

							if (LCDSelected < 2) {
								LCDSelected++;
							}
							else {
								LCDSelected = 0;
							}

							currentLCD = lcd[LCDSelected];
							
							break;

						default:
							
							if (ISALLEGROALPHANUMERIC(e.keyboard.keycode)) {
								char c = (char) KEYCODETOCHAR(e.keyboard.keycode);
								keyboardIn.push_back(c);
							}
							else if(e.keyboard.keycode == 75){	//75: espacio 
								char c = e.keyboard.keycode-43;
								keyboardIn.push_back(c);
							}
							else if (e.keyboard.keycode == 74) { //74: signo de pregunta
								char c = e.keyboard.keycode - 11;
								keyboardIn.push_back(c);
							}
							break;
						}
					break;

				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					isRunning = false;
					break;
			}

		}
	}

}

void destroyAllegro()
{
	al_destroy_display(display);
	al_destroy_event_queue(queue);
}