#include "Castle.h"
#include "WorldBuilding.h"

extern WorldBuilding cityView;

Castle::Castle()
{
    //ctor
}

Castle::Castle(const Castle& model)
{
    m_walkDifficulty = model.m_walkDifficulty;
    m_img = model.m_img;
    m_objectTexture = model.m_objectTexture;
    m_ownerTextureP1 = model.m_ownerTextureP1;
    m_ownerTextureP2 = model.m_ownerTextureP2;
    m_buildTexture = model.m_buildTexture;
    m_damageTexture = model.m_damageTexture;
    m_height = model.m_height;
    m_width = model.m_width;
    m_adjustment = model.m_adjustment;
    m_drawState = model.m_drawState;
    m_maxHealth = model.m_maxHealth;
    m_healthPerState = model.m_healthPerState;
    m_states = model.m_states;
    m_maxState = model.m_maxState;
    m_duration = model.m_duration;
    m_getPosition = model.m_getPosition;
    m_currState = model.m_currState;
    m_health = model.m_health;
    m_constructing = model.m_constructing;
    m_useObject = model.m_useObject;
    m_currTimePoint = steady_clock::now();

    m_type = model.m_type;
    /*m_objectRect.x = 100;
    m_objectRect.y = 100;*/
    m_objectRect.h = cityView.m_hexagonHeight*3;
    m_objectRect.w = cityView.m_hexagonWidth*3 +15;
}

Castle::~Castle()
{
    //dtor
}

void Castle::load(ifstream& stream)
{
    string tmp;
    stream >> tmp >> m_walkDifficulty;
    stream >> tmp >> m_img;
    stream >> tmp >> m_ownerImgP1;
    stream >> tmp >> m_ownerImgP2;
    stream >> tmp >> m_buildImg;
    stream >> tmp >> m_damageImg;
    stream >> tmp >> m_height;
    stream >> tmp >> m_width;
    stream >> tmp >> m_adjustment;
    stream >> tmp >> m_maxHealth;
    stream >> tmp >> m_states;
    stream >> tmp >> m_maxState;
    stream >> tmp >> m_duration;
    stream >> tmp >> m_getPosition;
    stream >> tmp >> m_currState;
    stream >> tmp >> m_health;
    stream >> tmp >> m_constructing;
}

void Castle::load(string configFile, SDL_Renderer* renderer)
{
    m_type = configFile;
    configFile = "config\\" + configFile;

    ifstream stream;
    stream.open(configFile.c_str());

    load(stream);

    stream.close();

    m_objectTexture = LoadTexture(m_img, renderer);

    m_ownerTextureP1 = LoadTexture(m_ownerImgP1, renderer);
    m_ownerTextureP2 = LoadTexture(m_ownerImgP2, renderer);

    m_buildTexture = LoadTexture(m_buildImg, renderer);

    m_damageTexture = LoadTexture(m_damageImg, renderer);

    m_healthPerState = m_maxHealth / m_maxState;

    m_drawState.h = m_height;
    m_drawState.w = m_width;
    m_drawState.x = 0;
}

void Castle::setMapCoordinates(int a, int b){

    m_mapCoordinates.x = a;
    m_mapCoordinates.y = b;

}

void Castle::update()
{

}

void Castle::draw(SDL_Renderer* renderer)
{
    //cout<<"CASTLE  "<<endl;
    //D(m_objectRect.h);
    SDL_RenderCopy(renderer, m_objectTexture, NULL, &m_objectRect);
    //cout<<"objRect"<<m_objectRect.x<<" "<<m_objectRect.y<<" "<<m_objectRect.w<<" "<<m_objectRect.h<<endl;
}

