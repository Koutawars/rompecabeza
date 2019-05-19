#pragma once
#include "Move.h"
#include <vector>
#include <iostream>

class Camino
{
public:
	Camino(int mapa[3][3], long valor);
	Camino(int mapa[3][3], long valor, std::vector<enum Move> moves);
	Camino();
	std::vector<enum Move> moves;
	int mapa[3][3];
	long valor;

	void imprimir();
	~Camino();
};

