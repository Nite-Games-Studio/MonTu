#include "Buttons.h"

Buttons::Buttons()
{
    //ctor
}

Buttons::~Buttons()
{
    //dtor
}

Buttons::Buttons(const Buttons& model)
{
        m_buttonSize = model.m_buttonSize;
        m_buttonDrawStart = model.m_buttonDrawStart;
        m_margins = model.m_margins;
        m_position = model.m_position;

        m_menuButtonImg = model.m_menuButtonImg;
        m_exitButtonImg = model.m_exitButtonImg;
        m_battleButtonImg = model.m_battleButtonImg;
        m_mapButtonImg = model.m_mapButtonImg;

        m_menuButtonTexture = model.m_menuButtonTexture;
        m_exitButtonTexture = model.m_exitButtonTexture;
        m_battleButtonTexture = model.m_battleButtonTexture;
        m_mapButtonTexture = model.m_mapButtonTexture;

        m_menuButtonRect = model.m_menuButtonRect;
        m_exitButtonRect = model.m_exitButtonRect;
        m_battleButtonRect = model.m_battleButtonRect;
        m_mapButtonRect = model.m_mapButtonRect;

        cout<<"Buttons in constructor: "<<m_menuButtonImg<<" "<<m_battleButtonImg<<" "<<m_mapButtonImg<<" "<<m_exitButtonImg<<endl;
}

void Buttons::load(string configFile,SDL_Renderer* renderer)
{

    configFile = "config\\" + configFile;

    fstream stream;

    string tmp;
    stream.open(configFile.c_str());

    stream >> tmp >> m_buttonSize;
    stream >> tmp >> m_buttonDrawStart;
    stream >> tmp >> m_margins;
    stream >> tmp >> m_menuButtonImg;
    stream >> tmp >> m_battleButtonImg;
    stream >> tmp >> m_mapButtonImg;
    stream >> tmp >> m_exitButtonImg;

    m_menuButtonTexture = LoadTexture(m_menuButtonImg,renderer);
    m_battleButtonTexture = LoadTexture(m_battleButtonImg,renderer);
    m_mapButtonTexture = LoadTexture(m_mapButtonImg,renderer);
    m_exitButtonTexture = LoadTexture(m_exitButtonImg,renderer);

    m_menuButtonRect.w = m_buttonSize;
    m_menuButtonRect.h = m_buttonSize;
    m_battleButtonRect.w = m_buttonSize;
    m_battleButtonRect.h = m_buttonSize;
    m_mapButtonRect.w = m_buttonSize;
    m_mapButtonRect.h = m_buttonSize;
    m_exitButtonRect.w = m_buttonSize;
    m_exitButtonRect.h = m_buttonSize;

    m_menuButtonRect.x = m_buttonDrawStart + 1*(2*m_margins + m_menuButtonRect.w) + m_margins;
    m_menuButtonRect.y = 2*m_margins;
    m_battleButtonRect.x = m_buttonDrawStart + 2*(2*m_margins + m_battleButtonRect.w) + m_margins;
    m_battleButtonRect.y = 2*m_margins;
    m_mapButtonRect.x = m_buttonDrawStart + 3*(2*m_margins + m_mapButtonRect.w) + m_margins;
    m_mapButtonRect.y = 2*m_margins;
    m_exitButtonRect.x = m_buttonDrawStart + 4*(2*m_margins + m_exitButtonRect.w) + m_margins;
    m_exitButtonRect.y = 2*m_margins;

    cout<<"Buttons in load: "<<m_menuButtonImg<<" "<<m_battleButtonImg<<" "<<m_mapButtonImg<<" "<<m_exitButtonImg<<endl;

    stream.close();
}

void Buttons::update()
{



}

void Buttons::draw(SDL_Renderer* renderer)
{

    SDL_RenderCopy(renderer, m_menuButtonTexture, NULL, &m_menuButtonRect);
    SDL_RenderCopy(renderer, m_battleButtonTexture, NULL, &m_battleButtonRect);
    SDL_RenderCopy(renderer, m_mapButtonTexture, NULL, &m_mapButtonRect);
    SDL_RenderCopy(renderer, m_exitButtonTexture, NULL, &m_exitButtonRect);

}
