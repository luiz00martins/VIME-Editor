#pragma once
#include <vector>
#include <string>
#include "vimeWall.h"
#include "vimeObject.h"

class vimeMap {
private:
	std::vector<vimeWall*> walls;
	std::vector<vimeObject*> objects;

public:
	~vimeMap();

	bool wallExists(vimeWall w);
	void addWall(vimeWall w);
	void removeWall(vimeWall w);
	vimeWall getWall(int index);
	int getLen();

	void save(std::string filePath);
};


class file_not_found : public std::exception {
public:
	file_not_found() noexcept : std::exception("file not found", 1) {}

private:
	file_not_found(char const* const _Message) noexcept : exception(_Message, 1) {}
};