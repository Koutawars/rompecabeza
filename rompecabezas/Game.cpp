#include "pch.h"
#include "Game.h"


Game::Game(){}


Game::~Game(){}

Game& Game::GetInstance()
{
	static Game instance;
	return instance;
}

void Game::initialize() {
	switch (pantalla) {
	case MENU:
		menu = std::vector<std::string>();
		posXMenu = 30;
		posYMenu = 145;
		separador = 55;
		select = -1;
		break;	
	}
}

void Game::loadContent() {
	switch (pantalla) {
	case MENU:
		fuente = al_load_font("Ghiya Strokes Reg.ttf", 48, NULL);
		menu.push_back("Iniciar");
		menu.push_back("Detalles");
		menu.push_back("Salir");
		break;
	}

	this->dibujar = true;
}

void Game::update(ALLEGRO_EVENT ev, bool *done) {
	switch (pantalla) {
	case MENU:
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				float mouseY = ev.mouse.y;
				for (int i = 0; i < menu.size(); i++) {
					int posicionTexto = separador * i + posYMenu;
					if (posicionTexto < mouseY && posicionTexto + separador > mouseY) {
						switch (i) {
						case 0:
							cambiarPantalla(GAMEPLAY);
							break;
						case 1:
							cambiarPantalla(ABOUT);
							break;
						case 2:
							*done = true;
							break;
						}
					}
				}
			}
		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			float mouseY = ev.mouse.y;
			bool encontrado = false;
			for (int i = 0; i < menu.size(); i++) {
				int posicionTexto = separador * i + posYMenu;
				if (posicionTexto < mouseY && posicionTexto + separador > mouseY) {
					select = i;
					dibujar = true;
					encontrado = true;
				}
			}
			if (!encontrado && select != -1) {
				select = -1;
				dibujar = true;
			}
		}
		break;
	}
}

void Game::draw(ALLEGRO_DISPLAY *display) {
	switch (pantalla) {
	case MENU:
		for (int i = 0; i < menu.size(); i++) {
			if (select == i) {
				al_draw_text(fuente, al_map_rgb(219, 48, 48), posXMenu, (i * separador) + posYMenu, NULL, menu[i].c_str());
			}
			else {
				al_draw_text(fuente, al_map_rgb(255, 255, 255), posXMenu, (i * separador) + posYMenu, NULL, menu[i].c_str());
			}
		}
		break;
	}
}

void Game::unloadContent() {
	switch (pantalla) {
	case MENU:
		al_destroy_font(fuente);
		menu.clear();
		break;
	}
}

void Game::cambiarPantalla(screen pantalla) {
	this->unloadContent();
	this->pantalla = pantalla;
	this->initialize();
	this->loadContent();
}
