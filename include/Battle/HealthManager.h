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
        SDL_Texture* m_healthBarFullTxtr;
        SDL_Texture* m_healthBarEmptyTxtr;

        SDL_Renderer* m_renderer;

        SDL_Rect m_imgRect;
        SDL_Rect m_drawBorderRect;
        SDL_Rect m_drawFillRect;

        float m_healthBarWidth;

        void init(string configFile, SDL_Renderer* renderer);
        void drawHealthbar(SDL_Renderer* renderer, SDL_Rect objRect, float health, float maxHealth);

        HealthManager();
        virtual ~HealthManager();

    protected:

    private:
};

#endif // HEALTHMANAGER_H
