#include "pch.h"
#include "Gpa.h"


Gpa::Gpa(){}


Gpa::~Gpa(){}

Gpa& Gpa::TraerSingleton()
{
	static Gpa instance;
	return instance;
}

void Gpa::inciarVariables() {
	switch (vista) {
	case 0:
		pestañas = std::vector<std::string>();
		posicionEn_x_pestañas = 125;
		posicionEn_y_pestañas = 230;
		distancia_texto = 180;
		slect = -1;
		break;
	case 1: 
	{
		win = false;
		numPas = 0;
		numPasMin = 0;
		desordenar = true;
		this->dar_solucion = false;
		this->pasados = std::vector<ListPasos>();
		int objetivo[3][3] = { {1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 0} };

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				this->objetivo[i][j] = objetivo[i][j];
			}
		}
		mapaAlzara(this->actual); // se genera map aleatorio

		for (int i = 0; i < 3; i ++) {
			for (int j = 0; j < 3; j++) {
				if (this->actual[i][j] == 0) {
					this->I = i;
					this->J = j;
				}
			}
		}

		posMapaX = 20, posMapaY = 20;
		TMap = 400;
		std::ifstream texto("puntoalto.txt");
		while (!texto.eof()) {
			texto >> numPasMin;
		}
		texto.close();
		break;
	}

	}
}

void Gpa::cargarTodo() {
	switch (vista) {
	case 0:
		font = al_load_font("Ghiya Strokes Reg.ttf", 48, NULL);
		pestañas.push_back("Iniciar");
		pestañas.push_back("Info");
		pestañas.push_back("Salir");
		break;
	case 1:

		font = al_load_font("Ghiya Strokes Reg.ttf", 48, NULL);
		font_aux = al_load_font("Ghiya Strokes Reg.ttf", 20, NULL);
		imagen = al_load_bitmap("image.png");
		break;
	case 2:

		font = al_load_font("Ghiya Strokes Reg.ttf", 48, NULL);
		break;
	}

	this->rePintar = true;
}
void Gpa::usar_algoritmo() {
	calcarMap(this->objetivo, this->actual);
	I = 2, J = 2;
}
void Gpa::aplicar_procedimiento() {
	std::vector<ListPasos> caminos = std::vector<ListPasos>();
	caminos.push_back(ListPasos(this->actual, Calcosto(this->actual))); // se crea el primer camino y se coloca en la matriz de caminos
	mostarMAt(this->actual);
	this->pasos_resolver = Aestrella(caminos);
	pasos_resolver.mostrar_pantalla();
	
}
void Gpa::procedimiento_Pc() {
	if (pasos_resolver.moves.size() > 0) {
		switch (pasos_resolver.moves[0])
		{
		case 0: // ARRIBA = 0
			trocar(this->actual, I, J, I - 1, J);
			I--;
			pasos_resolver.moves.erase(pasos_resolver.moves.begin());
			break;
		case 1: // ABAJO == 1
			trocar(this->actual, I, J, I + 1, J);
			I++;
			pasos_resolver.moves.erase(pasos_resolver.moves.begin());
			break;
		case 2: // DERECHA = 2
			trocar(this->actual, I, J, I, J + 1);
			J++;
			pasos_resolver.moves.erase(pasos_resolver.moves.begin());
			break;
		case 3: // IZQUIERDA = 3
			trocar(this->actual, I, J, I, J - 1);
			J--;
			pasos_resolver.moves.erase(pasos_resolver.moves.begin());
			break;
		}
	}
}


