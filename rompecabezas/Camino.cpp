#include "pch.h"
#include "Camino.h"



Camino::Camino(int mapa[3][3], long valor)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->mapa[i][j] = mapa[i][j];
		}
	}
	this->valor = valor;
	this->moves = std::vector<enum Move>();
}

Camino::Camino(int mapa[3][3], long valor, std::vector<enum Move> moves) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->mapa[i][j] = mapa[i][j];
		}
	}
	this->valor = valor;
	this->moves = std::vector<enum Move>(moves);
}

Camino::Camino()
{
}

Camino::~Camino()
{
}

void Camino::imprimir() {

	std::cout << "cccccc" << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << mapa[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "valor: " << this->valor << std::endl;
	std::cout << "Moves: ";
	long mov = 0;
	for (long i = 0; i < moves.size(); i++) mov++;
	std::cout << mov << std::endl;
	std::cout << "cccccc" << std::endl;

}
