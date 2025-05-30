export module Pong;
import Engine;
import Game;
import std;
using namespace std;

export class GamePong;

class GameState :Game {
protected:
	GamePong* game_{ nullptr };

public:
	void setContext(GamePong*);
	// Inherited via Game
	void update(Engine* const engine , float elapsedSeconds) override;
	void draw(Engine* const engine) const override;
	void handleInput(const Input&) override;
};


class GameStateMenu final :public GameState {
public:
	// Inherited via GameState
	void draw(Engine* const engine) const override;
	~GameStateMenu() override;
	void handleInput(const Input&) override;
};


class GamePong final : public Game
{
public:
	friend class GameStateMenu;
	GamePong();
	// Inherited via Game
	void update(Engine* const engine, float elapsedSeconds) override;
	void draw(Engine* const engine) const override;
	void handleInput(const Input&) override;
	~GamePong() override;
private:
	unique_ptr<GameState> state_ =  nullptr ;
	void TransitionTo(unique_ptr<GameState> newState);

};




module :private;
import Input;



void GameState::setContext(GamePong* g) {
	game_ = g;
}

// Inherited via Game
inline void GameState::update(Engine* const engine, float elapsedSeconds)
{
}

inline void GameState::draw(Engine* const engine) const
{
}

inline void GameState::handleInput(const Input& input)
{
	
}

inline void GameStateMenu::handleInput(const Input& input)
{
	if(input.MouseWheelY!=0)
		println("GameStateMenu; mouse wheel: {:.0f}", input.MouseWheelY);
}

// Inherited via Game
inline void GamePong::update(Engine* const engine, float elapsedSeconds)
{
}

inline void GamePong::draw(Engine* const engine) const
{
	engine->draw("table", { 0,0 });
	state_->draw(engine);
}

inline void GamePong::handleInput(const Input& input)
{
	state_->handleInput(input);
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
	engine->drawText("MENU STATE", { 20,20 });
}

inline GameStateMenu::~GameStateMenu() {
	println("GameStateMenu dtor");
}
