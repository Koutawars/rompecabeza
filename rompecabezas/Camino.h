#pragma once
#include <vector>
#include <iostream>

class Camino
{
public:
	Camino(int mapa[3][3]);
	Camino(int mapa[3][3], long valor);
	Camino(int mapa[3][3], long valor, std::vector<int> moves);
	Camino();
	std::vector<int> moves;
	int mapa[3][3];
	long valor;

	void imprimir();
	~Camino();
};

