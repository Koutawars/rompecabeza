#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/keyboard.h>

#include <vector>
#include <string>
#include <iostream>

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <utility>

#include "Move.h"
#include "Camino.h"


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
	void config();

	 /*----------- Menu ----------*/

	std::vector < std::string > menu;
	ALLEGRO_FONT *fuente;
	float posXMenu; // posición X del menu
	float posYMenu; // posición Y del menu
	float separador; // separador entre texto
	int select; // Para guardar a quien esta colocando el mouse encima del menu

	/*----------- GamePlay ----------*/
	int stateFinal[3][3]; // stateFinal - hacia donde quiero llegar
	int stateInicial[3][3]; // stateFinal - hacia donde quiero llegar
	int posI, posJ;

	float mapaX, mapaY;
	float tamanoMapa;

	std::vector <Camino> visitado; // guarda los nodos visitados
	void imprimirMatriz(int matriz[3][3]); // imprime una matriz
	long generarValor(int matriz[3][3]);  // genera el valor del costo
	int getRandomInt(int max);  // generar numero aleatorio entero
	void intercambiar(int (*mapa)[3], int i0, int j0, int i1, int j1); // intercambia la posición pasando un mapa
	void mapaAleatorio(int(*mapa)[3]); // genera mapa aleatorio 
	std::vector <Camino> expandir(std::vector<Camino> caminos, Camino caminoMenor, int indice); // recibe el numeros de caminos actualmente, el camino que se cree el menor, y el indice del camino menor 
	void copiarMapa(int(*mapa)[3], int(*copia)[3]); // copia un mapa
	Camino aAsterisco(std::vector<Camino> caminos);
	bool estaVisitado(Camino camino);
	void solucionar();
	Camino solucion;
	bool esOK(int x, int y);
	bool solucionando;
	void solucionarMaq();
	void movimientosMaq();

};

