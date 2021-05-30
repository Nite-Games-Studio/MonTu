#ifndef PICKANDBAN_H
#define PICKANDBAN_H

#include <vector>
#include <fstream>
#include <iostream>

#include "defines.h"

#include "Engine.h"

using namespace std;

class PickAndBan
{
    public:
        PickAndBan();
        virtual ~PickAndBan();

        SDL_Renderer* m_renderer;

        vector<icon*> m_icons;

        SDL_Texture* m_backgroundTexture;

        SDL_Rect m_Map1Button;
        SDL_Rect m_Map2Button;
        SDL_Rect m_Map3Button;
        SDL_Rect m_Map4Button;

        void init(string configFile, SDL_Renderer* renderer);
        void update();
        void draw();
        void pick(SQUAD picked);
        void ban(SQUAD banned);
    protected:

    private:
};

#endif // PICKANDBAN_H
