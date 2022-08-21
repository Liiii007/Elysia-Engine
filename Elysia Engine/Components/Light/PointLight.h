#pragma once
#include <string>
#include <Interface/IComponent.h>
#include <Interface/ComponentBase.h>

class Entity;
//Define "class X;" here

class PointLight : public ComponentBase {

public:
    
    PointLight(Entity* entity);
    static std::string componentName;
    std::string name;
	
    //ReturnType GetValue();
    //PointLight* SetValue(value);//Return the pointer for Chain calls
    Entity& ReturnParentEntity();//Same as above

    //Some Data Member
};
