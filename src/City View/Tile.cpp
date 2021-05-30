#include "Tile.h"

Tile::Tile()
{
    m_owner = NOOWNER;
}

Tile::~Tile()
{
    //dtor
}

Tile::Tile(const Tile& model, coordinates* cameraOffset)
{
    m_walkDifficulty = model.m_walkDifficulty;
    m_objectTexture = model.m_objectTexture;

    m_cameraOffset = cameraOffset;
}

void Tile::load(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;
    ifstream stream;
    string tmp, buff;
    stream.open(configFile.c_str());

    stream >> tmp >> m_walkDifficulty;
    stream >> tmp >> buff;

    stream.close();

    m_objectTexture = LoadTexture(buff, renderer);
}

void Tile::load(ifstream& stream){
    string tmp;
    stream >> tmp >> m_walkDifficulty;
    stream >> tmp >> m_img;
}

void Tile::draw(SDL_Renderer* renderer)
{
    m_presentRect.x = m_objectRect.x + m_cameraOffset->x;
    m_presentRect.y = m_objectRect.y + m_cameraOffset->y;
    m_presentRect.w = m_objectRect.w;
    m_presentRect.h = m_objectRect.h;
    SDL_RenderCopy(renderer, m_objectTexture, NULL, &m_presentRect);
}
