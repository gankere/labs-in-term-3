#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"

class Ball {
private:
    Velocity ballVelocity;
    Point ballCenter;
    Color ballColor;
    double ballRadius;
    bool isCollidable;

public:
    Ball(Point center, double radius, Velocity velocity, Color color, bool collusion);
    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;
    void draw(Painter& painter) const;
    void setCenter(const Point& center);
    Point getCenter() const;
    double getRadius() const;
    double getMass() const;
    bool getCollision() const;
};
