#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>
#include <vector>
#include <string>

enum screen {
	MENU,
	GAMEPLAY,
	ABOUT
};

class Game
{
public:
	Game();
	~Game();
	static Game &GetInstance();
	void initialize();
	void loadContent();
	void unloadContent();
	void update(ALLEGRO_EVENT ev, bool *done);
	void draw(ALLEGRO_DISPLAY *display);
	screen pantalla = MENU;
	bool dibujar;
	void cambiarPantalla(screen pantalla);

	 /*----------- Menu ----------*/

	std::vector < std::string > menu;
	ALLEGRO_FONT *fuente;
	float posXMenu; // posición X del menu
	float posYMenu; // posición Y del menu
	float separador; // separador entre texto
	int select; // Para guardar a quien esta colocando el mouse encima del menu
	

};

