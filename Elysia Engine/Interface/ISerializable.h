#pragma once
#include <string>

//template<typename T>

/// <summary>
/// һ�������л���Ľӿ�
/// Ŀǰ������WorldManager����ڴ������������������Ϣ
/// </summary>
/// 


class ISerializable
{
public:
	virtual bool load(std::string path) = 0;
	virtual bool save(std::string path) = 0;
};

