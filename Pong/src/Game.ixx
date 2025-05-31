export module Game;
import Engine;
import std;
import Command;
using namespace std;

export
class Game
{
public:
	virtual void update(Command& command, Engine& , float elapsedSeconds) = 0;
	virtual void draw(Engine* ) const = 0;
	virtual ~Game() = default;
};


