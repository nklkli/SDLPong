export module Engine;

import std;

export 
struct Point
{
	float x;
	float y;

	Point(float x, float y) : x(x), y(y)
	{
	}
};

export
struct Image
{
	const std::string name;

	Image(const std::string& name) :
		name(name)
	{

	}
};


export 
class Engine
{
public:
    virtual void draw(const Image&, const Point&) const = 0;
    virtual void play(const std::string& sound) = 0;
    virtual ~Engine() =default;
};
