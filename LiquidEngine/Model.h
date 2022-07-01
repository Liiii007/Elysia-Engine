#pragma once
#include <vector>
#include <string>

class Model
{
public:
	Model(std::string name) :name(name) {};

	std::string name;

	std::vector<float> vectices;
	std::vector<float> normal;
	std::vector<int> indices;
	
	void importFromDisk(std::string path);


};