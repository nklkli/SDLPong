#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include "draw.h"
#include "sound.h"
#include "pong.h"
#include <assert.h>




//unsigned int current_word = 0;

//_controlfp(&current_word, _EM_INEXACT, _MCW_EM);

enum State {
	STATE_MENU,
	STATE_PLAYING,
	STATE_GAME_OVER
};


typedef float (*MOVE_Y)(void);


struct KeyboardState
{
	bool UP;
	bool DOWN;
	bool A;
	bool M;
	bool K;
	bool Z;

};



struct Game
{
	struct Bat
	{
		char image[10];
		bool move_with_ai;
		float x, y;  /* position */
		int player, score;
		int timer;
	} bats[2]; // player 1 and player 2 bats.

	struct Ball
	{
		float x, y, dx, dy, speed;
		char image[10];
	} ball;

	float ai_offset;/* a small offset to make the AI less robotic */
	struct KeyboardState keyboard_state;
	float UPDATE_STEP_SECS;
	float elapsedSeconds;
	enum State state;
	int num_players;
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;
	float HALF_HEIGHT;
	float HALF_WIDTH;
	float MAX_AI_SPEED;
	float PLAYER_SPEED;
};




void update_intern(struct Game* game);
void draw(const struct Game* game, SDL_Renderer*);
static float p1_controls(const struct Game*);
static float p2_controls(const struct Game*);
static bool ball_is_out(const struct Ball*, float playfiedl_width);
float move_bat_ai(const struct Game*, const struct Bat*);
//static struct Game create_game(void);
struct Bat create_bat(float HALF_HEIGHT, int player, bool move_with_ai);
void update_bat(const struct Game* game, struct Bat* bat);
struct Ball create_ball(float,float, float direction);
void update_ball(struct Game* game);


struct Game* init(SDL_Renderer* renderer) {
	if (!drawInit(renderer))
		return false;
	if (!soundInit())
		return false;

	struct Game* game = malloc(sizeof(struct Game));
	game->WINDOW_WIDTH = WINDOW_WIDTH;
	game->WINDOW_HEIGHT = WINDOW_HEIGHT;
	game->HALF_WIDTH = game->WINDOW_WIDTH / 2;
	game->HALF_HEIGHT = game->WINDOW_HEIGHT / 2;
	game->MAX_AI_SPEED = 120;
	game->PLAYER_SPEED = 120;
	game->UPDATE_STEP_SECS = 1 / 100.0f;
	game->state = STATE_MENU;
	game->num_players = 1;  /* default to single player mode. */
	game->keyboard_state = (struct KeyboardState){ 0 };
	game->ai_offset = SDL_randf() * 20 - 10;

	game->ball = create_ball(game->HALF_WIDTH, game->HALF_HEIGHT, -1);
	game->bats[0] = create_bat(game->HALF_HEIGHT, 1, true);
	game->bats[1] = create_bat(game->HALF_HEIGHT, 2, true);


	return game;
}



void update(struct Game* game, SDL_Renderer* renderer)
{
	static Uint64 lastTicks = 0;
	Uint64 currentTicks = SDL_GetTicks();
	Uint64 milliSecondsElapsed = currentTicks - lastTicks  ;
	lastTicks = currentTicks;
	float secondsElapsed = (float)milliSecondsElapsed / 1000;

	while (secondsElapsed > 0)
	{
		float update_step = SDL_min(secondsElapsed, game->UPDATE_STEP_SECS);
		game->elapsedSeconds = update_step;
		update_intern(game);
		secondsElapsed -= update_step;
		secondsElapsed = SDL_max(secondsElapsed, 0.0f);
	}


	draw(game, renderer);
}


void update_intern(struct Game* game)
{
	if (game->keyboard_state.UP && game->num_players == 2)
	{
		game->num_players = 1;
		soundPlay("up.ogg");
	}
	else if (game->keyboard_state.DOWN && game->num_players == 1)
	{
		game->num_players = 2;
		soundPlay("foo.opus");
	}
	game->keyboard_state = (struct KeyboardState){ 0 };  /* reset the keyboard state. */

	update_ball(game);
	update_bat(game, &game->bats[0]);
	update_bat(game, &game->bats[1]);
}


