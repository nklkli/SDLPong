#pragma once
#include "Image.h"

class Engine
{
  public:
    virtual void draw(const Image &, const Point&) const = 0;
    virtual void play(const std::string &sound) = 0;
    virtual ~Engine() {};
};
