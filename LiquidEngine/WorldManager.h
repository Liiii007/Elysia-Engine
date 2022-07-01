#pragma once
#include <vector>
#include <string>
#include "Translation.h"
#include "Model.h"

struct SingleItem
{
	Model model;
	Translation translation;
};

class WorldManager : public ISerializable
{
public:
	

	std::vector<SingleItem> items;

	void appendItem();
	void removeItem();

	virtual bool read(std::string path);
	virtual bool write(std::string path);
};




