#include <vector>
#include "Ui.h"


UI::UI()
{
    //ctor
}

UI::~UI()
{
    //dtor
}

UI::UI(const UI& model){

    m_img = model.m_img;
    m_objectRect = model.m_objectRect;
    m_objectTexture = model.m_objectTexture;


}

void UI::load(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;
    ifstream stream;
    string tmp, buff;
    stream.open(configFile.c_str());

    stream >> tmp >> m_img;
    stream >> tmp >> m_objectRect.w >> m_objectRect.h;
    stream >> tmp >> m_objectRect.x >> m_objectRect.y;

    stream.close();

    m_objectTexture = LoadTexture(m_img, renderer);
}

void UI::update()
{

}

void UI::draw(SDL_Renderer* renderer)
{
    m_presentRect = m_objectRect;
    SDL_RenderCopy(renderer, m_objectTexture, NULL, &m_presentRect);
}

void UI::initSelectables()
{
        for(int i = 0;i < 11 ;i++){
            m_selectable = new UISelectable();
            m_selectables.push_back(m_selectable);
        }
}
