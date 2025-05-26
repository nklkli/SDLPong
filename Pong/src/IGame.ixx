export module Game;

export
struct IGame
{
	virtual void update(float elapsedSeconds) = 0;
	virtual void draw() const = 0;
	virtual ~IGame() = default;
};

