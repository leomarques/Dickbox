#include "Stage.h"

Stage::Stage()
{
    world = new World();
    freeDraw = new FreeDraw();
    customBox = new CustomBox();
    customPolygon = new CustomPolygon();
    customCircle = new CustomCircle();

    debugDrawOn = false;
    smallBodiesOn = false;
    cursorOn = true;
    autoDumpOn = false;
    bmpDrawOn = true;
    cleanModeOn = false;

    dt = gtime;

    bodyType = Random;
    gravity.x = 0.0f;
    gravity.y = -10.0f;

    bodiesSize = RANDBODYSIZE;
    bodiesRadius = RANDHALFSIZE;

    setMouseLock(false);

    createGround();
}

Stage::~Stage()
{
    delete world;
    delete freeDraw;
    delete customBox;
    delete customCircle;
    delete customPolygon;
}

bool Stage::step(void)
{
    updateInput();

    menuOn = false;

    if (keys[KEYESC])
        return false;

    if (mouse[0] or (autoDumpOn and (bodyType == Random or bodyType == Box or bodyType == Circle)))
    {
        bodiesSize = smallBodiesOn ? SMALLBODYSIZE : RANDBODYSIZE;
        bodiesRadius = smallBodiesOn ? SMALLHALFSIZE : RANDHALFSIZE;

        switch (bodyType)
        {
        case Random:
            if (RANDOM(0, 1))
                world->makeBox(coordAllegToB2(Point(mouse_x, mouse_y)), bodiesSize);
            else
                world->makeCircle(coordAllegToB2(Point(mouse_x, mouse_y)), bodiesRadius);
            break;

        case Box:
            world->makeBox(coordAllegToB2(Point(mouse_x, mouse_y)), bodiesSize);
            break;

        case Circle:
            world->makeCircle(coordAllegToB2(Point(mouse_x, mouse_y)), bodiesRadius);
            break;

        case Free_Draw:
            freeDraw->takePoint(Point(mouse_x, mouse_y));
            break;

        case Custom_Polygon:
            customPolygon->takePoint(Point(mouse_x, mouse_y));
            break;

        case Custom_Box:
            customBox->takePoint(Point(mouse_x, mouse_y));
            break;

        case Custom_Circle:
            customCircle->takePoint(Point(mouse_x, mouse_y));
            break;
        }
    }
    else
    {
        if (freeDraw->On)
            freeDraw->makeBody(world);
        if (customBox->On)
            customBox->makeBody(world);
        if (customCircle->On)
            customCircle->makeBody(world);
        if (customPolygon->On)
            customPolygon->updateView(Point(mouse_x, mouse_y));
    }

    if (mouse[1])
    {
        if (customPolygon->On)
            customPolygon->makeBody(world);
        else
            world->makeBomb(coordAllegToB2(Point(mouse_x, mouse_y)));
    }

    if (keys[KEY1])
        bodyType = Random;

    if (keys[KEY2])
        bodyType = Box;

    if (keys[KEY3])
        bodyType = Circle;

    if (keys[KEY4])
    {
        bodyType = Free_Draw;
        setMouseLock(false);
    }

    if (keys[KEY5])
    {
        bodyType = Custom_Box;
        setMouseLock(false);
    }

    if (keys[KEY6])
        world->destroyLastBody();

    if (keys[KEY7])
        world->toggleStaticMode();

    if (keys[KEY8])
        world->toggleSimulation();

    if (keys[KEY9])
        if (bodyType == Random or bodyType == Box or bodyType == Circle)
            toggleMouseLock();

    if (keys[KEY0])
    {
        world->destroyAllBodies();
        createGround();
    }

    if (keys[KEYP])
        pyramidShow();

    if (keys[KEYTAB])
        menuOn = true;

    if (keys[KEYH])
        cleanModeOn = !cleanModeOn;

    if (keys[KEYA])
        autoDumpOn = !autoDumpOn;

    if (keys[KEYC])
        cursorOn = !cursorOn;

    if (keys[KEYX])
    {
        bodyType = Custom_Circle;
        setMouseLock(false);
    }

    if (keys[KEYV])
    {
        bodyType = Custom_Polygon;
        setMouseLock(true);
    }

    if (keys[KEYN])
        world->destroyLastNonStaticBody();

    if (keys[KEYM])
        world->destroyAllNonStaticBodies();

    if (keys[KEYS])
        smallBodiesOn = !smallBodiesOn;

    if (keys[KEYUP])
    {
        gravity.y += 2;
        world->setGravity(gravity);
    }

    if (keys[KEYDOWN])
    {
        gravity.y -= 2;
        world->setGravity(gravity);
    }

    if (keys[KEYLEFT])
    {
        gravity.x -= 2;
        world->setGravity(gravity);
    }

    if (keys[KEYRIGHT])
    {
        gravity.x += 2;
        world->setGravity(gravity);
    }

    if (keys[KEYD])
    {
        if (debugDrawOn)
        {
            if (bmpDrawOn)
            {
                bmpDrawOn = false;
            }
            else
            {
                bmpDrawOn = true;
                toggleDebugDraw();
            }
        }
        else
        {
            toggleDebugDraw();
        }
    }

    world->simulate();

    return true;
}

