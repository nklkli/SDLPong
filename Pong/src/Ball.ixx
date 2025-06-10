module;
export module Ball;
import Entity;

export
class Ball :public Entity {
public:
	float x{}, y{};
	void draw() const override;
	void update(float elapsedSeconds) override;
	bool out() const;
	~Ball() override;
};





void Ball::draw() const
{
	Entity::draw();
}


void Ball::update(float elapsedSeconds)
{
	Entity::update(elapsedSeconds);
}


bool Ball::out() const
{
	return false;
}


Ball::~Ball()
{
}
