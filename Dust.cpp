#include "Dust.h"

Dust::Dust(Point center, double radius, Point velocity, Color color,double lifetimer) {
    dustCenter = center;
    dustVelocity = velocity;
    dustRadius = radius;
    dustColor = color;
    lifeTimer = lifetimer;
}

void Dust::update(double tick) {
    dustCenter = dustCenter + dustVelocity * tick;
    lifeTimer-=tick;
}

bool Dust::isDead() const {
    return lifeTimer<= 0.0;
}

void Dust::draw(Painter& painter) const {
    painter.draw(dustCenter, dustRadius, dustColor);
}