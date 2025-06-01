export module Actor;
import std;
import Point;
using namespace std;

export
class Actor
{
	Point position;

public:
	const string image;
	Actor(const string& image, const Point& p);
	virtual void update(float time);
	inline void go_up(float steps);
	inline void go_down(float steps);
	inline void go_left(float steps);
	inline void go_right(float steps);
	inline void change_position(const Point& newPosition);
	inline Point get_position() const;
	virtual ~Actor();
};

module :private;

Actor::Actor(const string& image, const Point& p) :
	position{ p }, image{ image }
{
}

void Actor::update(float time) {
}

inline void Actor::go_up(float steps) {
	position.y -= steps;
}

inline void Actor::go_down(float steps) {
	position.y += steps;
}

inline void Actor::go_left(float steps) {
	position.x -= steps;
}

inline void Actor::go_right(float steps) {
	position.x += steps;
}

inline void Actor::change_position(const Point& newPosition) {
	position = newPosition;
}

inline  Point Actor::get_position() const {
	return position;
}

inline Actor::~Actor()
{
	print("Actor dtor\n");
}