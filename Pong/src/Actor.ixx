module;
#include <SDL3/SDL_events.h>
export module Actor;
import std;
export import Point;
using namespace std;

export
class Actor
{
protected:
	Point position{ 0,0 };
	int _timer{ 0 };

public:
	const string image;
	virtual void update(float time);
	virtual void handleInput(SDL_Event*);
	void go_up(float steps);
	void go_down(float steps);
	void go_left(float steps);
	void go_right(float steps);
	void change_position(const Point& newPosition);
	Point get_position() const;
	virtual ~Actor();
};




module :private;




void Actor::update(float time) {
}


void Actor::handleInput(SDL_Event*)
{
}


void Actor::go_up(float steps) {
	position.y -= steps;
}

void Actor::go_down(float steps) {
	position.y += steps;
}

void Actor::go_left(float steps) {
	position.x -= steps;
}

void Actor::go_right(float steps) {
	position.x += steps;
}

void Actor::change_position(const Point& newPosition) {
	position = newPosition;
}

Point Actor::get_position() const {
	return position;
}

Actor::~Actor()
{
	print("Actor dtor\n");
}