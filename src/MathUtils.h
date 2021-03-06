#ifndef MATHUTILS_H_INCLUDED
#define MATHUTILS_H_INCLUDED

#include <cmath>

#define PI 3.14159265

class Point
{
public:
    int x, y;

    Point()
    {
        x = y = 0;
    }

    Point(int x, int y) : x(x), y(y) {}

    void operator=(Point p)
    {
        x = p.x;
        y = p.y;
    }

    Point operator-(Point p)
    {
        return Point(x - p.x, y - p.y);
    }

    bool operator==(Point p)
    {
        return x == p.x && y == p.y;
    }
};

inline int myPow(int x, int p)
{
	int r = 1;
    for (int i = 0; i < p; i++)
        r *= x;

    return r;
}

inline double pointDistance(Point p0, Point p1)
{
    return sqrt(myPow(p0.x - p1.x, 2) + myPow(p0.y - p1.y, 2));
}

inline int prodEscAlg(Point p0, Point p1)
{
    return (p0.x * p1.x) + (p0.y * p1.y);
}

inline double vecMod(Point p)
{
    return sqrt(myPow(p.x, 2) + myPow(p.y, 2));
}

inline double vecsCos(Point p0, Point p1)
{
    return prodEscAlg(p0, p1) / (vecMod(p0) * vecMod(p1));
}

#endif // MATHUTILS_H_INCLUDED
