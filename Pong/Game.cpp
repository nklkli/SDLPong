#include "Game.h"
#include <print>

void Game::init(Engine* e)
{
	engine.reset(e);
}

void Game::update(float elapsedSeconds)
{

}

void Game::draw() const
{
	engine->draw("ball2", 0, 0);
}

Game::~Game()
{
	std::print("Game dtor\n");
}

Actor::~Actor()
{
	std::print("Actor dtor\n");
}
