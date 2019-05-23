#include "pch.h"
#include "ListPasos.h"

ListPasos::ListPasos(int map[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->map[i][j] = map[i][j];
		}
	}
	this->largo = 0;
	this->moves = std::vector<int>();
}

ListPasos::ListPasos(int map[3][3], long largo)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->map[i][j] = map[i][j];
		}
	}
	this->largo = largo;
	this->moves = std::vector<int>();
}

ListPasos::ListPasos(int map[3][3], long largo, std::vector<int> moves) {
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->map[i][j] = map[i][j];
		}
	}
	this->largo = largo;
	this->moves = std::vector<int>(moves);
}

ListPasos::ListPasos()
{
}

ListPasos::~ListPasos()
{
}

void ListPasos::mostrar_pantalla() {
	/*
	std::cout << "cccccc" << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int a = map[i][j];
			std::cout << a << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "largo: " << this->largo << std::endl;
	std::cout << "Moves: ";
	long mov = 0;
	for (long i = 0; i < moves.size(); i++) mov++;
	std::cout << mov << std::endl;
	std::cout << "cccccc" << std::endl;
	*/

}
