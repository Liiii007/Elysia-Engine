#pragma once
class ICommand
{
public:
	virtual void OnPress() = 0;
	virtual void OnRelease() = 0;
};

