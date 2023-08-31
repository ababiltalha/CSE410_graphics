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
};


#endif // COLOR_HPP