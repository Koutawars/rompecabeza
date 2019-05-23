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
	case 0:
		menu = std::vector<std::string>();
		posXMenu = 125;
		posYMenu = 230;
		separador = 180;
		select = -1;
		break;
	case 1: 
	{
		gano = false;
		numeroMovimientos = 0;
		numeroMinimo = 0;
		revolver = true;
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
		std::ifstream texto("puntoalto.txt");
		while (!texto.eof()) {
			texto >> numeroMinimo;
		}
		texto.close();
		break;
	}

	}
}

void Game::loadContent() {
	switch (pantalla) {
	case 0:
		fuente = al_load_font("Ghiya Strokes Reg.ttf", 48, NULL);
		menu.push_back("Iniciar");
		menu.push_back("Info");
		menu.push_back("Salir");
		break;
	case 1:

		fuente = al_load_font("Ghiya Strokes Reg.ttf", 48, NULL);
		fuente2 = al_load_font("Ghiya Strokes Reg.ttf", 20, NULL);
		imagen = al_load_bitmap("image.png");
		break;
	case 2:

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
		case 0: // ARRIBA = 0
			intercambiar(this->stateInicial, posI, posJ, posI - 1, posJ);
			posI--;
			solucion.moves.erase(solucion.moves.begin());
			break;
		case 1: // ABAJO == 1
			intercambiar(this->stateInicial, posI, posJ, posI + 1, posJ);
			posI++;
			solucion.moves.erase(solucion.moves.begin());
			break;
		case 2: // DERECHA = 2
			intercambiar(this->stateInicial, posI, posJ, posI, posJ + 1);
			posJ++;
			solucion.moves.erase(solucion.moves.begin());
			break;
		case 3: // IZQUIERDA = 3
			intercambiar(this->stateInicial, posI, posJ, posI, posJ - 1);
			posJ--;
			solucion.moves.erase(solucion.moves.begin());
			break;
		}
	}
}


void Game::update(ALLEGRO_EVENT ev, bool *done) {
	switch (pantalla) {
	case 0:
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				float mouseY = ev.mouse.x;
				for (int i = 0; i < menu.size(); i++) {
					int posicionTexto = (separador * i + posXMenu) - 30;
					if (posicionTexto < mouseY && posicionTexto + separador > mouseY) {
						switch (i) {
						case 0:
							cambiarPantalla(1);
							break;
						case 1:
							cambiarPantalla(2);
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
			float mouseY = ev.mouse.x;
			bool encontrado = false;
			for (int i = 0; i < menu.size(); i++) {
				int posicionTexto = (separador * i + posXMenu)-30;
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
	case 1:
		if (this->solucionando == false) {

			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (ev.mouse.button & 1)
				{
					float mouseY = ev.mouse.y;
					float mouseX = ev.mouse.x;
					if (mouseX > 640 - 100 && mouseY < 80) {
						cambiarPantalla(0);
					}
				}
			}
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_H:
					numeroMovimientos = 0;
					solucionar();
					revolver = false;
					dibujar = true;
					break;
				case ALLEGRO_KEY_UP:
					if (esOK(posI - 1, posJ)) {
						intercambiar(this->stateInicial, posI - 1, posJ, posI, posJ);
						posI = posI - 1;
						if(revolver)numeroMovimientos++;
						dibujar = true;
					}
				break;
				case ALLEGRO_KEY_DOWN:
					if (esOK(posI + 1, posJ)) {
						intercambiar(this->stateInicial, posI + 1, posJ, posI, posJ);
						posI = posI + 1;
						if (revolver)numeroMovimientos++;
						dibujar = true;
					}
				break;
				case ALLEGRO_KEY_LEFT:
					if (esOK(posI, posJ - 1)) {
						intercambiar(this->stateInicial, posI, posJ - 1, posI, posJ);
						posJ = posJ - 1;
						if (revolver)numeroMovimientos++;
						dibujar = true;
					}
					break;
				case ALLEGRO_KEY_RIGHT:
					if (esOK(posI, posJ + 1)) {
						intercambiar(this->stateInicial, posI, posJ + 1, posI, posJ);
						posJ = posJ + 1;
						if (revolver)numeroMovimientos++;
						dibujar = true;
					}
					break;
				case ALLEGRO_KEY_L:
					this->visitado.clear();
					copiarMapa(stateFinal, stateInicial);
					mapaAleatorio(stateInicial);
					revolver = false;
					numeroMovimientos = 0;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							if (this->stateInicial[i][j] == 0) {
								this->posI = i;
								this->posJ = j;
							}
						}
					}
					solucionarMaq();
					solucionando = true;
					break;
				case ALLEGRO_KEY_M:
					this->visitado.clear();
					numeroMovimientos = 0;
					copiarMapa(stateFinal, stateInicial);
					mapaAleatorio(stateInicial);
					revolver = true;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							if (this->stateInicial[i][j] == 0) {
								this->posI = i;
								this->posJ = j;
							}
						}
					}
					dibujar = true;
					break;
				}
				if(generarValor(this->stateInicial) == 0){
					gano = true;
				}
				else {
					gano = false;

				}
			}
		}
		else {
			movimientosMaq();
			dibujar = true;
		}
		break;
	case 2:
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				float mouseY = ev.mouse.y;
				float mouseX = ev.mouse.x;
				if (mouseX > 640 - 100 && mouseY < 80) {
					cambiarPantalla(0);
				}
			}
		}
		break;
	}
}

