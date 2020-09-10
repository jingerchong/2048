#include <windows.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <cstring>
#include <cmath>
#include <random>
#include <ctime>
#include <vector>

int const gridWidth = 4;
int const gridSize = 16;

int const numInitialTiles = 2;
int const emptyTile = 0;
int const normalTile = 1;
int const specialTile = 2;
int const winTile = 11;
int const maxTileValue = 15;

double const probNormal = 0.1;

int const errorValue = -1;

// float const alpha = 0.0059f;
float const alpha = 0.00625f;

enum class moveDir { Up = 0, Right, Down, Left, Default = -1 };

int newTileValue() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	if (dist(mt) < probNormal) {
		return normalTile;
	}
	else {
		return specialTile;
	}
};

bool doesFileExist(const std::string& name) {
	std::ifstream f(name.c_str());
	return f.good();
}