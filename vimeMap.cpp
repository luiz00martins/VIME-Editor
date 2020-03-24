#include "vimeMap.h"
#include <fstream>

#define MAP_SCALE 10

vimeMap::~vimeMap(){
	for (int i = 0; i < walls.size(); i++) {
		delete walls[i];
	}
}

bool vimeMap::wallExists(vimeWall w) {
	for (int i = 0; i < walls.size(); i++) {
		if (*walls[i] == w) {
			return true;
		}
	}
	return false;
}

void vimeMap::addWall(vimeWall w) {
	if (!wallExists(w)) {
		walls.push_back(new vimeWall(w));
	}
}

void vimeMap::removeWall(vimeWall w) {
	for (int i = 0; i < walls.size(); i++) {
		if (*walls[i] == w) {
			walls.erase(walls.begin() + i);
		}
	}
}

vimeWall vimeMap::getWall(int index) {
	return *walls[index];
}

int vimeMap::getLen() {
	return walls.size();
}

void vimeMap::save(std::string filePath)
{
	std::fstream file;
	file.open(filePath, std::ios::out);
	if (file.is_open()) {
		file << "pp 10 3 e\n";
		
		for (vimeWall* wall : walls) {
			file << "w " 
				<< wall->p0.x * MAP_SCALE << ' '
				<< wall->p1.x * MAP_SCALE << ' '
				<< wall->p0.y * MAP_SCALE << ' ' 
				<< wall->p1.y * MAP_SCALE 
				<< " -2 8 1 1 1 1 archs/textures/tiles/textura.png 3 e\n";
		}

		file << "gr -50 50 -50 50 -2 -2 1 1 1 1 archs/textures/tiles/concrete.png 8 e\n";
		file << "gr -50 50 -50 50 8 8 1 1 1 1 archs/textures/tiles/concrete.png 8 e\n";

		for (int i = 0; i < 5; i++) {
			file << "o\n";
		}

		file.close();
	}
	else
		throw new file_not_found();
}