void Stage::render(BITMAP *buffer)
{
    clear_to_color(buffer, makecol(75, 75, 75));
    if (bmpDrawOn)
        for (vector<Body*>::iterator it = world->bodyList.begin(); it != world->bodyList.end(); ++it)
        {
            Body *b = *it;

            BITMAP *bmp = b->bmp;
            if (!bmp)
                continue;

            Point p = b->getAllegPosition();
            int x = p.x - (bmp->w / 2);
            int y = p.y - (bmp->h / 2);

            rotate_sprite(buffer, bmp, x, y, b->getAllegAngle());
        }

    if (freeDraw->On)
        draw_sprite(buffer, freeDraw->bmp, 0, 0);

    if (customBox->On)
        draw_sprite(buffer, customBox->bmp, 0, 0);

    if (customCircle->On)
        draw_sprite(buffer, customCircle->bmp, 0, 0);

    if (customPolygon->On)
        draw_sprite(buffer, customPolygon->bmp, 0, 0);

    /*************************************************************************************************/
    //Menu
    if (!cleanModeOn)
    {
        textprintf_centre_ex(buffer, font, SCREEN_W / 2, 40, GREEN, -1, "Hold TAB for menu");

        textprintf_ex(buffer, font, 10, 15, GRAY, -1, "Body type:");
        switch (bodyType)
        {
        case Random:
            textprintf_ex(buffer, font, 100, 15, BLUE, -1, "Random");
            break;

        case Box:
            textprintf_ex(buffer, font, 100, 15, YELLOW, -1, "Box");
            break;

        case Circle:
            textprintf_ex(buffer, font, 100, 15, YELLOW, -1, "Circle");
            break;

        case Free_Draw:
            textprintf_ex(buffer, font, 100, 15, PURPLE, -1, "Free Draw");
            break;

        case Custom_Box:
            textprintf_ex(buffer, font, 100, 15, PURPLE, -1, "Custom Box");
            break;

        case Custom_Circle:
            textprintf_ex(buffer, font, 100, 15, PURPLE, -1, "Custom Circle");
            break;

        case Custom_Polygon:
            textprintf_ex(buffer, font, 100, 15, PURPLE, -1, "Custom Polygon");
            break;

        default:
            break;
        }

        textprintf_ex(buffer, font, 230, 15, GRAY, -1, "Static mode:");
        if (world->staticModeOn)
            textprintf_ex(buffer, font, 340, 15, GREEN, -1, "On");
        else
            textprintf_ex(buffer, font, 340, 15, RED, -1, "Off");

        textprintf_ex(buffer, font, 400, 15, GRAY, -1, "Mouse lock:");
        if (mouseLockOn)
            textprintf_ex(buffer, font, 500, 15, GREEN, -1, "On");
        else
            textprintf_ex(buffer, font, 500, 15, RED, -1, "Off");

        textprintf_ex(buffer, font, 600, 15, GRAY, -1, "Gravity: %.2f, %.2f", gravity.x, gravity.y);

        if (!world->simulateOn)
            textprintf_centre_ex(buffer, font, SCREEN_W / 2, 65, YELLOW, -1, "PAUSED");

        textprintf_ex(buffer, font, 10, 40, GRAY, -1, "Bodies:");
        textprintf_ex(buffer, font, 70, 40, world->bodyList.size() > 400 ? RED : GRAY, -1, "%d", world->bodyList.size());

        dt = (int) (gtime - dt);
        if (dt == 0) dt = 1;
        int fps = 60 / dt;
        dt = gtime;

        textprintf_ex(buffer, font, 10, 65, GRAY, -1, "Approx. FPS:");
textprintf_ex(buffer, font, 110, 65, fps < 60 ? fps < (60 / 2) ? RED : YELLOW : GREEN, -1, "%d", fps);

        if (menuOn)
        {
            textprintf_ex(buffer, font, 10, 85, WHITE, -1, "Press keys 0-9 to choose options");
            textprintf_ex(buffer, font, 10, 100, WHITE, -1, "0 : Reset");
            textprintf_ex(buffer, font, 10, 110, WHITE, -1, "1 : Random bodies");
            textprintf_ex(buffer, font, 10, 120, WHITE, -1, "2 : Box");
            textprintf_ex(buffer, font, 10, 130, WHITE, -1, "3 : Circle");
            textprintf_ex(buffer, font, 10, 140, WHITE, -1, "4 : Free draw");
            textprintf_ex(buffer, font, 10, 150, WHITE, -1, "5 : Custom Box");
            textprintf_ex(buffer, font, 10, 160, WHITE, -1, "6 : Destroy last body");
            textprintf_ex(buffer, font, 10, 170, WHITE, -1, "7 : Toggle static mode");
            textprintf_ex(buffer, font, 10, 180, WHITE, -1, "8 : Pause");
            textprintf_ex(buffer, font, 10, 190, WHITE, -1, "9 : Toggle Mouse lock");
            textprintf_ex(buffer, font, 10, 205, WHITE, -1, "MB1 : Make bodies");
            textprintf_ex(buffer, font, 10, 215, WHITE, -1, "MB2 : Make bombs");

            textprintf_ex(buffer, font, 10, 235, WHITE, -1, "H : Hide all text");
            textprintf_ex(buffer, font, 10, 245, WHITE, -1, "C : Toggle cursor");
            textprintf_ex(buffer, font, 10, 255, WHITE, -1, "A : Auto dump bodies");
            textprintf_ex(buffer, font, 10, 265, WHITE, -1, "N : Destroy last non-static body");
            textprintf_ex(buffer, font, 10, 275, WHITE, -1, "M : Destroy all non-static body");
            textprintf_ex(buffer, font, 10, 285, WHITE, -1, "S : Toggle small bodies");
            textprintf_ex(buffer, font, 10, 295, WHITE, -1, "V : Custom polygon");
            textprintf_ex(buffer, font, 10, 305, WHITE, -1, "X : Custom circle");
            textprintf_ex(buffer, font, 10, 315, WHITE, -1, "P : Pyramid show");
            textprintf_ex(buffer, font, 10, 325, WHITE, -1, "Arrow keys : Adjust gravity");
        }
    }
    /*************************************************************************************************/

    if (cursorOn)
        draw_sprite(buffer, mouse_sprite, mouse_x, mouse_y);
}

void Stage::toggleDebugDraw(void)
{
    world->setDebugDraw(debugDrawOn ? NULL : debugDraw);
    debugDrawOn = !debugDrawOn;
}

void Stage::createGround(void)
{
    world->staticModeOn = true;
    world->makeBox(coordAllegToB2(Point(SCREEN_W / 2, SCREEN_H - 15)), b2Vec2(6.0f, 0.15f));
    world->staticModeOn = false;
}

void Stage::pyramidShow(void)
{
    float32 size = 0.2f;
    int rows = 15;

    float32 x = -(size * rows) + size, y = 5.0f;

    for (int i = rows; i > 0; i--, x += size, y += (size * 2))
    {
        float32 xl = x;
        for (int j = 0; j < i; j++, xl += (size * 2))
            world->makeBox(b2Vec2(xl, y), b2Vec2(size, size));
    }

    world->makeBomb(b2Vec2(-0.5f, 0.3f));
    world->makeBomb(b2Vec2(0.5f, 0.3f));
    world->makeBomb(b2Vec2(0.0f, 0.3f));
}
