export module Pong;
import Engine;
import Game;
import Input;
import std;
using namespace std;

export class GamePong;

class GameState : Game
{
protected:
	GamePong* game_{ nullptr };

public:
	void setContext(GamePong*);
	// Inherited via Game
	void update(float elapsedSeconds) override;
	void draw() const override;
	string getName() const override = 0;
	void handleInput(const Input& input) override;
};

class GameStatePlay : public GameState
{
public:
	inline void update(float elapsedSeconds) override;
	inline void draw() const override;
	~GameStatePlay() override;
	string getName() const override { return "StatePlay"; }
	void handleInput(const Input& input) override;
};

class GameStateGameOver : public GameState
{
public:
	inline void draw() const override;
	inline void update(float elapsedSeconds) override;
	~GameStateGameOver() override;
	string getName() const override { return "StateOver"; }
	void handleInput(const Input& input) override;;
};

class GameStateMenu final : public GameState
{
public:
	// Inherited via GameState
	void draw() const override;
	~GameStateMenu() override;
	inline void update(float elapsedSeconds) override;
	string getName() const override { return "StateMenu"; }
	void handleInput(const Input& input) override;;
};

class GamePong final : public Game
{
public:
	inline static unique_ptr<Engine> engine;
	static constexpr int WIDTH{ 800 };
	static constexpr int HEIGHT{ 480 };
	GamePong();
	// Inherited via Game
	void update(float elapsedSeconds) override;
	void draw() const override;
	~GamePong() override;
	void TransitionTo(unique_ptr<GameState> newState);
	void SetNumplayers(int numPlayers);
	int GetNumplayers() const { return num_players_; }
	string getName() const override { return "GamePong"; }
	void handleInput(const Input& input) override;

private:
	unique_ptr<GameState> state_ = nullptr;
	int num_players_ = 1;
};

module :private;

void GameState::setContext(GamePong* g)
{
	game_ = g;
}

// Inherited via Game
inline void GameState::update(float elapsedSeconds)
{
}

inline void GameState::draw() const
{
}


void GameState::handleInput(const Input& input)
{
}


void GameStatePlay::update(float elapsedSeconds)
{

}

void GameStatePlay::draw() const
{
	game_->engine->drawText(getName(), { 20,20 });
}

GameStatePlay::~GameStatePlay()
{
	println("{} dtor", GameStatePlay::getName());
}


void GameStatePlay::handleInput(const Input& input)
{
}


void GameStateGameOver::draw() const
{
	game_->engine->draw("over", { 0,0 });
	game_->engine->drawText(getName(), { 20,20 });
}

void GameStateGameOver::update(float elapsedSeconds)
{

}

GameStateGameOver::~GameStateGameOver()
{
	println("{} dtor", GameStateGameOver::getName());
}

// Inherited via Game
inline void GamePong::update(float elapsedSeconds)
{
	state_->update(elapsedSeconds);
}

inline void GamePong::draw() const
{
	engine->draw("table", { 0, 0 });
	state_->draw();
}

GamePong::~GamePong()
{
	println("{} dtor", getName());
}

void GamePong::TransitionTo(unique_ptr<GameState> newState)
{
	println("GamePong.TransitionTo: {} ---> {}",
		state_ == nullptr ? "None" : state_->getName(),
		newState->getName()
	);
	state_.reset();
	newState->setContext(this);
	state_ = move(newState);
}


void GamePong::SetNumplayers(int numPlayers)
{
	num_players_ = numPlayers;
}


void GamePong::handleInput(const Input& input)
{
	state_->handleInput(input);
}


GamePong::GamePong()
{
	TransitionTo(make_unique<GameStateMenu>());
}


void GameStateGameOver::handleInput(const Input& input)
{
}


// Inherited via GameState
inline void GameStateMenu::draw() const
{
	auto image = format("menu{}", game_->GetNumplayers() - 1);
	game_->engine->draw(image, { 0,0 });
	game_->engine->drawText(getName(), { 20, 20 });
}

inline GameStateMenu::~GameStateMenu()
{
	println("{} dtor", getName());
}

void GameStateMenu::update(float elapsedSeconds)
{

}


void GameStateMenu::handleInput(const Input& input)
{
	if (input.ArrowUpPressed && game_->GetNumplayers() == 2)
	{
		game_->SetNumplayers(1);
		game_->engine->play("up");
	}
	else if (input.ArrawDownPressed && game_->GetNumplayers() == 1)
	{
		game_->SetNumplayers(2);
		game_->engine->play("down");
	}
}
