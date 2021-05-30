#include "UISelectable.h"

#include "World.h"
extern World world;
UISelectable::UISelectable()
{
    //ctor
}

UISelectable::~UISelectable()
{
    //dtor
}

UISelectable::UISelectable(const UISelectable& model,short int position){

    m_img = model.m_img;
    m_imgShop = model.m_imgShop;
    m_imgStreet = model.m_imgStreet;
    m_imgArchery = model.m_imgArchery;

    button = model.button;

    button.currentBonusW = 0;
    button.currentBonusH = 0;

    m_margins= model.m_margins;
    m_widthBegin = model.m_widthBegin;
    m_height = model.m_height;
    m_position = model.m_position;

    button.objectRect.x = m_widthBegin + position*(2*m_margins + button.minRect.w) + m_margins ;
    button.objectRect.y = world.m_SCREEN_HEIGHT - button.minRect.h - m_margins - m_height;

    button.minRect = button.objectRect;

    m_type = NOBUILD;

}

void UISelectable::load(string configFile, SDL_Renderer* renderer){


    configFile = "config\\" + configFile;
    ifstream stream;
    string tmp, buff;
    stream.open(configFile.c_str());
    stream >> tmp >> m_img;
    stream >> tmp >> button.minRect.w >> button.minRect.h;
    stream >> tmp >> button.maxRect.w >> button.maxRect.h;
    stream >> tmp >> m_margins;
    stream >> tmp >> m_widthBegin;
    stream >> tmp >> m_height;
    stream >> tmp >> m_imgStreet;
    stream >> tmp >> m_imgShop;
    stream >> tmp >> m_imgArchery;
    stream >> tmp >> m_imgCastle;
    stream >> tmp >> button.bonusW >> button.bonusH;
    //cout << tmp << m_imgStreet << m_imgShop << m_imgArchery << m_imgCastle << endl;;
    stream.close();

    button.objTexture = LoadTexture(m_img, renderer);

    button.objectRect = button.minRect;

}


void UISelectable::update()
{

}
void UISelectable::draw(SDL_Renderer* renderer)
{

    m_presentRect = button.objectRect;

    SDL_RenderCopy(renderer, button.objTexture, NULL, &m_presentRect);
    if(m_typeTexture!=NULL){
        SDL_RenderCopy(renderer, m_typeTexture, NULL, &m_presentRect);
    }
}


void UISelectable::assignSelectable(SDL_Renderer* renderer, UI_ICON_TYPE type){

    m_type = type;
    switch(type){
    case STREET:
        m_typeTexture = LoadTexture(m_imgStreet,renderer);
        break;
    case ARCHERY:
        m_typeTexture = LoadTexture(m_imgArchery,renderer);
        break;
    case SHOP:
        m_typeTexture = LoadTexture(m_imgShop,renderer);
        break;

    }

}

void UISelectable::checkState(UISelectable* selectable,int mouseX, int mouseY)
{

}
