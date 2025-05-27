
export module Pong;

export import std;

import IEngine;
import Game;

export
class Actor
{
	Point position;

public:
	Actor(const std::string& image, const Point& p) :
		position{p}, image{image}
	{
	}

	explicit	Actor(const std::string& image) :
		image{ image }
	{
	}


	virtual void update(float time) {
	};

	inline void go_up(float steps) {
		position.y -= steps;
	}

	inline void go_down(float steps) {
		position.y += steps;
	}

	inline void go_left(float steps) {
		position.x -= steps;
	}

	inline void go_right(float steps) {
		position.x += steps;
	}

	inline void change_position(const Point& newPosition) {
		position = newPosition;
	}

	inline const Point get_position() const {
		return position;
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


	void update(float elapsedSeconds) override {

	}



	void draw() const override {
		engine->draw("table", { 0,0 });
	}



	~PongGame() {
		std::print("Game dtor\n");
	}
};
