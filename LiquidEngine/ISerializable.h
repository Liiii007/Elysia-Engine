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
	virtual bool read(std::string path) = 0;
	virtual bool write(std::string path)  = 0;
};

