#include "WorldMap.h"
#include "World.h"

extern World world;

WorldMap::WorldMap()
{
    //ctor
}

WorldMap::~WorldMap()
{
    //dtor
}

void WorldMap::init(string configFile)
{
    srand(time(NULL));

    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;
    string nextButtonImg;
    string armyTestImg;
    string borderImg;

    stream.open(configFile.c_str());

    stream >> tmp >> nextButtonImg;
    stream >> tmp >> nextButtonRect.x >> nextButtonRect.y >> nextButtonRect.w >> nextButtonRect.h;
    stream >> tmp >> zoomMulti;
    stream >> tmp >> maxZoom.w >> maxZoom.h;
    stream >> tmp >> minZoom.w >> minZoom.h;
    stream >> tmp >> allImages;
    stream >> tmp >> imagesPerChunk;
    stream >> tmp >> armyTestImg;
    stream >> tmp >> army.objRect.x >> army.objRect.y >> army.objRect.w >> army.objRect.h;
    stream >> tmp >> borderImg;
    stream >> tmp >> speed;

    stream.close();

    NextButtonTexture = LoadTexture(nextButtonImg, world.m_main_renderer);
    army.objTexture = LoadTexture(armyTestImg, world.m_main_renderer);
    m_selectedArmy.objTexture = LoadTexture(borderImg, world.m_main_renderer);

    cameraRect.x = 0;
    cameraRect.y = 0;
    cameraRect.w = world.m_SCREEN_WIDTH;
    cameraRect.h = world.m_SCREEN_HEIGHT;

    cameraPosBeforeDrag = world.m_mouse;

    army.dstRect = army.objRect;
    army.coor = army.objRect;
}

void WorldMap::loadMap(string configFile)
{
    srand(time(NULL));

    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    vector <string> MapImg;
    string buff;

    stream.open(configFile.c_str());

    stream >> tmp >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);

    stream.close();

    for(int i = 0; i < 16; i ++)
    {
        mapPieces.push_back(LoadTexture("WorldMap\\" + MapImg[i], world.m_main_renderer));
    }
}

void WorldMap::update()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_PollEvent(&(world.m_event));

    oldX = cameraRect.x;
    oldY = cameraRect.y;

    zoom();

    if(world.m_mouseIsPressed)
    {
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, nextButtonRect)/*state[SDL_SCANCODE_ESCAPE] && world.m_buttonDown*/)
        {
            world.m_quitScene = true;
            world.m_gameState = MENU;
        }
    }
    if (state[SDL_SCANCODE_ESCAPE] && world.m_buttonDown)
    {
        world.m_quitScene = true;
        world.m_gameState = MENU;
    }

    framer();

    moveWithMouse();

    ///openCity(army.objRect);

    updateArmy(&army);
}

void WorldMap::draw()
{
    SDL_RenderClear(world.m_main_renderer);

    drawMap();

    SDL_RenderCopy(world.m_main_renderer, NextButtonTexture, NULL, &(nextButtonRect));

    drawArmy(&army);

    if(borderActive == true && !world.m_drag)
    {
        SDL_RenderCopy(world.m_main_renderer, m_selectedArmy.objTexture, NULL, &(m_selectedArmy.objRect));
    }

    SDL_RenderPresent(world.m_main_renderer);
}

void WorldMap::framer()
{
    if(cameraRect.w < minZoom.w || cameraRect.h < minZoom.h)
    {
        cameraRect.x = oldX;
        cameraRect.y = oldY;
        cameraRect.w = minZoom.w;
        cameraRect.h = minZoom.h;
    }
    if(cameraRect.w > maxZoom.w || cameraRect.h > maxZoom.h)
    {
        cameraRect.x = oldX;
        cameraRect.y = oldY;
        cameraRect.w = maxZoom.w;
        cameraRect.h = maxZoom.h;
    }
    if(cameraRect.x + cameraRect.w > 7680)
    {
        cameraRect.x = 7680 - cameraRect.w;
    }
    if(cameraRect.x < 0)
    {
        cameraRect.x = 0;
    }
    if(cameraRect.y + cameraRect.h > 4320)
    {
        cameraRect.y = 4320 - cameraRect.h;
    }
    if(cameraRect.y < 0)
    {
        cameraRect.y = 0;
    }
}

void WorldMap::zoom()
{
    if(world.m_event.type == SDL_MOUSEWHEEL)
    {
        float w_old = cameraRect.w;
        float h_old = cameraRect.h;

        cameraRect.w -= zoomMulti * world.m_event.wheel.y * 16.0;
        cameraRect.h -= zoomMulti * world.m_event.wheel.y * 9.0;

        cameraRect.x += (w_old - cameraRect.w) * (world.m_mouse.x / (1.0 * world.m_SCREEN_WIDTH));
        cameraRect.y += (h_old - cameraRect.h) * (world.m_mouse.y / (1.0 * world.m_SCREEN_HEIGHT));
    }
}