void Gpa::refrescar(ALLEGRO_EVENT ev, bool *done) {
	switch (vista) {
	case 0:
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button & 1)
			{
				float mouseY = ev.mouse.x;
				for (int i = 0; i < pestañas.size(); i++) {
					int posicionTexto = (distancia_texto * i + posicionEn_x_pestañas) - 30;
					if (posicionTexto < mouseY && posicionTexto + distancia_texto > mouseY) {
						switch (i) {
						case 0:
							moverPestana(1);
							break;
						case 1:
							moverPestana(2);
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
			for (int i = 0; i < pestañas.size(); i++) {
				int posicionTexto = (distancia_texto * i + posicionEn_x_pestañas)-30;
				if (posicionTexto < mouseY && posicionTexto + distancia_texto > mouseY) {
					slect = i;
					rePintar = true;
					encontrado = true;
				}
			}
			if (!encontrado && slect != -1) {
				slect = -1;
				rePintar = true;
			}
		}
		break;
	case 1:
		if (this->dar_solucion == false) {

			if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
			{
				if (ev.mouse.button & 1)
				{
					float mouseY = ev.mouse.y;
					float mouseX = ev.mouse.x;
					if (mouseX > 640 - 100 && mouseY < 80) {
						moverPestana(0);
					}
				}
			}
			if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_H:
					numPas = 0;
					usar_algoritmo();
					desordenar = false;
					rePintar = true;
					break;
				case ALLEGRO_KEY_UP:
					if (esta_dentro(I - 1, J)) {
						trocar(this->actual, I - 1, J, I, J);
						I = I - 1;
						if(desordenar)numPas++;
						rePintar = true;
					}
				break;
				case ALLEGRO_KEY_DOWN:
					if (esta_dentro(I + 1, J)) {
						trocar(this->actual, I + 1, J, I, J);
						I = I + 1;
						if (desordenar)numPas++;
						rePintar = true;
					}
				break;
				case ALLEGRO_KEY_LEFT:
					if (esta_dentro(I, J - 1)) {
						trocar(this->actual, I, J - 1, I, J);
						J = J - 1;
						if (desordenar)numPas++;
						rePintar = true;
					}
					break;
				case ALLEGRO_KEY_RIGHT:
					if (esta_dentro(I, J + 1)) {
						trocar(this->actual, I, J + 1, I, J);
						J = J + 1;
						if (desordenar)numPas++;
						rePintar = true;
					}
					break;
				case ALLEGRO_KEY_L:
					this->pasados.clear();
					calcarMap(objetivo, actual);
					mapaAlzara(actual);
					desordenar = false;
					numPas = 0;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							if (this->actual[i][j] == 0) {
								this->I = i;
								this->J = j;
							}
						}
					}
					aplicar_procedimiento();
					dar_solucion = true;
					break;
				case ALLEGRO_KEY_M:
					this->pasados.clear();
					numPas = 0;
					calcarMap(objetivo, actual);
					mapaAlzara(actual);
					desordenar = true;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							if (this->actual[i][j] == 0) {
								this->I = i;
								this->J = j;
							}
						}
					}
					rePintar = true;
					break;
				}
				if(Calcosto(this->actual) == 0){
					win = true;
				}
				else {
					win = false;

				}
			}
		}
		else {
			procedimiento_Pc();
			rePintar = true;
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
					moverPestana(0);
				}
			}
		}
		break;
	}
}