void Game::draw(ALLEGRO_DISPLAY *display) {
	switch (pantalla) {
	case 0:
		for (int i = 0; i < menu.size(); i++) {
			al_draw_text(fuente, al_map_rgb(219, 48, 48), posXMenu+ (i * separador) ,posYMenu, NULL, menu[i].c_str());
		}
		break;
	case 1:
		imprimirMatriz(this->stateInicial);
		int calculo, calculo2, a;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				calculo = (int)((this->stateInicial[i][j] - 1) / 3);
				calculo2 = (this->stateInicial[i][j] + 2) % 3;
				std::cout << "pos: "<< this->stateInicial[i][j] << std::endl;
				std::cout << " Y: " << calculo << " X: " << calculo2 << std::endl;
				if (this->stateInicial[i][j] != 0) {
					al_draw_bitmap_region(imagen, calculo2 * (tamanoMapa / 3), calculo * (tamanoMapa / 3), (tamanoMapa / 3), (tamanoMapa / 3), (mapaX + j * (tamanoMapa / 3)), (mapaY + i * (tamanoMapa / 3)), NULL);
				}
				else {
					if(gano)al_draw_bitmap_region(imagen, 2 * (tamanoMapa / 3), 2 * (tamanoMapa / 3), (tamanoMapa / 3), (tamanoMapa / 3), (mapaX + j * (tamanoMapa / 3)), (mapaY + i * (tamanoMapa / 3)), NULL);
				}
				//al_draw_filled_rectangle((mapaX + j*(tamanoMapa/3)), (mapaY + i * (tamanoMapa / 3)), (mapaX + j * (tamanoMapa / 3) + (tamanoMapa / 3)) - 3, (mapaY + i * (tamanoMapa / 3) + (tamanoMapa / 3)) - 3, al_map_rgb(200,200,200));
				//al_draw_textf(fuente, al_map_rgb(0, 0, 0), (mapaX + j * (tamanoMapa / 3)) + (tamanoMapa / 3)/2.5, (mapaY + i * (tamanoMapa / 3)) + (tamanoMapa / 3)/3, NULL, "%d", this->stateInicial[i][j]);
			}
		}

		if (solucionando == true) {
			al_rest(0.8);
			if (solucion.moves.size() == 0){
				solucionando = false;
				if (generarValor(this->stateInicial) == 0) {
					al_draw_bitmap_region(imagen, 2 * (tamanoMapa / 3), 2 * (tamanoMapa / 3), (tamanoMapa / 3), (tamanoMapa / 3), (mapaX + 2 * (tamanoMapa / 3)), (mapaY + 2 * (tamanoMapa / 3)), NULL);
					gano = true;
				}
			}
		}
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 520, 0, NULL, "Atras");
		if (gano) {
			al_draw_text(fuente, al_map_rgb(255, 255, 255), 520, 400, NULL, "Win");

			if (numeroMinimo > numeroMovimientos && revolver == true) {
				numeroMinimo = numeroMovimientos;
				std::ofstream fs("puntoalto.txt");
				fs << numeroMinimo;

				numeroMovimientos = 0;
				revolver = false;
				fs.close();
			}
		}

		al_draw_text(fuente2, al_map_rgb(255, 255, 255), 440, 100, NULL, "L = Completar");
		al_draw_text(fuente2, al_map_rgb(255, 255, 255), 440, 125, NULL, "M = Mezclar");
		al_draw_text(fuente2, al_map_rgb(255, 255, 255), 440, 150, NULL, "flechas = mover");
		al_draw_textf(fuente2, al_map_rgb(255, 255, 255), 440, 175, NULL, "record = %d", numeroMinimo);

		al_draw_textf(fuente2, al_map_rgb(255, 255, 255), 440, 200, NULL, "Movimientos hechos: %d", numeroMovimientos);

		if (gano) {
			al_flip_display();
			al_rest(2);
			this->cambiarPantalla(0);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			this->draw(display);
		}
		break;
	case 2:
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 520, 0, NULL, "Atras");
		break;
	}
}

