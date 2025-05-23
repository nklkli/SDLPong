#pragma once
#include <string>
class Engine
{
public:
	virtual void draw(const std::string& image, float x, float y) = 0;
	virtual void play(const std::string& sound) = 0;
};



