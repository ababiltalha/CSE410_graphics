#ifndef COLOR_HPP
#define COLOR_HPP

class Color
{
public:
    double r, g, b;

    Color(){
        r = g = b = 0;
    }

    Color(double r, double g, double b){
        this->r = r;
        this->g = g;
        this->b = b;
    }

    Color operator+(Color c){
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator*(double d){
        return Color(r * d, g * d, b * d);
    }
};


#endif // COLOR_HPP