void Game::unloadContent() {
	switch (pantalla) {
	case 0:
		al_destroy_font(fuente);
		menu.clear();
		break;
	case 1:
		al_destroy_font(fuente);
		al_destroy_font(fuente2);
		al_destroy_bitmap(imagen);
		break;
	case 2:
		al_destroy_font(fuente);
		break;
	}
}

Camino Game::aAsterisco(std::vector<Camino> caminos) {
	Camino menor = caminos[0];
	int indice = 0;
	for (size_t i = 0; i < caminos.size(); i++) {
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
	if (caminoMenor.moves.size() < 100) {
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
							caminoAux.moves.push_back(0); // arriba
							caminoAux.valor += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (i + 1 != 3) {
						copiarMapa(caminoMenor.mapa, mapaAux);
						intercambiar(mapaAux, i + 1, j, i, j);
						caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
						if (!estaVisitado(caminoAux)) {
							caminoAux.moves.push_back(1); // abajo
							caminoAux.valor += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (j + 1 != 3) {
						copiarMapa(caminoMenor.mapa, mapaAux);
						intercambiar(mapaAux, i, j + 1, i, j);
						caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
						if (!estaVisitado(caminoAux)) {
							caminoAux.moves.push_back(2);// derecha
							caminoAux.valor += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (j - 1 != -1) {
						copiarMapa(caminoMenor.mapa, mapaAux);
						intercambiar(mapaAux, i, j - 1, i, j);
						caminoAux = Camino(mapaAux, generarValor(mapaAux), caminoMenor.moves);
						if (!estaVisitado(caminoAux)) {
							caminoAux.moves.push_back(3); // izquierda
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
	for (size_t i = 0; i < caminos.size(); i++) {
		if (caminos[i].valor < caminoMenor.valor) {
			caminoMenor = caminos[i];
		}
	}

	if (generarValor(caminoMenor.mapa) == 0) {
		std::vector <Camino> retornar;
		retornar.push_back(caminoMenor);
		return retornar; 
	}

	for (size_t i = 0; i < caminos.size(); i++) {
		if (caminoMenor.valor == caminos[i].valor) {
			if (generarValor(caminos[i].mapa) == 0) {
				std::vector <Camino> retornar;
				retornar.push_back(caminos[i]);
				return retornar; // si es el camino menor y estas en el goal retorna un vector con el
			}
		}
	}
	if (caminos.size() > 1000) {
		std::cout << caminos.size() << std::endl;
	}

	return caminos;
}

void Game::cambiarPantalla(int pantalla) {
	this->unloadContent();
	this->pantalla = pantalla;
	this->initialize();
	this->loadContent();
	dibujar = true;
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