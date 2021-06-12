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
    string armyTestImg;
    string borderImg;
    string flagImg;

    stream.open(configFile.c_str());

    stream >> tmp >> zoomMulti;
    stream >> tmp >> maxZoom.w >> maxZoom.h;
    stream >> tmp >> minZoom.w >> minZoom.h;
    stream >> tmp >> allImages;
    stream >> tmp >> imagesPerChunk;
    stream >> tmp >> armyTestImg;
    stream >> tmp >> borderImg;
    stream >> tmp >> speed;
    stream >> tmp >> flagImg;

    stream.close();

    armyTexture = LoadTexture(armyTestImg, world.m_main_renderer);
    m_selectedArmy.objTexture = LoadTexture(borderImg, world.m_main_renderer);
    FlagTexture = LoadTexture(flagImg, world.m_main_renderer);

    cameraRect.x = 0;
    cameraRect.y = 0;
    cameraRect.w = world.m_SCREEN_WIDTH;
    cameraRect.h = world.m_SCREEN_HEIGHT;

    cameraPosBeforeDrag = world.m_mouse;

    loadSquad("all_squads.txt");
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

    for (int i = 0; i < 16; i++)
    {
        mapPieces.push_back(LoadTexture("WorldMap\\" + MapImg[i], world.m_main_renderer));
    }
}

void WorldMap::loadCity(string configFile)
{
    configFile = "data\\" + configFile;
    fstream stream;

    string tmp;

    stream.open(configFile.c_str());

    while(!stream.eof())
    {
        stream >> tmp;
        tmp = "\\cities\\" + tmp;
        City* city = new City;
        city->load(tmp);
        m_cities.push_back(city);
    }

    stream.close();
}

void WorldMap::update()
{
    oldX = cameraRect.x;
    oldY = cameraRect.y;

    zoom();

    framer();

    moveWithMouse();
  
    updateArmy();

    openCity();
}

void WorldMap::draw()
{
    SDL_RenderClear(world.m_main_renderer);

    drawMap();

    for (int i = 0; i < m_cities.size(); i++)
    {
        m_cities[i]->draw();
    }

    drawArmy();

    SDL_RenderPresent(world.m_main_renderer);
}

void WorldMap::framer()
{
    if (cameraRect.w < minZoom.w || cameraRect.h < minZoom.h)
    {
        cameraRect.x = oldX;
        cameraRect.y = oldY;
        cameraRect.w = minZoom.w;
        cameraRect.h = minZoom.h;
    }
    if (cameraRect.w > maxZoom.w || cameraRect.h > maxZoom.h)
    {
        cameraRect.x = oldX;
        cameraRect.y = oldY;
        cameraRect.w = maxZoom.w;
        cameraRect.h = maxZoom.h;
    }
    if (cameraRect.x + cameraRect.w > 7680)
    {
        cameraRect.x = 7680 - cameraRect.w;
    }
    if (cameraRect.x < 0)
    {
        cameraRect.x = 0;
    }
    if (cameraRect.y + cameraRect.h > 4320)
    {
        cameraRect.y = 4320 - cameraRect.h;
    }
    if (cameraRect.y < 0)
    {
        cameraRect.y = 0;
    }
}

