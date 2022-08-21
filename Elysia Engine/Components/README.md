# Component

## 组件列表
- Translation->Entity的内置组件，用途甚广，因此不需要单独添加
- Mesh->用以处理与保存模型数据，同时存有顶点/索引缓冲区等信息供渲染系统调用
- Material->与Shader联动，存有物体的材质信息（待扩展）
- Light->场景中的灯光定义，目前为平行光



## 使用 `AutoComponentGenerater.py` 脚本自动创建组件

运行后输入组件名字即可

## 手动创建组件

假设组件名为T

对于 `T.h`
```c++
//required
#pragma once
#include <string>
#include "../../Interface/IComponent.h"
#include "../../Interface/ComponentBase.h"

class Entity;
//Define "class X;" here

class T : public ComponentBase {

public:
    
	T(Entity* entity);
	static std::string componentName;
	std::string name;

	//Regist to reflect
	static void Bind();
	//Parse init data from json file
	static void Parse(Entity& entity, const rapidjson::Value& parm);
	
    //optional
    ReturnType GetValue();
    T* SetValue(value);//Return the pointer for Chain calls
    Entity& ReturnParentEntity();//Same as above

    //Some Data Member
};
```

对于 `T.cpp`
```c++
//Note:For dependent component, include X.h here, define "class X;" in T.h

//Required
#include "T.h"
#include "../../World/Entity.h"

std::string T::componentName = "T";
T::T(Entity* entity) : ComponentBase(entity) {}

void T::Bind() {
	ComponentBase::initList[componentName] = &Parse;
}

void T::Parse(Entity& entity, const rapidjson::Value& parm) {
	//Parse logic
}

//Optional
T* T::SetValue(value) {
	mValue = value
	return this;
}

Entity& T::ReturnParentEntity() {
	return *parentEntity;//Define in ComponentBase.h
}
```
随后手动include或是加入 `FullComponentClass.h` 和 `FullComponentHeader.h` 进行自动include即可使用