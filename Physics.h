#pragma once
#include "Ball.hpp"
#include "Dust.h"
#include <vector>

class Physics {
  public:
    Physics(double timePerTick = 0.001);
    void setWorldBox(const Point& topLeft, const Point& bottomRight);
    void update(std::vector<Ball>& balls, std::vector<Dust>& dusts, size_t ticks) const;

  private:
    void collideBalls(std::vector<Ball>& balls, std::vector<Dust>& dusts) const;
    void collideWithBox(std::vector<Ball>& balls) const;
    void move(std::vector<Ball>& balls) const;
    void processCollision(Ball& a, Ball& b, double distanceBetweenCenters2, std::vector<Dust>& dusts) const;

  private:
    Point topLeft;
    Point bottomRight;
    double timePerTick;
};