void WorldMap::zoom()
{
    if (world.m_event.type == SDL_MOUSEWHEEL)
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
    SDL_Rect src = { 0, 0, 1920, 1080 };

    for (int i = 0; i < allImages; i++)
    {
        SDL_Rect world_space = { 1920 * (i % imagesPerChunk), 1080 * (i / imagesPerChunk), 1920, 1080 };

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
    if (world.m_drag)
    {
        mouseDragDistance.x = world.m_mouse.x - currentPos.x;
        mouseDragDistance.y = world.m_mouse.y - currentPos.y;

        cameraRect.x = cameraPosBeforeDrag.x - mouseDragDistance.x;
        cameraRect.y = cameraPosBeforeDrag.y - mouseDragDistance.y;
    }
    else if (!world.m_drag)
    {
        currentPos.x = world.m_mouse.x;
        currentPos.y = world.m_mouse.y;

        cameraPosBeforeDrag.x = cameraRect.x;
        cameraPosBeforeDrag.y = cameraRect.y;
    }
}

void WorldMap::openCity()
{
    for (int i = 0; i < m_cities.size(); i++)
    {
        for (int j = 0; j < armyVec.size(); j++)
        {
            SDL_Rect screen_space =
            {
                zoom_lvl * (m_cities[i]->m_objRect.x - cameraRect.x),
                zoom_lvl * (m_cities[i]->m_objRect.y - cameraRect.y),
                zoom_lvl * m_cities[i]->m_objRect.w,
                zoom_lvl * m_cities[i]->m_objRect.h
            };

            if (checkForCollisionBetweenRects(armyVec[j]->objRect, m_cities[i]->m_objRect))
            {
                armyVec[j]->objRect.x += 500;
                armyVec[j]->objRect.y += 500;
                world.currentCity = m_cities[i]->m_cityName;
                world.m_quitScene = true;
                world.m_gameState = CITYBUILDING;
            }                      
        }   
    }
}

void WorldMap::loadSquad(string configFile)
{
    configFile = "data\\" + configFile;
    fstream stream;

    string tmp;
    int buffInt;
    coordinates buffCoor;

    stream.open(configFile.c_str());

    while (!stream.eof())
    {
        stream >> tmp;
        m_armyFiles.push_back(tmp);
    }

    stream.close();

    for (int i = 0; i < m_armyFiles.size(); i++)
    {
        m_armyFiles[i] = "data\\squads\\" + m_armyFiles[i];
        
        stream.open(m_armyFiles[i]);

        stream >> tmp >> buffInt;

        if (buffInt == 1)
        {
            stream >> tmp >> buffInt;
            stream >> tmp >> buffCoor.x >> buffCoor.y;
            addArmy(buffCoor, buffInt);
            cout << "HERE";
        }
        stream.close();
    }
}

void WorldMap::drawArmy()
{
    for (int i = 0; i < armyVec.size(); i++)
    {
        SDL_Rect screen_space =
        {
            zoom_lvl * (armyVec[i]->objRect.x - cameraRect.x),
            zoom_lvl * (armyVec[i]->objRect.y - cameraRect.y),
            zoom_lvl * armyVec[i]->objRect.w,
            zoom_lvl * armyVec[i]->objRect.h
        };

        if (world.m_mouseIsDoubleClicked && checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, screen_space))
        {
            m_selectedArmy.objRect.x = (screen_space.x - 5);
            m_selectedArmy.objRect.y = (screen_space.y - 5);
            m_selectedArmy.objRect.w = (screen_space.w + 10);
            m_selectedArmy.objRect.h = (screen_space.h + 10);

            armyVec[i]->borderActive = true;
        }

        if (zoom_lvl < 0.9)
        {
            SDL_RenderCopy(world.m_main_renderer, FlagTexture, NULL, &(screen_space));
        }
        else
        {
            SDL_RenderCopy(world.m_main_renderer, armyTexture, NULL, &(screen_space));
        }

        if (armyVec[i]->borderActive == true)
        {
            m_selectedArmy.objRect.x = (screen_space.x - 5);
            m_selectedArmy.objRect.y = (screen_space.y - 5);
            m_selectedArmy.objRect.w = (screen_space.w + 10);
            m_selectedArmy.objRect.h = (screen_space.h + 10);

            SDL_RenderCopy(world.m_main_renderer, m_selectedArmy.objTexture, NULL, &(m_selectedArmy.objRect));
        }
    }  
}

void WorldMap::addArmy(coordinates coor, int index)
{
    mapObject* newArmy = new mapObject();

    newArmy->index = index;

    newArmy->objRect.x = coor.x;
    newArmy->objRect.y = coor.y;
    newArmy->objRect.w = 100;
    newArmy->objRect.h = 100;
    newArmy->objTexture = armyTexture;
    newArmy->coor.x = coor.x;
    newArmy->coor.y = coor.y;

    newArmy->dstRect = newArmy->objRect;

    armyVec.push_back(newArmy);
}

