export module IEngine;

import std;

export 
struct Point
{
	float x;
	float y;
};




export 
class IEngine
{
public:
	virtual void draw(const std::string&, const Point&) const = 0;
    virtual void play(const std::string& sound) const = 0;
    virtual ~IEngine() =default;
};
