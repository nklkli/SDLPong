#pragma once
#include <string>

struct Point
{
	float x;
	float y;

	Point(float x, float y) : x(x), y(y)
	{
	}
};


struct Image
{
	const std::string name;

	Image(const std::string& name) :
		name(name)
	{

	}
};