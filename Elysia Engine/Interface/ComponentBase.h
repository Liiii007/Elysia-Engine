#pragma once
#include <unordered_map>
#include <functional>
#include <rapidjson/document.h>

#include <Renderer/imgui/imgui.h>
#include <Renderer/imgui/imgui_impl_win32.h>
#include <Renderer/imgui/imgui_impl_dx12.h>

class Entity;

class ComponentBase
{
public:
	ComponentBase(Entity* entity);

	virtual void DrawEditorUI() {
		return;
		//Log::Error("Base");
	}

	Entity* parentEntity;
	static std::unordered_map<std::string, std::function<void(Entity&, const rapidjson::Value&)>> initList;
};

#ifndef ReflectBindComponent
#define ReflectBindComponent(T)                        \
std::string T::componentName = #T;                    \
void T::Bind() {                                       \
	ComponentBase::initList[T::componentName] = &Parse;\
} 
#endif // !ReflectBindComponent

