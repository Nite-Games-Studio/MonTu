#ifndef HEALTHMANAGER_H
#define HEALTHMANAGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <SDL.h>

#include "Engine.h"
#include "defines.h"

class HealthManager
{
    public:
        SDL_Texture* m_objectTexture;
        SDL_Texture* m_numbersTexture;

        SDL_Renderer* m_renderer;

        SDL_Rect m_objectRect;
        SDL_Rect m_numbersRect;

        int m_damageDevider;
        short* m_numberOfUnits;

        void init(string configFile, SDL_Renderer* renderer);
        void drawHealthbar(SDL_Renderer* renderer, SDL_Rect objRect);
        void takeDamage(double damage);
        void updateUnitsNumber();

        HealthManager();
        virtual ~HealthManager();

    protected:

    private:
};

#endif // HEALTHMANAGER_H
