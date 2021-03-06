#ifndef RENDERUTILS_H_INCLUDED
#define RENDERUTILS_H_INCLUDED

#include <allegro.h>
#include <Box2D.h>

#include "MathUtils.h"
#include "GameUtils.h"

#define RED makecol(255, 0, 0)
#define GREEN makecol(0, 255, 0)
#define BLUE makecol(0, 0, 255)
#define YELLOW makecol(255, 255, 0)
#define PURPLE makecol(225, 0, 220)
#define WHITE makecol(255, 255, 255)
#define GRAY makecol(180, 180, 180)
#define TRANSPARENT makecol(255, 0, 255)
#define RANDOMCOLOR makecol(RANDOM(0, 255), RANDOM(0, 255), RANDOM(0, 255))

inline void drawLine(BITMAP *bmp, Point p0, Point p1, int color)
{
    line(bmp, p0.x, p0.y, p1.x, p1.y, color);
}

inline void drawCircle(BITMAP *bmp, Point p, int radius, int color)
{
    circle(bmp, p.x, p.y, radius, color);
}

inline void drawFilledCircle(BITMAP *bmp, Point p, int radius, int color)
{
    circlefill(bmp, p.x, p.y, radius, color);
}

inline void drawRect(BITMAP *bmp, Point p1, Point p2, int color)
{
    rect(bmp, p1.x, p1.y, p2.x, p2.y, color);
}

inline void drawFilledRect(BITMAP *bmp, Point p1, Point p2, int color)
{
    rectfill(bmp, p1.x, p1.y, p2.x, p2.y, color);
}

inline BITMAP* makeBitmap(int x, int y, int color)
{
    BITMAP *b = create_bitmap(x, y);
    if (!b)
        exit(2);
    clear_to_color(b, color);
    return b;
}

inline BITMAP* createBoxBitmap(const b2Vec2 dimensions, const int color)
{
    BITMAP* bmp = makeBitmap((int) (dimensions.x * 2 * SCALE), (int) (dimensions.y * 2 * SCALE), TRANSPARENT);

    drawFilledRect(bmp, Point(0, 0), Point(bmp->w - 1, bmp->h - 1), color);

    return bmp;
}

inline BITMAP* createCircleBitmap(const float32 radius, const int color)
{
    BITMAP* bmp = makeBitmap((int) (radius * 2 * SCALE) + 1, (int) (radius * 2 * SCALE) + 1, TRANSPARENT);

    drawFilledCircle(bmp, Point((int) (radius * SCALE), (int) (radius * SCALE)), (int) (radius * SCALE), color);
    line(bmp, bmp->w / 2, bmp->h / 2, bmp->w / 2, 0, color);

    return bmp;
}

#endif // RENDERUTILS_H_INCLUDED
