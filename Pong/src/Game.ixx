export module Game;
import Engine;
import std;
import Input;
using namespace std;

export
class Game
{
public:
	virtual void update(Engine* , float elapsedSeconds) = 0;
	virtual void draw(Engine* ) const = 0;
	virtual void handleInput(const Input&) = 0;
	virtual ~Game() = default;
};


