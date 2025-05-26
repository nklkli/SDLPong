
export module Pong;

export import std;

import IEngine;
import Game;

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
class PongGame : public IGame
{
	std::unique_ptr<IEngine> engine;
	

public:
	explicit	PongGame(std::unique_ptr<IEngine> e) : engine{ std::move(e) }
	{

	}


	void update(float elapsedSeconds) {

	}

	void draw() const {
		engine->draw("table", { 0,0 });
	}

	~PongGame() {
		std::print("Game dtor\n");
	}
};
