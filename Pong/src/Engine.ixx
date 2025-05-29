export module Engine;
import std;
import Point;
using namespace std;


export
class Input 
{
	virtual float GetMouseWheelMoveY() =0;
};


export 
class Engine
{
public:
	virtual void draw(const string&, const Point&) const = 0;	
	virtual void drawText(const string&, const Point&) const = 0;    
	virtual void play(const string& sound) const = 0;  
	virtual ~Engine() =default;
};
