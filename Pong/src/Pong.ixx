
export module Pong;

export import std;

import Engine;

export
class Actor
{
	float x{ 0 };
	float y{ 0 };

public:
	Actor(const std::string& image, float _x, float _y) :
		x{ _x }, y{ _y }, image{ image }
	{
	}

	explicit	Actor(const std::string& image) :
		image{ image }
	{
	}


	virtual void update(float time) {
	};

	inline void go_up(float steps) {
		y -= steps;
	}

	inline void go_down(float steps) {
		y += steps;
	}

	inline void go_left(float steps) {
		x -= steps;
	}

	inline void go_right(float steps) {
		x += steps;
	}

	inline void change_position(float x, float y) {
		this->x = x;
		this->y = y;
	}

	inline float position_x() const {
		return x;
	}

	inline float position_y() const {
		return y;
	}

	virtual ~Actor()
	{
		std::print("Actor dtor\n");
	}



	const std::string image;

};

export 
class PongGame
{
	std::unique_ptr<Engine> engine;
	const Image table = Image{ "table" };

public:
	PongGame() = default;

	void init(Engine* e) {
		engine.reset(e);
		//table = Image(Point{ 0,0 }, "table");
	}

	void update(float elapsedSeconds) {

	}

	void draw() const {
		engine->draw(table, Point{ 0,0 });
	}

	~PongGame() {
		std::print("Game dtor\n");
	}
};
