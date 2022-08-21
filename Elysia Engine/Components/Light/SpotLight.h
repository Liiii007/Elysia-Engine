#pragma once
#include <string>
#include <Interface/IComponent.h>
#include <Interface/ComponentBase.h>

class Entity;
//Define "class X;" here

class SpotLight : public ComponentBase {

public:
    
    SpotLight(Entity* entity);
    static std::string componentName;
    std::string name;
	
    //ReturnType GetValue();
    //SpotLight* SetValue(value);//Return the pointer for Chain calls
    Entity& ReturnParentEntity();//Same as above

    //Some Data Member
};
