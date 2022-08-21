#pragma once

class Entity;

class ComponentBase
{
public:
	ComponentBase(Entity* entity);

	Entity* parentEntity;
};

