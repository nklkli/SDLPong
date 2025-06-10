module;
#include <SDL3/SDL_events.h>
export module Paddle;
export import Actor;
import Pong;
import Ball;
import std;
using std::function, std::println, std::string_view,
std::string, std::to_string, std::unique_ptr, std::make_unique;




class PaddleBase :Entity
{
	friend
	unique_ptr<PaddleBase> createPaddle(
		bool humandPaddle, 
		Pong* pong, 
		int player, 
		float x, 
		float y);

	string _image{ "blank" };
	// Each bat has a timer which starts at zero and counts down by one every frame.
//		When a player concedes a point, their timer is set to 20, which causes
//		the bat to display a different animation frame. It is also used to
//		decide when to create a new ball in the centre of the screen - see comments
//		in Game.update for more on this.
//		Finally, it is used in Game.draw to determine when to display a visual
//		effect over the top of the background 
	int _timer{};
	virtual float move_y() = 0;
protected:
	Pong* _pong{ nullptr };
	int _player{}; // 0 or 1
	float _x{}, _y{};
	float _speed = 6;
public:
	void draw() const override;
	void update(float elapsedSeconds) override;
};




class PaddleHuman : public PaddleBase
{
	float _move_y{};
	float move_y() override;
public:
	PaddleHuman() = default;
	void handleInput(SDL_Event* event) override;
};


class PaddleComputer : public PaddleBase
{
	float move_y() override;
public:
	PaddleComputer() = default;
	float MAX_AI_SPEED{ 6 };
	float ai_offset{};
};





void PaddleBase::draw() const
{

}


void PaddleBase::update(float elapsedSeconds)
{
	_timer -= 1;

	// Our movement function tells us how much to move on the Y axis
	float    y_movement = move_y();

	// Apply y_movement to y position, ensuring bat does not go through the side walls
	_y = fmin(400.f, fmax(80.f, _y + y_movement));

	// Choose the appropriate sprite.
	// There are 3 sprites per player - e.g.bat00 is the left - hand player's
	// standard bat sprite, bat01 is the sprite to use when the ball
	// has just bounced off the bat, and bat02
	// is the sprite to use when the bat has just missed the ball and
	// the ball has gone out of bounds.
	// bat10, 11 and 12 are the equivalents for the right-hand player

	int frame = 0;
	if (_timer > 0) {
		if (_pong->ball.out())
			frame = 2;
		else
			frame = 1;
	}

	_image = "bat" + to_string(_player) + to_string(frame);
}


float PaddleHuman::move_y()
{
	float y = _move_y;
	_move_y = 0;
	return y;
}


void PaddleHuman::handleInput(SDL_Event* event)
{
	if (event->type != SDL_EVENT_KEY_DOWN) return;

	if (_player == 0)
	{
		if (event->key.scancode == SDL_SCANCODE_A ||
			event->key.scancode == SDL_SCANCODE_UP) // move up
		{
			_move_y = -_speed;
		}
		else if (event->key.scancode == SDL_SCANCODE_Z ||
			event->key.scancode == SDL_SCANCODE_DOWN) // move down
		{
			_move_y = _speed;
		}
	}
	else if (_player == 1) {

		if (event->key.scancode == SDL_SCANCODE_K) // move up
		{
			_move_y = -_speed;
		}
		else if (event->key.scancode == SDL_SCANCODE_M) // move down
		{
			_move_y = _speed;
		}
	}

}


float PaddleComputer::move_y()
{
	/*Returns a number indicating how the computer player will move -
	e.g. 4 means it will move 4 pixels down the screen. */

	// To decide where we want to go, we first check to see how far we are from the ball.
	float x_distance = fabs(_pong->ball.x - _x);

	// If the ball is far away, we move towards the centre of the screen(HALF_HEIGHT),
	// on the basis that we don't yet know whether the ball will be in the top or
	// bottom half of the screen when it reaches our position on the X axis.
	// By waiting at a central position, we're as ready as it's possible to be for all eventualities.
	float target_y_1 = _pong->HALF_HEIGHT;

	// If the ball is close, we want to move towards its position on the Y axis.
	// We also apply a small offset which is randomly generated each time the ball bounces.
	// This is to make the computer player slightly less robotic
	// - a human player wouldn't be able to hit the ball right in the centre of the bat each time.
	float target_y_2 = _pong->ball.y + ai_offset;

	//The final step is to work out the actual Y position we want to move towards.
	// We use what's called a weighted average -
	// taking the average of the two target Y positions we've previously calculated,
	//	but shifting the
	//	balance towards one or the other depending on how far away the ball is.
	//	If the ball is more than 400 pixels
	//	(half the screen width) away on the X axis,
	//	our target will be half the screen height(target_y_1).
	//	If the ball is at the same position as us on the X axis,
	//	our target will be target_y_2.If it's 200 pixels away,
	//	we'll aim for halfway between target_y_1 and target_y_2.
	//	This reflects the idea that as the ball gets closer,
	//  we have a better idea of where it's going to end up.
	float weight1 = fmin(1.0f, x_distance / _pong->HALF_WIDTH);
	float weight2 = 1 - weight1;

	float target_y = (weight1 * target_y_1) + (weight2 * target_y_2);

	// Subtract target_y from our current Y position,
	// then make sure we can't move any further than MAX_AI_SPEED each frame
	return fmin(MAX_AI_SPEED, fmax(-MAX_AI_SPEED, target_y - _y));

}


