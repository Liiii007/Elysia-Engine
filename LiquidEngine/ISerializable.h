#pragma once
#include <string>

//template<typename T>

/// <summary>
/// 一个可序列化类的接口
/// 目前用在了WorldManager里，用于储存整个世界的坐标信息
/// </summary>
/// 


class ISerializable
{
public:
	virtual bool read(std::string path) = 0;
	virtual bool write(std::string path)  = 0;
};

