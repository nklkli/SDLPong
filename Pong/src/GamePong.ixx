export module Pong;
import Engine;
import Game;
import std;
using namespace std;

class GamePong;

class GameState :Game {
protected:
	GamePong* game_{ nullptr };

public:
	void setContext(GamePong*);
	// Inherited via Game
	void update(Engine* const engine , float elapsedSeconds) override;
	void draw(Engine* const engine) const override;
	void handleInput(Engine* const engine) override;
};


class GameStateMenu :public GameState {
public:
	// Inherited via GameState
	void draw(Engine* const engine) const override;
	~GameStateMenu();
};


export
class GamePong : public Game
{
public:
	friend class GameStateMenu;
	GamePong();
	// Inherited via Game
	void update(Engine* const engine, float elapsedSeconds) override;
	void draw(Engine* const engine) const override;
	void handleInput(Engine* const engine) override;
	~GamePong();
private:
	unique_ptr<GameState> state_ =  nullptr ;
	void TransitionTo(unique_ptr<GameState> newState);

};




module :private;



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

inline void GameState::handleInput(Engine* const engine)
{
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

inline void GamePong::handleInput(Engine*const engine)
{
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
