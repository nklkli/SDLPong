export module Game;
import Engine;
import std;
import Input;
using namespace std;

export
class Game
{
protected:
	string name_;
public:
	virtual void update(float elapsedSeconds) = 0;
	virtual void draw() const = 0;
	virtual void handleInput(const Input& input) = 0;
	string getName() const
	{
		auto className = typeid(*this).name();
		return format("{}{}", 
			className, 
			name_.empty() ? "": " name=(" + name_ + ")");
	}
	virtual ~Game() = default;

};