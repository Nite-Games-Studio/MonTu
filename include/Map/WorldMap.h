#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <iostream>
#include <fstream>
#include <SDL.h>
#include <ctime>
#include <vector>
#include <time.h>
#include <cmath>

#include "Engine.h"
#include "defines.h"
#include "City.h"

class WorldMap
{
public:
    WorldMap();
    virtual ~WorldMap();

    SDL_Texture* WorldMapTexture;
    SDL_Texture* FlagTexture;
    SDL_Texture* armyTexture;

    SDL_Rect maxZoom;
    SDL_Rect minZoom;
    SDL_Rect cameraRect;
    SDL_Rect chunkCameraRect;

    coordinates cameraCenter;
    coordinates direction;
    coordinates currentPos;
    coordinates mouseDragDistance;
    coordinates cameraPosBeforeDrag;
    coordinates armyDirection;

    UI_object m_selectedArmy;

    vector <SDL_Texture*> mapPieces;
    vector <mapObject*> armyVec;
    vector <City*> m_cities;
    vector <string> m_armyFiles;

    int zoomMulti;
    int mapWidth;
    int mapHeight;
    int oldX, oldY;
    int currentChunk;
    int allImages;
    int imagesPerChunk;

    float zoom_lvl = 1;
    double moveRatio;
    double speed;

    void drawMap();
    void init(string configFile);
    void update();
    void draw();
    void framer();
    void zoom();
    void moveWithMouse();
    void loadMap(string configFile);
    void openCity();
    void drawArmy();
    void updateArmy();
    void saveArmy(string configFile);
    void save(mapObject* army, int pos);
    void loadArmy(string configFile);
    void load(ifstream& stream);
    void loadCity(string configFile);
    void addArmy(coordinates coor, int index);
    void loadSquad(string configFile);

protected:

private:
};

#endif // WORLDMAP_H
