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
#include <fstream> 
#include <algorithm>

#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <utility>

#include "ListPasos.h"


class Gpa
{
public:
	Gpa();
	~Gpa();
	static Gpa &TraerSingleton();
	void inciarVariables();
	void cargarTodo();
	void eliminarTodo();
	void refrescar(ALLEGRO_EVENT evento, bool *done);
	void pintarPantalla(ALLEGRO_DISPLAY *pDisp);
	int vista = 0;
	bool rePintar;
	void moverPestana(int vista);
	void configution();

	 /*----------- Menu ----------*/

	std::vector < std::string > pestañas;
	ALLEGRO_FONT *font;
	float posicionEn_x_pestañas; // posición X del pestañas
	float posicionEn_y_pestañas; // posición Y del pestañas
	float distancia_texto; // distancia_texto entre texto
	int slect; // Para guardar a quien esta colocando el mouse encima del pestañas

	/*----------- GamePlay ----------*/
	int objetivo[3][3]; // objetivo - hacia donde quiero llegar
	int actual[3][3]; // objetivo - hacia donde quiero llegar
	int I, J;
	ListPasos pasos_resolver;
	std::vector <ListPasos> pasados; // guarda los nodos visitados
	int numPas = 0;
	int numPasMin = 0;
	bool desordenar = true;
	ALLEGRO_BITMAP *imagen;

	float posMapaX, posMapaY;
	float TMap;
	bool win = false;

	void mostarMAt(int mat[3][3]); // imprime una mat
	long Calcosto(int mat[3][3]);  // genera el largo del costo
	int optRand(int max);  // generar numero aleatorio entero
	void trocar(int (*map)[3], int i0, int j0, int i1, int j1); // intercambia la posición pasando un map
	void mapaAlzara(int(*map)[3]); // genera map aleatorio 
	std::vector <ListPasos> extender(std::vector<ListPasos> caminos, ListPasos caminoMenor, int indice); // recibe el numeros de paso actualmente, el pasos que se cree el menor, y el indice del pasos menor 
	void calcarMap(int(*map)[3], int(*cop)[3]); // cop un map
	ListPasos Aestrella(std::vector<ListPasos> paso);//a estrella
	bool estaRecorrido(ListPasos pasos);
	void usar_algoritmo();
	bool esta_dentro(int x, int y);
	bool dar_solucion;
	void aplicar_procedimiento();
	void procedimiento_Pc();

	ALLEGRO_FONT *font_aux;

};

