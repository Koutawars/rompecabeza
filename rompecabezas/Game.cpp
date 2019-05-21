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
	case GAMEPLAY: 
	{
		this->solucionando = false;
		this->visitado = std::vector<Camino>();
		int stateFinal[3][3] = { {1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 0} };
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				this->stateFinal[i][j] = stateFinal[i][j];
			}
		}
		mapaAleatorio(this->stateInicial); // se genera mapa aleatorio

		for (int i = 0; i < 3; i ++) {
			for (int j = 0; j < 3; j++) {
				if (this->stateInicial[i][j] == 0) {
					this->posI = i;
					this->posJ = j;
				}
			}
		}

		mapaX = 20, mapaY = 20;
		tamanoMapa = 400;
		break;
	}

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
	case GAMEPLAY:

		fuente = al_load_font("Ghiya Strokes Reg.ttf", 48, NULL);
		break;
	case ABOUT:

		fuente = al_load_font("Ghiya Strokes Reg.ttf", 48, NULL);
		break;
	}

	this->dibujar = true;
}
void Game::solucionar() {
	copiarMapa(this->stateFinal, this->stateInicial);
	posI = 2, posJ = 2;
}
void Game::solucionarMaq() {
	std::vector<Camino> caminos = std::vector<Camino>();
	caminos.push_back(Camino(this->stateInicial, generarValor(this->stateInicial))); // se crea el primer camino y se coloca en la matriz de caminos
	imprimirMatriz(this->stateInicial);
	this->solucion = aAsterisco(caminos);
	solucion.imprimir();
	
}
void Game::movimientosMaq() {
	if (solucion.moves.size() > 0) {
		switch (solucion.moves[0])
		{
		case ARRIBA:
			intercambiar(this->stateInicial, posI, posJ, posI - 1, posJ);
			posI--;
			solucion.moves.erase(solucion.moves.begin());
			break;
		case ABAJO:
			intercambiar(this->stateInicial, posI, posJ, posI + 1, posJ);
			posI++;
			solucion.moves.erase(solucion.moves.begin());
			break;
		case IZQUIERDA:
			intercambiar(this->stateInicial, posI, posJ, posI, posJ - 1);
			posJ--;
			solucion.moves.erase(solucion.moves.begin());
			break;
		case DERECHA:
			intercambiar(this->stateInicial, posI, posJ, posI, posJ + 1);
			posJ++;
			solucion.moves.erase(solucion.moves.begin());
			break;
		}
	}
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
	case GAMEPLAY:
		if (this->solucionando == false) {

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				float mouseY = ev.mouse.y;
				float mouseX = ev.mouse.x;
				if (mouseX > 640 - 100 && mouseY < 80) {
					cambiarPantalla(MENU);
				}
			}
		}
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {

				case ALLEGRO_KEY_H:
					solucionar();
					dibujar = true;
					break;
				case ALLEGRO_KEY_UP:
					if (esOK(posI - 1, posJ)) {
						intercambiar(this->stateInicial, posI - 1, posJ, posI, posJ);
						posI = posI - 1;
						dibujar = true;
					}
					break;
				case ALLEGRO_KEY_DOWN:
					if (esOK(posI + 1, posJ)) {
						intercambiar(this->stateInicial, posI + 1, posJ, posI, posJ);
						posI = posI + 1;
						dibujar = true;
					}
					break;
				case ALLEGRO_KEY_LEFT:
					if (esOK(posI, posJ - 1)) {
						intercambiar(this->stateInicial, posI, posJ - 1, posI, posJ);
						posJ = posJ - 1;
						dibujar = true;
					}
					break;
				case ALLEGRO_KEY_RIGHT:
					if (esOK(posI, posJ + 1)) {
						intercambiar(this->stateInicial, posI, posJ + 1, posI, posJ);
						posJ = posJ + 1;
						dibujar = true;
					}
					break;
				case ALLEGRO_KEY_R:
					this->visitado.clear();
					solucionarMaq();
					solucionando = true;

					break;
				}
			}
		}
		else {
			movimientosMaq();
			dibujar = true;
		}

		break;
	case ABOUT:
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				float mouseY = ev.mouse.y;
				float mouseX = ev.mouse.x;
				if (mouseX > 640 - 100 && mouseY < 80) {
					cambiarPantalla(MENU);
				}
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
	case GAMEPLAY:
		imprimirMatriz(this->stateInicial);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				al_draw_filled_rectangle((mapaX + j*(tamanoMapa/3)), (mapaY + i * (tamanoMapa / 3)), (mapaX + j * (tamanoMapa / 3) + (tamanoMapa / 3)) - 3, (mapaY + i * (tamanoMapa / 3) + (tamanoMapa / 3)) - 3, al_map_rgb(200,200,200));
				al_draw_textf(fuente, al_map_rgb(0, 0, 0), (mapaX + j * (tamanoMapa / 3)) + (tamanoMapa / 3)/2.5, (mapaY + i * (tamanoMapa / 3)) + (tamanoMapa / 3)/3, NULL, "%d", this->stateInicial[i][j]);
			}
		}
		if (solucionando == true) {
			al_rest(0.8);
			if (solucion.moves.size() == 0){
				if(generarValor(this->stateInicial) == 0) 
					solucionando = false;
				else {
					solucionarMaq();
					solucionando = true;
				}
			}
		}
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 520, 0, NULL, "Atras");
		break;
	case ABOUT:
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 520, 0, NULL, "Atras");
		break;
	}
}

