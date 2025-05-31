export module Pong;
import Engine;
import Game;
import Command;
import std;
using namespace std;

export class GamePong;

class GameState : Game
{
protected:
	GamePong* game_{nullptr};

public:
	void setContext(GamePong*);
	// Inherited via Game
	void update(Command& command, Engine& engine, float elapsedSeconds) override;
	void draw(Engine*  engine) const override;
};

class GameStateGameOver : public GameState
{
public:
	inline void draw(Engine* engine) const override;
	inline void update(Command& command, Engine& engine, float elapsedSeconds) override;
	~GameStateGameOver() override;
};


class GameStateMenu final : public GameState
{
public:
	// Inherited via GameState
	void draw(Engine*  engine) const override;
	~GameStateMenu() override;
	inline void update(Command& command, Engine& engine, float elapsedSeconds) override;
};


class GamePong final : public Game
{
public:
	GamePong();
	// Inherited via Game
	void update(Command& command, Engine& engine, float elapsedSeconds) override;
	void draw(Engine*  engine) const override;
	~GamePong() override;
	void TransitionTo(unique_ptr<GameState> newState);

private:
	unique_ptr<GameState> state_ = nullptr;
};


module :private;



void GameState::setContext(GamePong* g)
{
	game_ = g;
}

// Inherited via Game
inline void GameState::update(Command& command, Engine& engine, float elapsedSeconds)
{

}

inline void GameState::draw(Engine* const engine) const
{
}


void GameStateGameOver::draw(Engine* engine) const
{
	engine->draw("over", { 0,0 });
	engine->drawText("GAME OVER", { 20,20 });
}

void GameStateGameOver::update(Command& command, Engine& engine, float elapsedSeconds)
{
	if (command.NextState)
	{
		command.NextState = false;
		game_->TransitionTo(make_unique<GameStateMenu>());
	}
}

GameStateGameOver::~GameStateGameOver()
{
	println("GameOver dtor");
}


// Inherited via Game
inline void GamePong::update(Command& command, Engine& engine, float elapsedSeconds)
{
	state_->update(command, engine, elapsedSeconds);
}

inline void GamePong::draw(Engine* const engine) const
{
	engine->draw("table", {0, 0});
	state_->draw(engine);
}


GamePong::~GamePong()
{
	println("GamePong dtor");
}

void GamePong::TransitionTo(unique_ptr<GameState> newState)
{
	if (state_)
		state_.reset();
	newState->setContext(this);
	state_ = move(newState);
}


GamePong::GamePong()
{
	TransitionTo(make_unique<GameStateMenu>());
}

// Inherited via GameState
inline void GameStateMenu::draw(Engine* const engine) const
{
	engine->drawText("MENU STATE", {20, 20});
}

inline GameStateMenu::~GameStateMenu()
{
	println("GameStateMenu dtor");
}

void GameStateMenu::update(Command& command, Engine& engine, float elapsedSeconds)
{
	if (command.NextState)
	{
		command.NextState = false;
		game_->TransitionTo(make_unique<GameStateGameOver>());
	}
}
