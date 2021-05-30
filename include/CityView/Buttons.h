#ifndef BUTTONS_H
#define BUTTONS_H


#include <string>
#include <fstream>
#include <iostream>

#include "defines.h"
#include "Engine.h"
class Buttons
{
    public:
        Buttons();
        virtual ~Buttons();
        Buttons(const Buttons& model);

        short int m_buttonSize;
        short int m_buttonDrawStart;
        short int m_margins;
        short int m_position;

        string m_menuButtonImg;
        string m_exitButtonImg;
        string m_battleButtonImg;
        string m_mapButtonImg;

        SDL_Texture* m_menuButtonTexture;
        SDL_Texture* m_exitButtonTexture;
        SDL_Texture* m_battleButtonTexture;
        SDL_Texture* m_mapButtonTexture;

        SDL_Rect m_menuButtonRect;
        SDL_Rect m_exitButtonRect;
        SDL_Rect m_battleButtonRect;
        SDL_Rect m_mapButtonRect;

        void load(string configFile,SDL_Renderer* renderer);
        void update();
        void draw(SDL_Renderer* renderer);



    protected:

    private:
};

#endif // BUTTONS_H
