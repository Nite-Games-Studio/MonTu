#ifndef TILE_H
#define TILE_H

#include <string>
#include <fstream>
#include <iostream>

#include "defines.h"
#include "Engine.h"

using namespace std;

class Tile
{
    public:
        Tile();
        Tile(const Tile& model, coordinates* cameraOffset);
        virtual ~Tile();

        SDL_Texture* m_objectTexture;
        SDL_Rect m_objectRect;
        SDL_Rect m_presentRect;

        coordinates* m_cameraOffset;

        OWNER m_owner;
        short int m_walkDifficulty;
        vector<SDL_Point*> m_collisionPoints;

        string m_img;

        coordinates m_drawCoordinates;
        coordinates m_mapCoordinates;

        void init(Tile* model);
        void load(string configFile, SDL_Renderer* renderer);
        void load(ifstream& stream);
        void draw(SDL_Renderer* renderer);

    protected:

    private:
};

#endif // TILE_H
