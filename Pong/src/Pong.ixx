export module Pong;
import Engine;
import Actor;
export import Game;
export import Input;
import std;
using namespace std;

export class Pong;



class GameState : public Game
{
protected:
	Pong* game_{ nullptr };

public:
	void setContext(Pong*);
	// Inherited via Game
	void update(float elapsedSeconds) override;
	void draw() const override;

	void handleInput(const Input& input) override;

};

class GameStatePlay : public GameState
{
public:
	inline void update(float elapsedSeconds) override;
	inline void draw() const override;
	~GameStatePlay() override;
	void handleInput(const Input& input) override;
};

class GameStateGameOver : public GameState
{
public:
	inline void draw() const override;
	inline void update(float elapsedSeconds) override;
	~GameStateGameOver() override;
	void handleInput(const Input& input) override;;
};

class GameStateMenu final : public GameState
{
public:
	// Inherited via GameState
	void draw() const override;
	~GameStateMenu() override;
	inline void update(float elapsedSeconds) override;
	void handleInput(const Input& input) override;;
};

class Pong final : public Game
{

	unique_ptr<Engine> engine_;
public:
	static constexpr float WIDTH{ 800 };
	static constexpr float HEIGHT{ 480 };
	static constexpr float HALF_HEIGHT = HEIGHT / 2.0f;
	static constexpr float HALF_WIDTH = WIDTH / 2.0f;
	static constexpr float PADDLE_LEFT_X = 40;
	static constexpr float PADDLE_RIGHT_X = 760;

	float ai_offset{ 0 };
	Actor ball{};

	Pong(unique_ptr<Engine> engine);
	const Engine& GetEngine() const;
	// Inherited via Game
	void update(float elapsedSeconds) override;
	void draw() const override;
	~Pong() override;
	void TransitionTo(unique_ptr<GameState> newState);
	void SetNumplayers(int numPlayers);
	int GetNumplayers() const { return num_players_; }
	void handleInput(const Input& input) override;

private:
	unique_ptr<GameState> state_ = nullptr;
	int num_players_ = 1;
};

module :private;



//def p1_controls() :
//	move = 0
//	if keyboard.z or keyboard.down :
//		move = PLAYER_SPEED
//		elif keyboard.a or keyboard.up :
//		move = -PLAYER_SPEED
//		return move
//
//
//	def p2_controls() :
//	move = 0
//	if keyboard.m :
//		move = PLAYER_SPEED
//		elif keyboard.k :
//		move = -PLAYER_SPEED
//		return move

void GameState::setContext(Pong* g)
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
	game_->GetEngine().drawText(getName(), { 20,20 });
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
	game_->GetEngine().draw("over", { 0,0 });
	game_->GetEngine().drawText(getName(), { 20,20 });
}

void GameStateGameOver::update(float elapsedSeconds)
{

}

GameStateGameOver::~GameStateGameOver()
{
	println("{} dtor", getName());
}

Pong::Pong(unique_ptr<Engine> engine):engine_(move(engine))
{
	name_="hey";
	TransitionTo(make_unique<GameStateMenu>());
}

const Engine& Pong::GetEngine() const
{
	return *engine_.get();
}

// Inherited via Game
inline void Pong::update(float elapsedSeconds)
{
	state_->update(elapsedSeconds);
}

inline void Pong::draw() const
{
	engine_->draw("table", { 0, 0 });
	state_->draw();
}

Pong::~Pong()
{
	println("{} dtor", getName());
}

void Pong::TransitionTo(unique_ptr<GameState> newState)
{
	println("Pong.TransitionTo: {} ---> {}",
		state_ == nullptr ? "None" : state_->getName(),
		newState->getName()
	);
	state_.reset();
	newState->setContext(this);
	state_ = move(newState);
}


void Pong::SetNumplayers(int numPlayers)
{
	num_players_ = numPlayers;
}


void Pong::handleInput(const Input& input)
{
	state_->handleInput(input);
}






void GameStateGameOver::handleInput(const Input& input)
{
}


// Inherited via GameState
inline void GameStateMenu::draw() const
{
	auto image = format("menu{}", game_->GetNumplayers() - 1);
	game_->GetEngine().draw(image, { 0,0 });
	game_->GetEngine().drawText(getName(), { 20, 20 });
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
		game_->GetEngine().play("up");
	}
	else if (input.ArrawDownPressed && game_->GetNumplayers() == 1)
	{
		game_->SetNumplayers(2);
		game_->GetEngine().play("down");
	}
}