void draw(const struct Game* game, SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
	SDL_RenderClear(renderer);  /* start with a blank canvas. */

	drawImage(renderer, "table.png", 0, 0,false);

	switch (game->state)
	{
	case STATE_MENU:
		drawImage(renderer, game->num_players == 1 ? "menu0.png" : "menu1.png", 0, 0, false);
		break;
	case STATE_PLAYING:
		;
		break;
	case STATE_GAME_OVER:
		drawImage(renderer, "over.png", 0, 0, false);
		break;

	}


	drawImage(renderer, (game->ball).image, (game->ball).x, game->ball.y, true);
	drawImage(renderer, (game->bats[0]).image, game->bats[0].x, game->bats[0].y, true);
	drawImage(renderer, (game->bats[1]).image, game->bats[1].x, game->bats[1].y, true);


	SDL_RenderPresent(renderer);

}




void handle_event(struct Game* game, SDL_Event const* event)
{


	if (event->type == SDL_EVENT_KEY_DOWN) {
		if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
			SDL_PushEvent(&(SDL_Event) { .type = SDL_EVENT_QUIT });
		}

		game->keyboard_state.UP = event->key.scancode == SDL_SCANCODE_UP;
		game->keyboard_state.DOWN = event->key.scancode == SDL_SCANCODE_DOWN;
		game->keyboard_state.A = event->key.scancode == SDL_SCANCODE_A;
		game->keyboard_state.K = event->key.scancode == SDL_SCANCODE_K;
		game->keyboard_state.M = event->key.scancode == SDL_SCANCODE_M;
		game->keyboard_state.Z = event->key.scancode == SDL_SCANCODE_Z;
	}
}

void quit(struct Game* game)
{
	drawQuit();
	soundQuit();
	free(game);
}





float p1_controls(const struct Game* game)
{
	if (game->keyboard_state.DOWN || game->keyboard_state.Z)
		return game->PLAYER_SPEED;
	if (game->keyboard_state.UP || game->keyboard_state.A)
		return -game->PLAYER_SPEED;
	return 0.0f;
}



float p2_controls(const struct Game* game)
{
	if (game->keyboard_state.M) // moves down
		return game->PLAYER_SPEED;
	if (game->keyboard_state.K) // moves up
		return -game->PLAYER_SPEED;
	return 0.0f;
}

bool ball_is_out(const struct Ball* ball, float playfiedl_width)
{
	//# Has ball gone off the left or right edge of the screen?
	return ball->x < 0 || ball->x > playfiedl_width;
}


float move_bat_ai(const struct Game* game, const struct Bat* bat)
{
	// The AI logic to move the bat.
	// Returns a number indicating how the computer player will move - e.g. 4 means it will move 4 pixels down the screen.

	//assert(!SDL_isnanf(game->ball.x));

	// To decide where we want to go, we first check to see how far we are from the ball.
	const float x_distance = SDL_fabsf(game->ball.x - bat->x);

	/* # If the ball is far away, we move towards the centre of the screen (HALF_HEIGHT), on the basis that we don't
	 # yet know whether the ball will be in the top or bottom half of the screen when it reaches our position on
	 # the X axis. By waiting at a central position, we're as ready as it's possible to be for all eventualities.*/
	const float target_y_1 = game->HALF_HEIGHT;

	/*		# If the ball is close, we want to move towards its position on the Y axis. We also apply a small offset which
			# is randomly generated each time the ball bounces. This is to make the computer player slightly less robotic
			# - a human player wouldn't be able to hit the ball right in the centre of the bat each time.*/
	const float	target_y_2 = game->ball.y + game->ai_offset;

	/*	# The final step is to work out the actual Y position we want to move towards. We use what's called a weighted
		# average - taking the average of the two target Y positions we've previously calculated, but shifting the
		# balance towards one or the other depending on how far away the ball is. If the ball is more than 400 pixels
		# (half the screen width) away on the X axis, our target will be half the screen height (target_y_1). If the
		# ball is at the same position as us on the X axis, our target will be target_y_2. If it's 200 pixels away,
		# we'll aim for halfway between target_y_1 and target_y_2. This reflects the idea that as the ball gets closer,
		# we have a better idea of where it's going to end up.*/
	const float weight1 = SDL_min(1.0f, x_distance / game->HALF_WIDTH);
	const float weight2 = 1.0f - weight1;

	const float target_y = (weight1 * target_y_1) + (weight2 * target_y_2);

	//# Subtract target_y from our current Y position, then make sure we can't move any further than MAX_AI_SPEED
	//# each frame
	float move_y =  SDL_min(game->MAX_AI_SPEED*game->elapsedSeconds, SDL_max(-game->MAX_AI_SPEED*game->elapsedSeconds, target_y - bat->y));



	return move_y;
	
}