void WorldMap::drawMap()
{
    SDL_Rect src = {0, 0, 1920, 1080};

    for (int i = 0; i < allImages; i++)
    {
        SDL_Rect world_space  = {1920 * (i % imagesPerChunk), 1080 * (i / imagesPerChunk), 1920, 1080};

        SDL_Rect screen_space =
        {
            zoom_lvl * (world_space.x - cameraRect.x),
            zoom_lvl * (world_space.y - cameraRect.y),
            zoom_lvl * world_space.w,
            zoom_lvl * world_space.h
        };

        SDL_RenderCopy(world.m_main_renderer, mapPieces[i], &src, &screen_space);
    }
}

void WorldMap::moveWithMouse()
{
    if(world.m_drag)
    {
        mouseDragDistance.x = world.m_mouse.x - currentPos.x;
        mouseDragDistance.y = world.m_mouse.y - currentPos.y;

        cameraRect.x = cameraPosBeforeDrag.x - mouseDragDistance.x;
        cameraRect.y = cameraPosBeforeDrag.y - mouseDragDistance.y;
    }
    else if(!world.m_drag)
    {
        currentPos.x = world.m_mouse.x;
        currentPos.y = world.m_mouse.y;

        cameraPosBeforeDrag.x = cameraRect.x;
        cameraPosBeforeDrag.y = cameraRect.y;
    }
}

void WorldMap::openCity(SDL_Rect cityRect)
{
    SDL_Rect cityScreenRect;

    cityScreenRect = cityRect;

    cityScreenRect.x = cityRect.x - cameraRect.x;
    cityScreenRect.y = cityRect.y - cameraRect.y;

    if(world.m_mouseIsPressed)
    {
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, cityScreenRect))
        {
            cout << "Clicked: " << world.m_mouse.x << " " << world.m_mouse.y << endl;
        }
    }
}

void WorldMap::drawArmy(mapObject* army)
{
    SDL_Rect screen_space =
    {
        zoom_lvl * (army -> objRect.x - cameraRect.x),
        zoom_lvl * (army -> objRect.y - cameraRect.y),
        zoom_lvl * army -> objRect.w,
        zoom_lvl * army -> objRect.h
    };

    if(world.m_mouseIsDoubleClicked && checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, screen_space))
    {
        m_selectedArmy.objRect.x = (screen_space.x - 5);
        m_selectedArmy.objRect.y = (screen_space.y - 5);
        m_selectedArmy.objRect.w = (screen_space.w + 10);
        m_selectedArmy.objRect.h = (screen_space.h + 10);

        borderActive = true;
    }
    if(zoom_lvl < 0.6)
    {
        SDL_RenderCopy(world.m_main_renderer, NextButtonTexture, NULL, &(screen_space));
    }
    else if(zoom_lvl > 1)
    {
        SDL_RenderCopy(world.m_main_renderer, m_selectedArmy.objTexture, NULL, &(screen_space));
    }
    else
    {
        SDL_RenderCopy(world.m_main_renderer, army -> objTexture, NULL, &(screen_space));
    }
}

void WorldMap::updateArmy(mapObject* army)
{
    zoom_lvl = world.m_SCREEN_WIDTH / (0.0 + cameraRect.w);

    SDL_Rect armyScreenRect;

    armyScreenRect = army -> objRect;

    armyScreenRect.x = (army -> objRect.x - cameraRect.x);
    armyScreenRect.y = (army -> objRect.y - cameraRect.y);

    if(world.m_mouseIsPressed && borderActive)
    {
        if(!checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, armyScreenRect))
        {
            army -> dstRect.x = (world.m_mouse.x - army -> objRect.w / 2) / zoom_lvl + cameraRect.x;
            army -> dstRect.y = (world.m_mouse.y - army -> objRect.h / 2) / zoom_lvl + cameraRect.y;

            borderActive = false;

            army -> mooving = true;
        }
    }

    if(army -> mooving)
    {
        armyDirection.x = army -> dstRect.x - army -> objRect.x;
        armyDirection.y = army -> dstRect.y - army -> objRect.y;

        moveRatio = (double)armyDirection.x / (double)armyDirection.y;

        if(armyDirection.x < 0)
        {
            army -> coor.x += speed * fabs(moveRatio) * -1.0;
        }
        else
        {
           army -> coor.x += speed * fabs(moveRatio);
        }

        if(armyDirection.y < 0)
        {
            army -> coor.y += speed * -1.0 / fabs(moveRatio);
        }
        else
        {
           army -> coor.y += speed / fabs(moveRatio);
        }

        army -> objRect.x = army -> coor.x;
        army -> objRect.y = army -> coor.y;

        if(abs(army -> dstRect.x - army -> objRect.x) < 5)
        {
            army -> mooving = false;
        }
    }
   /// D(zoom_lvl);
}
