#pragma once

class Shader;

class ResourceManager
{
public:
	ResourceManager() = default;

	void Init();

	ResourceManager(const ResourceManager&) = delete;
};