struct Bat create_bat(float HALF_HEIGHT, int player, bool move_with_ai)
{
	struct Bat bat;
	bat.x = player == 1 ? 40.0f : 760.0f; // player 1 is on the left, player 2 on the right.
	bat.y = HALF_HEIGHT;
	bat.player = player;
	bat.score = 0;
	bat.timer = 0;
	//bat.image=;
	bat.move_with_ai = move_with_ai;
	return bat;
}

void update_bat(const struct Game* game, struct Bat* bat)
{
	bat->timer -= 1;

	float  y_movement = 0;

	if (bat->move_with_ai)
		y_movement = move_bat_ai(game, bat);
	else {
		if (bat->player == 1)
			y_movement = p1_controls(game) * game->elapsedSeconds;
		else if (bat->player == 2)
			y_movement = p2_controls(game) * game->elapsedSeconds;
	}

	//SDL_Log("Move bat %.2f, Elapsed: %.4f", y_movement, game->elapsedSeconds);

	//  # Apply y_movement to y position, ensuring bat does not go through the side walls
	bat->y = SDL_min(400, SDL_max(80, bat->y + y_movement));

	/*# Choose the appropriate sprite.There are 3 sprites per player - e.g.bat00 is the left - hand player's
		# standard bat sprite, bat01 is the sprite to use when the ball has just bounced off the bat, and bat02
		# is the sprite to use when the bat has just missed the ball and the ball has gone out of bounds.
		# bat10, 11 and 12 are the equivalents for the right - hand player*/

	int	frame = 0;
	if (bat->timer > 0) {
		if (ball_is_out(&game->ball, game->WINDOW_WIDTH))
			frame = 2;
		else
			frame = 1;
	}

	int nbytes = SDL_snprintf(bat->image, 10, "bat%d%d.png", bat->player - 1, frame);

	assert(nbytes == 9);
}


struct Ball create_ball(float half_width, float half_height, float direction)
{
	return (struct Ball) {
		.x = half_width,
		.y = half_height,
		.dx = direction,
		.dy = 0,
		.speed = 200,
		.image = "ball.png"
	};
}



