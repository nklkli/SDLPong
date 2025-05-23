#pragma once

#include <string>
#include "Engine.h"
#include <memory>





class Actor
{
public:
	Actor(const std::string& image, float x, float y) :
		image{ image }, x{ x }, y{ y }
	{
	}

	Actor(const std::string& image) :
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
		x = x;
		y = y;
	}

	inline float position_x() const {
		return x;
	}

	inline float position_y() const {
		return y;
	}

	
	const std::string image;

private:
	float x{ 0 };
	float y{ 0 };
};




class Game {

public:
	Game(Engine * engine) :
		engine{ engine }
	{

	}

	void draw() const;

private:
	std::unique_ptr<Engine> engine;

};
