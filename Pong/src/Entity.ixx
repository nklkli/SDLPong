module;
#include <SDL3/SDL_events.h>
#include  <typeinfo>
export module Entity;
import Engine;
import std;
using namespace std;

export
class Entity
{
protected:
	string name_;
public:
	virtual void update(float elapsedSeconds) {};
	virtual void draw() const {};
	virtual void handleInput(SDL_Event * event ) {};
	string getName() const
	{
		auto className = typeid(*this).name();
		return format("{}{}", 
			className, 
			name_.empty() ? "": " name=(" + name_ + ")");
	}
	virtual ~Entity() = default;

};