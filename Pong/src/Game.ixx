export module Game;
import Engine;
import std;
import Input;
using namespace std;

export
class Game
{
public:
	virtual void update(float elapsedSeconds) = 0;
	virtual void draw() const = 0;
	virtual void handleInput(const Input& input) = 0;
	virtual string getName() const = 0;
	virtual ~Game() = default;
};