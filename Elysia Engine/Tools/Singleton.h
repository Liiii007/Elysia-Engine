#pragma once
template<typename T>
class Singleton
{
public:
	Singleton()  = delete;
	~Singleton() = delete;

	static T* Get() {
		static T instance {};
		return &instance;
	}
};

