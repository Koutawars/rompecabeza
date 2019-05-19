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
		this->visitado = std::vector<Camino>();
		int stateFinal[3][3] = { {1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 0} };
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				this->stateFinal[i][j] = stateFinal[i][j];
			}
		}
		std::vector<Camino> caminos = std::vector<Camino>();
		int mapaInicial[3][3];
		mapaAleatorio(mapaInicial); // se genera mapa aleatorio

		caminos.push_back(Camino(mapaInicial, generarValor(mapaInicial))); // se crea el primer camino y se coloca en la matriz de caminos
		imprimirMatriz(mapaInicial);
		aAsterisco(caminos).imprimir();
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

Camino Game::aAsterisco(std::vector<Camino> caminos) {
	Camino menor = caminos[0];
	int indice = 0;
	for (int i = 0; i < caminos.size(); i++) {
		if (caminos[i].valor < menor.valor) {
			menor = caminos[i];
			indice = i;
		}
	}
	caminos = expandir(caminos, menor, indice);
	if (caminos.size() == 1) {
		menor = caminos[0];
		return menor;
	}
	else {
		return aAsterisco(caminos);
	}
}

std::vector <Camino> Game::expandir(std::vector<Camino> caminos, Camino caminoMenor, int indice) {
	int mapa[3][3], mapaAux[3][3];
	Camino caminoAux;
	copiarMapa(caminoMenor.mapa, mapa);
	//Sleep(100);
	caminoMenor.imprimir();
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
						caminoAux.valor += caminoAux.moves.size()*2;
						caminos.push_back(caminoAux);
					}
				}
				if (i + 1 != 3) {
					copiarMapa(caminoMenor.mapa, mapaAux);
					intercambiar(mapaAux, i + 1, j, i, j);
					caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
					if (!estaVisitado(caminoAux)) {
						caminoAux.moves.push_back(ABAJO);
						caminoAux.valor += caminoAux.moves.size()*2;
						caminos.push_back(caminoAux);
					}
				}
				if (j - 1 != -1) {
					copiarMapa(caminoMenor.mapa, mapaAux);
					intercambiar(mapaAux, i, j - 1, i, j);
					caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
					if (!estaVisitado(caminoAux)) {
						caminoAux.moves.push_back(IZQUIERDA);
						caminoAux.valor += caminoAux.moves.size()*2;
						caminos.push_back(caminoAux);
					}
				}
				if (j + 1 != 3) {
					copiarMapa(caminoMenor.mapa, mapaAux);
					intercambiar(mapaAux, i, j + 1, i, j);
					caminoAux = Camino(mapaAux, generarValor(mapaAux) + caminoMenor.valor, caminoMenor.moves);
					if (!estaVisitado(caminoAux)) {
						caminoAux.moves.push_back(DERECHA);
						caminoAux.valor += caminoAux.moves.size()*2;
						caminos.push_back(caminoAux);
					}
				}

			}
		}
	}
	visitado.push_back(caminoMenor);
	caminos.erase(caminos.begin() + indice); // borra el camino donde se expandio

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