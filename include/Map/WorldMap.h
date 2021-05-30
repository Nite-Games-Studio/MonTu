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

class WorldMap
{
    public:
        WorldMap();
        virtual ~WorldMap();

        GAME_STATE gameState;

        SDL_Texture* WorldMapTexture;
        SDL_Texture* NextButtonTexture;

        mapObject army;

        SDL_Rect nextButtonRect;
        SDL_Rect maxZoom;
        SDL_Rect minZoom;
        SDL_Rect cameraRect;
        SDL_Rect chunkCameraRect;

        SDL_Rect cameraDstRect1;
        SDL_Rect cameraDstRect2;
        SDL_Rect cameraDstRect3;
        SDL_Rect cameraDstRect4;

        SDL_Rect cameraSrcRect1;
        SDL_Rect cameraSrcRect2;
        SDL_Rect cameraSrcRect3;
        SDL_Rect cameraSrcRect4;

        coordinates cameraCenter;
        coordinates direction;
        coordinates currentPos;
        coordinates mouseDragDistance;
        coordinates cameraPosBeforeDrag;
        coordinates armyDirection;

        UI_object m_selectedArmy;

        vector <SDL_Texture*> mapPieces;

        int zoomMulti;
        int mapWidth;
        int mapHeight;
        int oldX, oldY;
        int currentChunk;
        int allImages;
        int imagesPerChunk;

        bool borderActive;

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
        void openCity(SDL_Rect cityRect);
        void drawArmy(mapObject* army);
        void updateArmy(mapObject* army);

    protected:

    private:
};

#endif // WORLDMAP_H
