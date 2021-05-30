#include "HealthManager.h"

HealthManager::HealthManager()
{
    //ctor
}

HealthManager::~HealthManager()
{
    //dtor
}

void HealthManager::init(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;

    fstream stream;
    string tmp, buff;
    stream.open(configFile.c_str());

    stream >> tmp >> buff;
    m_healthBarFullTxtr = LoadTexture(buff, renderer);
    stream >> tmp >> buff;
    m_healthBarEmptyTxtr = LoadTexture(buff, renderer);
    stream >> tmp >> m_healthBarWidth >> m_imgRect.h;
    stream >> tmp >> m_drawBorderRect.w >> m_drawBorderRect.h;
    m_imgRect.x = 0;
    m_imgRect.y = 0;

    stream.close();
}

void HealthManager::drawHealthbar(SDL_Renderer* renderer, SDL_Rect objRect, float health, float maxHealth)
{
    m_drawBorderRect.x = objRect.x - m_drawBorderRect.w / 2 + objRect.w / 2;
    m_drawBorderRect.y = objRect.y + (objRect.h / 5 * 4);
    SDL_RenderCopy(renderer, m_healthBarEmptyTxtr, NULL, &m_drawBorderRect);

    m_imgRect.w =(int)(m_healthBarWidth * (health/maxHealth));
    m_drawFillRect = m_drawBorderRect;
    m_drawFillRect.w = (int)(m_drawBorderRect.w * (health/maxHealth));

    SDL_RenderCopy(renderer, m_healthBarFullTxtr, &m_imgRect, &m_drawFillRect);
}
