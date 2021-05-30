#include "PopUpWriter.h"

PopUpWriter::PopUpWriter()
{
    //ctor
}

PopUpWriter::~PopUpWriter()
{
    //dtor
}

SDL_Rect PopUpWriter::returnAvailablePosition(SDL_Rect objRect, SDL_Rect popRect, short int WINDOW_WIDTH, short int WINDOW_HEIGH)
{
    SDL_Rect returnRect;
    returnRect.w = popRect.w;
    returnRect.h = popRect.h;

    // Case #1
    // We draw on top of the object

    returnRect.x = objRect.x + objRect.w / 2 - popRect.w / 2;
    returnRect.y = objRect.y - m_verticalSpacing - popRect.h;

    if(checkIfInScreen(returnRect, WINDOW_WIDTH, WINDOW_HEIGH))
    {
        return returnRect;
    }

    // Case #2:
    // We draw on top-right of the object

    returnRect.x = 0;

    if(checkIfInScreen(returnRect, WINDOW_WIDTH, WINDOW_HEIGH) && objRect.x < WINDOW_WIDTH / 2)
    {
        return returnRect;
    }

    // Case #3:
    // We draw on top-left of the object

    returnRect.x = WINDOW_WIDTH - popRect.w;

    if(checkIfInScreen(returnRect, WINDOW_WIDTH, WINDOW_HEIGH))
    {
        return returnRect;
    }

    // Case #4:
    // We draw on bot of the object

    returnRect.x = objRect.x + objRect.w / 2 - popRect.w / 2;
    returnRect.y = objRect.y + objRect.w + m_verticalSpacing;

    if(checkIfInScreen(returnRect, WINDOW_WIDTH, WINDOW_HEIGH))
    {
        return returnRect;
    }

    // Case #5:
    // We draw on bot-right of the object

    returnRect.x = 0;

    if(checkIfInScreen(returnRect, WINDOW_WIDTH, WINDOW_HEIGH) && objRect.x < WINDOW_WIDTH / 2)
    {
        return returnRect;
    }

    // Case #6:
    // We draw on bot-left of the object

    returnRect.x = WINDOW_WIDTH - popRect.w;

    if(checkIfInScreen(returnRect, WINDOW_WIDTH, WINDOW_HEIGH))
    {
        return returnRect;
    }

    return popRect;
}

void PopUpWriter::init(string configFile, SDL_Renderer* renderer)
{
    m_renderer = renderer;

    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;
    string strBuff;

    stream.open(configFile.c_str());
    stream >> tmp >> m_verticalSpacing >> m_horizontalSpacing;
    stream >> tmp >> strBuff;
    stream >> tmp >> m_buildingListRect.w >> m_buildingListRect.h;
    stream.close();

    m_buildingListRect.x = 0;
    m_buildingListRect.y = 0;

    m_presentBuildingList = false;

    m_buildingListTexture = LoadTexture(strBuff, m_renderer);
}

void PopUpWriter::draw(SDL_Rect objRect, SDL_Rect popRect, SDL_Texture* objTexture)
{
    if(m_presentBuildingList)
    {
        popRect = returnAvailablePosition(objRect, popRect, 1366, 768);
        SDL_RenderCopy(m_renderer, objTexture, NULL, &popRect);
    }
}
