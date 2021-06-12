#include "HealthManager.h"
#include "World.h"
#include "string"
using namespace std;

extern World world;

HealthManager::HealthManager()
{
    m_numberOfUnits = nullptr;
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
    m_objectTexture = LoadTexture(buff, renderer);
    stream >> tmp >> m_objectRect.w >> m_objectRect.h;
    stream >> tmp >> m_damageDevider;
    m_objectRect.x = 0;
    m_objectRect.y = 0;

    stream.close();
}

void HealthManager::drawHealthbar(SDL_Renderer* renderer, SDL_Rect objRect)
{
    m_objectRect.x = objRect.x + objRect.w / 2;
    m_objectRect.y = objRect.y + objRect.w - 25;

    m_numbersRect.x = m_objectRect.x + m_objectRect.w / 2;
    m_numbersRect.y = m_objectRect.y + m_objectRect.h / 2;

    m_numbersRect.x -= m_numbersRect.w/2;
    m_numbersRect.y -= m_numbersRect.h/2;
    SDL_RenderCopy(world.m_main_renderer, m_objectTexture, NULL, &m_objectRect);
    SDL_RenderCopy(renderer, m_numbersTexture, NULL, &m_numbersRect);
}

void HealthManager::takeDamage(double damage)
{
    damage /= m_damageDevider;

    D(*m_numberOfUnits);

    *m_numberOfUnits -= damage;

    D(*m_numberOfUnits);
    D(damage);

    updateUnitsNumber();
}

void HealthManager::updateUnitsNumber()
{
    string buffStr = to_string(*m_numberOfUnits);
    m_numbersTexture = stringToTexture(buffStr, 25);

    SDL_QueryTexture(m_numbersTexture, NULL, NULL, &m_numbersRect.w, &m_numbersRect.h);
}