unique_ptr<PaddleBase> createPaddle(bool humandPaddle, Pong* pong, int player, float x, float y)
{
	unique_ptr<PaddleBase> p;
	if (humandPaddle)
		p = make_unique<PaddleHuman>();
	else
		p = make_unique<PaddleComputer>();
	p->_pong = pong;
	p->_player = player;
	p->_x = x;
	p->_y = y;

	return move(p);
}


//
//class Paddle : public Actor
//{
//	const Pong& _game;
//	const int _player;
//	int _score{ 0 };
//	TMOVEFUNC _move_paddle;
//	static constexpr float MAX_AI_SPEED = 6;
//
//public:
//	/* Each bat has a timer which starts at zero and counts down by one every frame.
//		When a player concedes a point, their timer is set to 20, which causes
//		the bat to display a different animation frame. It is also used to
//		decide when to create a new ball in the centre of the screen - see comments
//		in Game.update for more on this.
//		Finally, it is used in Game.draw to determine when to display a visual
//		effect over the top of the background */
//	int timer_{ 0 };
//
//	Paddle(const Pong& game, int player, TMOVEFUNC);
//	~Paddle() override;
//	float ai_move() const;
//	void update(float time) override;
//	void handleInput(SDL_Event*) override;
//};
//
//
//
//
//
//Paddle::Paddle(const Pong& game, int player, TMOVEFUNC move_func) :
//	_game(game), _player(player)
//{
//
//	position.x = player == 0 ? game.PADDLE_LEFT_X : game.PADDLE_RIGHT_X;
//	position.y = game.HALF_HEIGHT;
//
//	/* move_func is a function we may or may not have been passed
//	 * by the code which created this object.
//	 * If this bat is meant to be player controlled,
//	 move_func will be a function that when called, returns a number indicating
//	the direction and speed in which the bat should move,
//	based on the keys the player is currently pressing.
//	If move_func is None, this indicates that this bat should instead be controlled by the AI method.		*/
//
//	// wrap out member function 'ai_move' in suitable function pointer
//	auto p_ai_move = [this](SDL_Event * _)
//		{
//			return ai_move();
//		};
//
//	_move_paddle = move_func == nullptr ? p_ai_move : move_func;
//
//}
//
//
//
//Paddle::~Paddle()
//{
//	println("{} dtor: Player {}",
//		typeid(*this).name(),
//		_player);
//}
//
//
//float Paddle::ai_move() const
//{
//
//	/*Returns a number indicating how the computer player will move -
//	e.g. 4 means it will move 4 pixels down the screen. */
//
//	// To decide where we want to go, we first check to see how far we are from the ball.
//	float x_distance = fabs(_game.ball.get_position().x - position.x);
//
//	// If the ball is far away, we move towards the centre of the screen(HALF_HEIGHT),
//	// on the basis that we don't yet know whether the ball will be in the top or
//	// bottom half of the screen when it reaches our position on the X axis.
//	// By waiting at a central position, we're as ready as it's possible to be for all eventualities.
//	float target_y_1 = _game.HALF_HEIGHT;
//
//	// If the ball is close, we want to move towards its position on the Y axis.
//	// We also apply a small offset which is randomly generated each time the ball bounces.
//	// This is to make the computer player slightly less robotic
//	// - a human player wouldn't be able to hit the ball right in the centre of the bat each time.
//	float target_y_2 = _game.ball.get_position().y + _game.ai_offset;
//
//	//The final step is to work out the actual Y position we want to move towards.
//	// We use what's called a weighted average -
//	// taking the average of the two target Y positions we've previously calculated,
//	//	but shifting the
//	//	balance towards one or the other depending on how far away the ball is.
//	//	If the ball is more than 400 pixels
//	//	(half the screen width) away on the X axis,
//	//	our target will be half the screen height(target_y_1).
//	//	If the ball is at the same position as us on the X axis,
//	//	our target will be target_y_2.If it's 200 pixels away,
//	//	we'll aim for halfway between target_y_1 and target_y_2.
//	//	This reflects the idea that as the ball gets closer,
//	//  we have a better idea of where it's going to end up.
//	float weight1 = fmin(1.0f, x_distance / _game.HALF_WIDTH);
//	float weight2 = 1 - weight1;
//
//	float target_y = (weight1 * target_y_1) + (weight2 * target_y_2);
//
//	// Subtract target_y from our current Y position,
//	// then make sure we can't move any further than MAX_AI_SPEED each frame
//	return fmin(MAX_AI_SPEED, fmax(-MAX_AI_SPEED, target_y - position.y));
//
//
//}
//
//
//void Paddle::update(float time)
//{
//	_timer -= 1;
//
//	// Our movement function tells us how much to move on the Y axis
//}
//
//
//void Paddle::handleInput(SDL_Event* event)
//{	
//	auto y_movement = _move_paddle(event);
//}

