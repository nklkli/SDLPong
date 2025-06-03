export module Paddle;
import  Actor;


export
class Paddle:public Actor
{
public:
	Paddle(const ::std::string& image, const ::Point& p)
		: Actor(image, p)
	{
	}


	~Paddle() override;
	void update(float time) override;
}


Paddle::~Paddle()
{
}


void Paddle::update(float time)
{
	Actor::update(time);
}
