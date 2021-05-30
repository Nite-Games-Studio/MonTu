#ifndef UI_H
#define UI_H

#include <string>
#include <fstream>
#include <iostream>

#include "defines.h"
#include "Engine.h"
#include "UISelectable.h"

class UI
{

    public:
        UI();
        UI(const UI& model);
        virtual ~UI();

        string configFile;
        SDL_Renderer* renderer;

        void load(string configFile, SDL_Renderer* renderer);
        void update();
        void draw(SDL_Renderer* renderer);
        void initSelectables();

        SDL_Texture* m_objectTexture;
        SDL_Rect m_objectRect;
        SDL_Rect m_presentRect;

        UISelectable* m_selectable = NULL /*new UISelectable(*(world.m_configManager.modelUISelectable)*/;
        vector<UISelectable*> m_selectables;

        coordinates m_drawCoordinates;


        string m_img;

    protected:

    private:
};

#endif // UI_H
