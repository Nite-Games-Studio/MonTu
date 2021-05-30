#ifndef UISELECTABLE_H
#define UISELECTABLE_H

#include <string>
#include <fstream>
#include <iostream>

#include "defines.h"
#include "Engine.h"



class UISelectable
{
    public:
        UISelectable();
        virtual ~UISelectable();
        UISelectable(const UISelectable& model,short int position);

        string m_img;
        string m_img_sel;
        string m_imgStreet;
        string m_imgShop;
        string m_imgArchery;
        string m_imgCastle;

        short int m_margins;
        short int m_numberOfSelectables;
        short int m_widthBegin;
        short int m_height;
        short int m_position;

        UI_ICON_TYPE m_type;

        bool m_selected = false;


        SDL_Texture* m_typeTexture = NULL;
        Button button;
        SDL_Rect m_presentRect;

        void load(string configFile, SDL_Renderer* renderer);
        void update();
        void draw(SDL_Renderer* renderer);
        //void drawWhenSelected(SDL_Renderer* renderer);

        void assignSelectable(SDL_Renderer* renderer, UI_ICON_TYPE type);
        void checkState(UISelectable* selectable,int mouseX, int mouseY);

    protected:

    private:
};

#endif // UISELECTABLE_H
