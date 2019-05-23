#pragma once
#include <vector>
#include <iostream>

class ListPasos
{
public:
	ListPasos(int map[3][3]);
	ListPasos(int map[3][3], long largo);
	ListPasos(int map[3][3], long largo, std::vector<int> pas);
	ListPasos();
	std::vector<int> moves;
	int map[3][3];
	long largo;

	void mostrar_pantalla();
	~ListPasos();
};