void Gpa::pintarPantalla(ALLEGRO_DISPLAY *display) {
	switch (vista) {
	case 0:
		for (int i = 0; i < pestañas.size(); i++) {
			al_draw_text(font, al_map_rgb(219, 48, 48), posicionEn_x_pestañas+ (i * distancia_texto) ,posicionEn_y_pestañas, NULL, pestañas[i].c_str());
		}
		break;
	case 1:
		mostarMAt(this->actual);
		int calculo, calculo2, a;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				calculo = (int)((this->actual[i][j] - 1) / 3);
				calculo2 = (this->actual[i][j] + 2) % 3;
				std::cout << "pos: "<< this->actual[i][j] << std::endl;
				std::cout << " Y: " << calculo << " X: " << calculo2 << std::endl;
				if (this->actual[i][j] != 0) {
					al_draw_bitmap_region(imagen, calculo2 * (TMap / 3), calculo * (TMap / 3), (TMap / 3), (TMap / 3), (posMapaX + j * (TMap / 3)), (posMapaY + i * (TMap / 3)), NULL);
				}
				else {
					if(win)al_draw_bitmap_region(imagen, 2 * (TMap / 3), 2 * (TMap / 3), (TMap / 3), (TMap / 3), (posMapaX + j * (TMap / 3)), (posMapaY + i * (TMap / 3)), NULL);
				}
				//al_draw_filled_rectangle((posMapaX + j*(TMap/3)), (posMapaY + i * (TMap / 3)), (posMapaX + j * (TMap / 3) + (TMap / 3)) - 3, (posMapaY + i * (TMap / 3) + (TMap / 3)) - 3, al_map_rgb(200,200,200));
				//al_draw_textf(font, al_map_rgb(0, 0, 0), (posMapaX + j * (TMap / 3)) + (TMap / 3)/2.5, (posMapaY + i * (TMap / 3)) + (TMap / 3)/3, NULL, "%d", this->actual[i][j]);
			}
		}

		if (dar_solucion == true) {
			al_rest(0.8);
			if (pasos_resolver.moves.size() == 0){
				dar_solucion = false;
				if (Calcosto(this->actual) == 0) {
					al_draw_bitmap_region(imagen, 2 * (TMap / 3), 2 * (TMap / 3), (TMap / 3), (TMap / 3), (posMapaX + 2 * (TMap / 3)), (posMapaY + 2 * (TMap / 3)), NULL);
					win = true;
				}
			}
		}
		al_draw_text(font, al_map_rgb(255, 255, 255), 520, 0, NULL, "Atras");
		if (win) {
			al_draw_text(font, al_map_rgb(255, 255, 255), 520, 400, NULL, "Win");

			if (numPasMin > numPas && desordenar == true) {
				numPasMin = numPas;
				std::ofstream fs("puntoalto.txt");
				fs << numPasMin;

				numPas = 0;
				desordenar = false;
				fs.close();
			}
		}

		al_draw_text(font_aux, al_map_rgb(255, 255, 255), 440, 100, NULL, "L = Completar");
		al_draw_text(font_aux, al_map_rgb(255, 255, 255), 440, 125, NULL, "M = Mezclar");
		al_draw_text(font_aux, al_map_rgb(255, 255, 255), 440, 150, NULL, "flechas = mover");
		al_draw_textf(font_aux, al_map_rgb(255, 255, 255), 440, 175, NULL, "record = %d", numPasMin);

		al_draw_textf(font_aux, al_map_rgb(255, 255, 255), 440, 200, NULL, "Movimientos hechos: %d", numPas);

		if (win) {
			al_flip_display();
			al_rest(2);
			this->moverPestana(0);
			al_clear_to_color(al_map_rgb(0, 0, 0));
			this->pintarPantalla(display);
		}
		break;
	case 2:
		al_draw_text(font, al_map_rgb(255, 255, 255), 520, 0, NULL, "Atras");
		break;
	}
}

void Gpa::eliminarTodo() {
	switch (vista) {
	case 0:
		al_destroy_font(font);
		pestañas.clear();
		break;
	case 1:
		al_destroy_font(font);
		al_destroy_font(font_aux);
		al_destroy_bitmap(imagen);
		break;
	case 2:
		al_destroy_font(font);
		break;
	}
}

ListPasos Gpa::Aestrella(std::vector<ListPasos> caminos) {
	ListPasos menor = caminos[0];
	int indice = 0;
	for (size_t i = 0; i < caminos.size(); i++) {
		if (caminos[i].largo < menor.largo) {
			menor = caminos[i];
			indice = i;
		}
	}
	if (Calcosto(menor.map) != 0) {
		caminos = extender(caminos, menor, indice);
		if (caminos.size() == 1) {
			menor = caminos[0];
			return menor;
		}
		else {
			return Aestrella(caminos);
		}
	}
	else {
		return menor;
	}
}

