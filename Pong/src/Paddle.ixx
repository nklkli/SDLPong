export module Paddle;
export import Actor;
import Pong;
using namespace std;

class Paddle : public Actor
{
	const Pong& game_;
	const int player_;
	int score_{ 0 };
	function<float(Input)> move_func_;

public:
	/* Each bat has a timer which starts at zero and counts down by one every frame.
		When a player concedes a point, their timer is set to 20, which causes
		the bat to display a different animation frame. It is also used to
		decide when to create a new ball in the centre of the screen - see comments
		in Game.update for more on this.
		Finally, it is used in Game.draw to determine when to display a visual
		effect over the top of the background */
	int timer_{ 0 };

	Paddle(const Pong& game, int player, function<float(Input)>);
	~Paddle() override;
	float ai_move();
};


module:private;


Paddle::Paddle(const Pong& game, int player, function<float(Input)> move_func) :
	game_(game), player_(player)
{

	position.x = player == 0 ? game.PADDLE_LEFT_X : game.PADDLE_RIGHT_X;
	position.y = game.HALF_HEIGHT;

	// wrap out member function 'ai_move' in suitable function pointer
	auto p_ai_move = [this](Input _)
		{
			return ai_move();
		};

	move_func_ = move_func == nullptr ? p_ai_move : move_func;

	/*
	move_func is a function we may or may not have been passed by the code which created this object. If this bat
	is meant to be player controlled, move_func will be a function that when called, returns a number indicating
	the direction and speed in which the bat should move, based on the keys the player is currently pressing.
	If move_func is None, this indicates that this bat should instead be controlled by the AI method.
		if move_func != None:
			self.move_func = move_func
		else:
			self.move_func = self.ai
	 */

}



Paddle::~Paddle()
{
	println("{} dtor: Player {}",
		typeid(*this).name(),
		player_);
}

float Paddle::ai_move()
{

	/*Returns a number indicating how the computer player will move -
	e.g. 4 means it will move 4 pixels down the screen. */

	// To decide where we want to go, we first check to see how far we are from the ball.
	float x_distance = abs(game_.ball.get_position().x - position.x);

	// If the ball is far away, we move towards the centre of the screen(HALF_HEIGHT),
	// on the basis that we don't yet know whether the ball will be in the top or
	// bottom half of the screen when it reaches our position on the X axis.
	// By waiting at a central position, we're as ready as it's possible to be for all eventualities.
	float target_y_1 = game_.HALF_HEIGHT;

	// If the ball is close, we want to move towards its position on the Y axis.
	// We also apply a small offset which is randomly generated each time the ball bounces.
	// This is to make the computer player slightly less robotic
	// - a human player wouldn't be able to hit the ball right in the centre of the bat each time.
	float target_y_2 = game_.ball.get_position().y + game_.ai_offset;

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
	float weight1 = game_.GetEngine().min(1, x_distance / game_.HALF_WIDTH);
	float weight2 = 1 - weight1;

	float target_y = (weight1 * target_y_1) + (weight2 * target_y_2);

	return 0;
}