void WorldMap::updateArmy()
{
    zoom_lvl = world.m_SCREEN_WIDTH / (0.0 + cameraRect.w);

    for (int i = 0; i < armyVec.size(); i++)
    {
        SDL_Rect armyScreenRect;

        armyScreenRect = armyVec[i]->objRect;

        armyScreenRect.x = (armyVec[i]->objRect.x - cameraRect.x);
        armyScreenRect.y = (armyVec[i]->objRect.y - cameraRect.y);

        if (world.m_mouseIsPressed && armyVec[i]->borderActive)
        {
            if (!checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, armyScreenRect))
            {
                armyVec[i]->dstRect.x = (world.m_mouse.x - armyVec[i]->objRect.w / 2) / zoom_lvl + cameraRect.x;
                armyVec[i]->dstRect.y = (world.m_mouse.y - armyVec[i]->objRect.h / 2) / zoom_lvl + cameraRect.y;

                armyVec[i]->borderActive = false;

                armyVec[i]->mooving = true;
            }
        }

        if (armyVec[i]->mooving)
        {
            armyDirection.x = armyVec[i]->dstRect.x - armyVec[i]->objRect.x;
            armyDirection.y = armyVec[i]->dstRect.y - armyVec[i]->objRect.y;

            moveRatio = (double)armyDirection.x / (double)armyDirection.y;

            if (armyDirection.x < 0)
            {
                armyVec[i]->coor.x += speed * fabs(moveRatio) * -1.0;
            }
            else
            {
                armyVec[i]->coor.x += speed * fabs(moveRatio);
            }

            if (armyDirection.y < 0)
            {
                armyVec[i]->coor.y += speed * -1.0 / fabs(moveRatio);
            }
            else
            {
                armyVec[i]->coor.y += speed / fabs(moveRatio);
            }

            armyVec[i]->objRect.x = armyVec[i]->coor.x;
            armyVec[i]->objRect.y = armyVec[i]->coor.y;

            if (abs(armyVec[i]->dstRect.x - armyVec[i]->objRect.x) < 5)
            {
                armyVec[i]->mooving = false;
            }
        }
    }

    //for (int i = 0; i < armyVec.size(); i++)
    //{
    //    save(army, i);
    //}
}

void WorldMap::save(mapObject* army, int pos)
{
    ofstream file;
    string configFile;

    configFile = "Army_" + to_string(pos);
    configFile = "data\\army\\" + configFile + ".txt";

    file.open(configFile.c_str());

    file << "Army_X: " << army->coor.x << '\n';
    file << "Army_Y: " << army->coor.y << '\n';
    ///file << "DstRectX: " << army.dstRect.x << '\n';
    ///file << "DstRectY: " << army.dstRect.y << '\n';
    ///file << "DstRectW: " << army.dstRect.w << '\n';
    ///file << "DstRectH: " << army.dstRect.h << '\n';
    ///file << "Mooving: " << army.mooving << '\n';

    file.close();
}

void WorldMap::saveArmy(string configFile)
{
    configFile = "data\\" + configFile;

    ofstream file;

    file.open(configFile.c_str());

    for (int i = 0; i < armyVec.size(); i++)
    {
        save(armyVec[i], i);
        file << '\n';
    }

    file.close();
}

void WorldMap::load(ifstream& stream)
{
    mapObject army;

    string tmp;

    stream >> tmp >> army.coor.x;
    stream >> tmp >> army.coor.y;
    ///    stream >> tmp >> army -> dstRect.x;
    ///    stream >> tmp >> army -> dstRect.y;
    ///    stream >> tmp >> army -> dstRect.w;
    ///    stream >> tmp >> army -> dstRect.h;
    ///    stream >> tmp >> army -> mooving;
}
//
//void WorldMap::loadArmy(string configFile)
//{
//    configFile = "data\\" + configFile;
//
//    mapObject* army;
//
//    ifstream file;
//    string tmp;
//
//    int index = 0;
//
//    file.open(configFile.c_str());
//
//    while(!file.eof())
//    {
//        //load(&file);
//        index ++;
//    }
//
//    file.close();
//}