std::vector <ListPasos> Gpa::extender(std::vector<ListPasos> caminos, ListPasos caminoMenor, int indice) {
	int map[3][3], mapaAux[3][3];
	ListPasos caminoAux;
	caminoMenor.mostrar_pantalla();
	calcarMap(caminoMenor.map, map);
	//Sleep(100);
	if (caminoMenor.moves.size() < 100) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// encuentra el numero 0 en la matriz
				if (map[i][j] == 0) {
					// se expande en 4 direcciones
					if (i - 1 != -1) {
						calcarMap(caminoMenor.map, mapaAux);
						trocar(mapaAux, i - 1, j, i, j);
						caminoAux = ListPasos(mapaAux, Calcosto(mapaAux), caminoMenor.moves);
						if (!estaRecorrido(caminoAux)) {
							caminoAux.moves.push_back(0); // arriba
							caminoAux.largo += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (i + 1 != 3) {
						calcarMap(caminoMenor.map, mapaAux);
						trocar(mapaAux, i + 1, j, i, j);
						caminoAux = ListPasos(mapaAux, Calcosto(mapaAux), caminoMenor.moves);
						if (!estaRecorrido(caminoAux)) {
							caminoAux.moves.push_back(1); // abajo
							caminoAux.largo += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (j + 1 != 3) {
						calcarMap(caminoMenor.map, mapaAux);
						trocar(mapaAux, i, j + 1, i, j);
						caminoAux = ListPasos(mapaAux, Calcosto(mapaAux), caminoMenor.moves);
						if (!estaRecorrido(caminoAux)) {
							caminoAux.moves.push_back(2);// derecha
							caminoAux.largo += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}
					if (j - 1 != -1) {
						calcarMap(caminoMenor.map, mapaAux);
						trocar(mapaAux, i, j - 1, i, j);
						caminoAux = ListPasos(mapaAux, Calcosto(mapaAux), caminoMenor.moves);
						if (!estaRecorrido(caminoAux)) {
							caminoAux.moves.push_back(3); // izquierda
							caminoAux.largo += caminoAux.moves.size();
							caminos.push_back(caminoAux);
						}
					}

				}
			}
		}
	}
	pasados.push_back(caminoMenor);
	std::vector <int> indices;
	caminos.erase(caminos.begin() + indice);

	// busca el camino menor
	caminoMenor = caminos[0];
	for (size_t i = 0; i < caminos.size(); i++) {
		if (caminos[i].largo < caminoMenor.largo) {
			caminoMenor = caminos[i];
		}
	}

	if (Calcosto(caminoMenor.map) == 0) {
		std::vector <ListPasos> retornar;
		retornar.push_back(caminoMenor);
		return retornar; 
	}

	for (size_t i = 0; i < caminos.size(); i++) {
		if (caminoMenor.largo == caminos[i].largo) {
			if (Calcosto(caminos[i].map) == 0) {
				std::vector <ListPasos> retornar;
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

void Gpa::moverPestana(int vista) {
	this->eliminarTodo();
	this->vista = vista;
	this->inciarVariables();
	this->cargarTodo();
	rePintar = true;
}

void Gpa::mostarMAt(int matriz[3][3]) {
	std::cout << "------" << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << matriz[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "------" << std::endl;
}

long Gpa::Calcosto(int matriz[3][3]) {
	long value = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int x = 0; x < 3; x++) {
				for (int y = 0; y < 3; y++) {
					if (matriz[i][j] == objetivo[x][y]) {
						value += abs(x - i) + abs(y - j);
					}
				}
			}
		}
	}
	return value;
}

int Gpa::optRand(int max) {
	return rand() % max;
}

void Gpa::configution() {
	srand(time(NULL));
}

void Gpa::trocar(int(*map)[3], int i0, int j0, int i1, int j1) {
	int aux = map[i0][j0];
	map[i0][j0] = map[i1][j1];
	map[i1][j1] = aux;
}

void Gpa::mapaAlzara(int(*map)[3]) {
	std::vector<std::pair < int, int >> posibles = std::vector<std::pair < int, int >>();
	std::pair <int, int> aux;
	calcarMap(this->objetivo, map); // copio
	// revuelvo
	for (int i = 0; i < 20; i++) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (map[i][j] == 0)
				{
					if (esta_dentro(i + 1, j)) {
						posibles.push_back(std::make_pair(i + 1, j));
					}
					if (esta_dentro(i - 1, j)) {
						posibles.push_back(std::make_pair(i - 1, j));
					}
					if (esta_dentro(i, j + 1)) {
						posibles.push_back(std::make_pair(i, j + 1));
					}
					if (esta_dentro(i, j - 1)) {
						posibles.push_back(std::make_pair(i, j - 1));
					}
					aux = posibles[optRand(posibles.size())];
					trocar(map, aux.first, aux.second, i, j);
					posibles.clear();
				}
			}
		}
	}

	mostarMAt(map);

}

bool Gpa::esta_dentro(int x, int y)
{
	return (x >= 0 && x < 3 && y >= 0 && y < 3);
}

void Gpa::calcarMap(int(*map)[3], int(*copia)[3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			copia[i][j] = map[i][j];
		}
	}
}

bool Gpa::estaRecorrido(ListPasos camino) {
	bool returned = false;
	for (long i = 0; i < this->pasados.size(); i++) {
		returned = true;
		for (int x = 0; x < 3; x++) {
			for (int y = 0; y < 3; y++) {
				if (camino.map[x][y] != this->pasados[i].map[x][y]) {
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