#pragma once
#include <iostream>

class Color {
  public:
    Color();
    Color(double red, double green, double blue);
    double red() const;
    double green() const;
    double blue() const;

    friend std::istream& operator>>(std::istream& stream,Color& color) {
      double red, green, blue;
      stream >> red >> green >> blue;
      color = Color(red, green, blue);
      return stream;
    }

  private:
    double r{};
    double g{};
    double b{};
};
