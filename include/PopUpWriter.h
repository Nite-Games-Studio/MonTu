#ifndef POPUPWRITER_H
#define POPUPWRITER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <SDL.h>

#include "defines.h"
#include "Engine.h"


class PopUpWriter
{
    public:
        PopUpWriter();
        virtual ~PopUpWriter();

        SDL_Renderer* m_renderer;

        SDL_Texture* m_buildingListTexture;

        SDL_Rect m_buildingListRect;

        short int m_verticalSpacing;
        short int m_horizontalSpacing;

        bool m_presentBuildingList;

        SDL_Rect returnAvailablePosition(SDL_Rect objRect, SDL_Rect popRect, short int WINDOW_WIDTH, short int WINDOW_HEIGH);

        void init(string configFile, SDL_Renderer* renderer);
        void draw(SDL_Rect objRect, SDL_Rect popRect, SDL_Texture* objTexture);

    protected:

    private:
};

#endif // POPUPWRITER_H
