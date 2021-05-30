#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <SDL.h>

#include "defines.h"

using namespace std;

class Menu
{
    public:
        Menu();
        virtual ~Menu();
        SDL_Renderer* renderer;

        Button playButton;
        Button optionsButton;
        Button exitButton;
        Button backButton;

        SDL_Rect m_Map1Button;
        SDL_Rect m_Map2Button;
        SDL_Rect m_Map3Button;
        SDL_Rect m_Map4Button;

        SDL_Texture* m_menuTexture;
        SDL_Texture* m_backgroundMapTexture;

        SDL_Texture* m_Map1Texture;
        SDL_Texture* m_Map2Texture;
        SDL_Texture* m_Map3Texture;
        SDL_Texture* m_Map4Texture;

        SDL_Texture* m_Map1PickTexture;
        SDL_Texture* m_Map2PickTexture;
        SDL_Texture* m_Map3PickTexture;
        SDL_Texture* m_Map4PickTexture;

        int widthChange, heightChange;

        void menu();
        void Choose_Map();
        void init(string configFile);
        void draw();
        void update();
        void buttonHover(Button* button);

    protected:

    private:
};

#endif // MENU_H
