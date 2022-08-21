#pragma once
#include <string>
#include <Interface/IComponent.h>
#include <Interface/ComponentBase.h>

class Entity;
//Define "class X;" here

class DirectLight : public ComponentBase {

public:
    
    DirectLight(Entity* entity);
    static std::string componentName;
    std::string name;
	
    //ReturnType GetValue();
    //DirectLight* SetValue(value);//Return the pointer for Chain calls
    Entity& ReturnParentEntity();//Same as above

    //Some Data Member
};
