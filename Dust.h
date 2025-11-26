#pragma once
#include "Point.h"
#include "Color.h"
#include "Painter.h"

class Dust {
    private:
        Point dustVelocity;
        Point dustCenter;
        Color dustColor;
        double dustRadius;
        double lifeTimer;

    public:
        Dust(Point center, double radius, Point velocity, Color color, double lifetimer);
        void update(double tick);
        bool isDead() const;
        void draw(Painter& painter) const;
};