void Game::unloadContent() {
	switch (pantalla) {
	case MENU:
		al_destroy_font(fuente);
		menu.clear();
		break;
	case GAMEPLAY:
		al_destroy_font(fuente);
		break;
	case ABOUT:
		al_destroy_font(fuente);
		break;
	}
}

Camino Game::aAsterisco(std::vector<Camino> caminos) {
	Camino menor = caminos[0];
	int indice = 0;
	for (int i = 0; i < caminos.size(); i++) {
		if (caminos[i].valor < menor.valor) {
			menor = caminos[i];
			indice = i;
		}
	}
	if (generarValor(menor.mapa) != 0) {
		caminos = expandir(caminos, menor, indice);
		if (caminos.size() == 1) {
			menor = caminos[0];
			return menor;
		}
		else {
			return aAsterisco(caminos);
		}
	}
	else {
		return menor;
	}
}

std::vector <Camino> Game::expandir(std::vector<Camino> caminos, Camino caminoMenor, int indice) {
	int mapa[3][3], mapaAux[3][3];
	Camino caminoAux;
	caminoMenor.imprimir();
	copiarMapa(caminoMenor.mapa, mapa);
	//Sleep(100);
	if (caminoMenor.moves.size() < 50) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// encuentra el numero 0 en la matriz
				if (mapa[i][j] == 0) {
					// se expande en 4 direcciones
					if (i - 1 != -1) {
						copiarMapa(caminoMenor.mapa, mapaAux);
						intercambiar(mapaAux, i - 1, j, i, j);
						caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
						if (!estaVisitado(caminoAux)) {
							caminoAux.moves.push_back(ARRIBA);
							caminoAux.valor += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (i + 1 != 3) {
						copiarMapa(caminoMenor.mapa, mapaAux);
						intercambiar(mapaAux, i + 1, j, i, j);
						caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
						if (!estaVisitado(caminoAux)) {
							caminoAux.moves.push_back(ABAJO);
							caminoAux.valor += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (j - 1 != -1) {
						copiarMapa(caminoMenor.mapa, mapaAux);
						intercambiar(mapaAux, i, j - 1, i, j);
						caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
						if (!estaVisitado(caminoAux)) {
							caminoAux.moves.push_back(IZQUIERDA);
							caminoAux.valor += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (j + 1 != 3) {
						copiarMapa(caminoMenor.mapa, mapaAux);
						intercambiar(mapaAux, i, j + 1, i, j);
						caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
						if (!estaVisitado(caminoAux)) {
							caminoAux.moves.push_back(DERECHA);
							caminoAux.valor += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}

				}
			}
		}
	}
	visitado.push_back(caminoMenor);
	std::vector <int> indices;
	caminos.erase(caminos.begin() + indice);

	// busca el camino menor
	caminoMenor = caminos[0];
	for (int i = 0; i < caminos.size(); i++) {
		if (caminos[i].valor < caminoMenor.valor) {
			caminoMenor = caminos[i];
		}
	}
	if (generarValor(caminoMenor.mapa) == 0) {
		std::vector <Camino> retornar;
		retornar.push_back(caminoMenor);
		return retornar; 
	}

	for (int i = 0; i < caminos.size(); i++) {
		if (caminoMenor.valor == caminos[i].valor) {
			if (generarValor(caminos[i].mapa) == 0) {
				std::vector <Camino> retornar;
				retornar.push_back(caminos[i]);
				return retornar; // si es el camino menor y estas en el goal retorna un vector con el
			}
		}
	}
	if (caminos.size() > 400) {
		Camino c = caminos[0], d = caminos[1];
		c.moves.empty();
		d.moves.empty();
		caminos.clear();
		caminos.push_back(d);
		caminos.push_back(c);
	}

	return caminos;
}

void Game::cambiarPantalla(screen pantalla) {
	this->unloadContent();
	this->pantalla = pantalla;
	this->initialize();
	this->loadContent();
}

void Game::imprimirMatriz(int matriz[3][3]) {
	std::cout << "------" << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << matriz[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "------" << std::endl;
}

long Game::generarValor(int matriz[3][3]) {
	long value = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					if (matriz[i][j] == stateFinal[x][y]) {
						value += abs(x - i) + abs(y - j);
					}
				}
			}
		}
	}
	return value;
}

int Game::getRandomInt(int max) {
	return rand() % max;
}

void Game::config() {
	srand(time(NULL));
}

void Game::intercambiar(int(*mapa)[3], int i0, int j0, int i1, int j1) {
	int aux = mapa[i0][j0];
	mapa[i0][j0] = mapa[i1][j1];
	mapa[i1][j1] = aux;
}

void Game::mapaAleatorio(int(*mapa)[3]) {
	std::vector<std::pair < int, int >> posibles = std::vector<std::pair < int, int >>();
	std::pair <int, int> aux;
	copiarMapa(this->stateFinal, mapa); // copio
	// revuelvo
	for (int i = 0; i < 20; i++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (mapa[i][j] == 0)
				{
					if (esOK(i + 1, j)) {
						posibles.push_back(std::make_pair(i + 1, j));
					}
					if (esOK(i - 1, j)) {
						posibles.push_back(std::make_pair(i - 1, j));
					}
					if (esOK(i, j + 1)) {
						posibles.push_back(std::make_pair(i, j + 1));
					}
					if (esOK(i, j - 1)) {
						posibles.push_back(std::make_pair(i, j - 1));
					}
					aux = posibles[getRandomInt(posibles.size())];
					intercambiar(mapa, aux.first, aux.second, i, j);
					posibles.clear();
				}
			}
		}
	}

	imprimirMatriz(mapa);

}

bool Game::esOK(int x, int y)
{
	return (x >= 0 && x < 3 && y >= 0 && y < 3);
}

void Game::copiarMapa(int(*mapa)[3], int(*copia)[3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			copia[i][j] = mapa[i][j];
		}
	}
}

bool Game::estaVisitado(Camino camino) {
	bool returned = false;
	for (long i = 0; i < this->visitado.size(); i++) {
		returned = true;
		for (int x = 0; x < 3; x++) {
			for (int y = 0; y < 3; y++) {
				if (camino.mapa[x][y] != this->visitado[i].mapa[x][y]) {
					returned = false;
				}
			}
		}
		if (returned == true) {
			return returned;
		}
	}

	return returned;

}