void update_ball(struct Game* game)
{
	struct Ball* ball = &game->ball;

	float original_x = ball->x;
	float original_y = ball->y;

	ball->x += ball->dx * ball->speed * game->elapsedSeconds;
	ball->y += ball->dy * ball->speed * game->elapsedSeconds;


	
	// Check to see if ball needs to bounce off a bat

	/*
	 To determine whether the ball might collide with a bat, we first measure the horizontal distance from the
ball to the centre of the screen, and check to see if its edge has gone beyond the edge of the bat.
The centre of each bat is 40 pixels from the edge of the screen, or to put it another way, 360 pixels
from the centre of the screen. The bat is 18 pixels wide and the ball is 14 pixels wide. Given that these
sprites are anchored from their centres, when determining if they overlap or touch, we need to look at
their half-widths - 9 and 7. Therefore, if the centre of the ball is 344 pixels from the centre of the
screen, it can bounce off a bat (assuming the bat is in the right position on the Y axis - checked
shortly afterwards).
We also check the previous X position to ensure that this is the first frame in which the ball crossed the threshold.
	 */

	if (SDL_fabsf(ball->x - game->HALF_WIDTH) >= 344 && SDL_fabsf(original_x - game->HALF_WIDTH)<344) {
		// Now that we know the edge of the ball has crossed the threshold on the x-axis, we need to check to
		// see if the bat on the relevant side of the arena is at a suitable position on the y - axis for the
		// ball collide with it.
		float new_dir_x = 0;
		struct Bat* bat = NULL;
		if (ball->x < game->HALF_WIDTH)
		{
			new_dir_x = 1;
			bat = &game->bats[0];
		}
		else
		{
			new_dir_x = -1;
			bat = &game->bats[1];
		}

		float difference_y = ball->y - bat->y;

		if (difference_y > -64 && difference_y < 64)
		{
			// Ball has collided with bat - calculate new direction vector

			/*To understand the maths used below, we first need to consider what would happen with this kind of
				collision in the real world.The ball is bouncing off a perfectly vertical surface.This makes for a
				pretty simple calculation.Let's take a ball which is travelling at 1 metre per second to the right,
				and 2 metres per second down.Imagine this is taking place in space, so gravity isn't a factor.
				After the ball hits the bat, it's still going to be moving at 2 m/s down, but it's now going to be
				moving 1 m / s to the left instead of right.So its speed on the y - axis hasn't changed, but its
				direction on the x - axis has been reversed.This is extremely easy to code - "self.dx = -self.dx".
				However, games don't have to perfectly reflect reality.
				In Pong, hitting the ball with the upper or lower parts of the bat would make it bounce diagonally
				upwards or downwards respectively.This gives the player a degree of control over where the ball
				goes.To make for a more interesting game, we want to use realistic physics as the starting point,
				but combine with this the ability to influence the direction of the ball.When the ball hits the
				bat, we're going to deflect the ball slightly upwards or downwards depending on where it hit the
				bat.This gives the player a bit of control over where the ball goes.*/

				// Bounce the opposite way on the X axis
			ball->dx = -ball->dx;

			// Deflect slightly up or down depending on where ball hit bat
			ball->dy += difference_y / 128;

			// Limit the Y component of the vector so we don't get into a situation where the ball is bouncing
			// up and down too rapidly
			ball->dy = SDL_min(SDL_max(ball->dy, -1), 1);

			// Ensure our direction vector is a unit vector, i.e.represents a distance of the equivalent of
			// 1 pixel regardless of its angle
			float hypot = SDL_sqrtf(ball->dx * ball->dx + ball->dy * ball->dx);
			ball->dx = ball->dx / hypot;
			ball->dy = ball->dy / hypot;

			

			//   Create an impact effect
			// TODO game.impacts.append(Impact((self.x - new_dir_x * 10, self.y)))

			// Increase speed with each hit
			ball->speed += 15;

			// Add an offset to the AI player's target Y position, so it won't aim to hit the ball exactly
			//  in the centre of the bat
			game->ai_offset = SDL_randf() * 20 - 10;

			//  Bat glows for 10 frames
			bat->timer = 10;

			//  Play hit sounds, with more intense sound effects as the ball gets faster
			//  play every time in addition to 
			soundPlay("hit0.ogg");
			if (ball->speed <= 250)
				soundPlay("hit_slow0.ogg");
			else if (ball->speed <= 300)
				soundPlay("hit_medium0.ogg");
			else if (ball->speed <= 350)
				soundPlay("hit_fast0.ogg");
			else
				soundPlay("hit_veryfast0.ogg");
		}
	}

	// The top and bottom of the arena are 220 pixels from the centre
	if (SDL_fabsf(ball->y - game->HALF_HEIGHT) > 220)
	{
		// Invert vertical direction and apply new dy to y so that the ball is no longer overlapping with the
		// edge of the arena
		ball->dy = -ball->dy;
		ball->y = original_y;
		ball->y += ball->dy;

		// Create impact effect
		// TODO  game.impacts.append(Impact(self.pos))

		// Sound effect
		soundPlay("bounce0.ogg");
		//soundPlay("bounce_synth0.ogg");
	}

}

