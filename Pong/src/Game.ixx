export module Game;
import Engine;
import std;
using namespace std;

export
class Game
{
public:
	virtual void update(Engine* const, float elapsedSeconds) = 0;
	virtual void draw(Engine* const) const = 0;
	virtual void handleInput(Engine* const) = 0;
	virtual ~Game() = default;
};


