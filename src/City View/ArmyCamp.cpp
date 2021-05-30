#include "ArmyCamp.h"
#include "WorldBuilding.h"

extern WorldBuilding cityView;

ArmyCamp::ArmyCamp()
{
    //ctor
}

ArmyCamp::ArmyCamp(const ArmyCamp& model)
{
    m_walkDifficulty = model.m_walkDifficulty;
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
    m_capacity = model.m_capacity;
    m_states = model.m_states;
    m_maxState = model.m_maxState;
    m_duration = model.m_duration;
    m_getPosition = model.m_getPosition;
    m_currState = model.m_currState;
    m_health = model.m_health;
    m_usedCapacity = model.m_usedCapacity;
    m_constructing = model.m_constructing;
    m_useObject = model.m_useObject;
    m_currTimePoint = steady_clock::now();
    for (int i = 1; i < (int) COUNT; i++){
        m_entity[i] = model.m_entity[i];
        m_entityWaitDuration[i] = model.m_entityWaitDuration[i];
    }

    /*m_objectRect.x = tile->m_drawCoordinates.x;
    m_objectRect.y = tile->m_drawCoordinates.y;
    m_objectRect.h = cityView.m_hexagonHeight;
    m_objectRect.w = cityView.m_hexagonWidth;
    m_owner = owner;*/
}

ArmyCamp::~ArmyCamp()
{
    SDL_DestroyTexture(m_objectTexture);
    SDL_DestroyTexture(m_ownerTextureP1);
    SDL_DestroyTexture(m_ownerTextureP2);
    SDL_DestroyTexture(m_buildTexture);
    SDL_DestroyTexture(m_damageTexture);
}

void ArmyCamp::load(ifstream& stream)
{
    string tmp;
    stream >> tmp >> m_ownerImgP1;
    stream >> tmp >> m_ownerImgP2;
    stream >> tmp >> m_buildImg;
    stream >> tmp >> m_damageImg;
    stream >> tmp >> m_height;
    stream >> tmp >> m_width;
    stream >> tmp >> m_adjustment;
    stream >> tmp >> m_maxHealth;
    stream >> tmp >> m_capacity;
    stream >> tmp >> m_states;
    stream >> tmp >> m_maxState;
    stream >> tmp >> m_duration;
    stream >> tmp >> m_getPosition;
    stream >> tmp >> m_currState;
    stream >> tmp >> m_health;
    stream >> tmp >> m_usedCapacity;
    stream >> tmp >> m_constructing;
    for (int i = 1; i < (int) COUNT; i++){
        stream >> tmp >> m_entity[i] >> m_entityWaitDuration[i];
    }
}

void ArmyCamp::load(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;
    ifstream stream;
    stream.open(configFile.c_str());

    Tile::load(stream);
    load(stream);

    stream.close();

    m_objectTexture = LoadTexture(m_img, renderer);

    m_ownerTextureP1 = LoadTexture(m_ownerImgP1, renderer);
    SDL_SetTextureColorMod(m_ownerTextureP1, cityView.m_CP1.r, cityView.m_CP1.g, cityView.m_CP1.b);
    m_ownerTextureP2 = LoadTexture(m_ownerImgP2, renderer);
    SDL_SetTextureColorMod(m_ownerTextureP2, cityView.m_CP2.r, cityView.m_CP2.g, cityView.m_CP2.b);

    m_buildTexture = LoadTexture(m_buildImg, renderer);

    m_damageTexture = LoadTexture(m_damageImg, renderer);

    m_healthPerState = m_maxHealth / m_maxState;

    m_useObject = true;

    m_drawState.h = m_height;
    m_drawState.w = m_width;
    m_drawState.x = 0;

    for (int i = 1; i < (int) COUNT; i++){
        m_entityTimePoint[i] = steady_clock::now();
        m_entityCount[i] = 0;
    }
}

void ArmyCamp::limitHealth()
{
    if (m_health >= m_maxHealth)
    {
        m_health = m_maxHealth;
        m_healthPerState = m_maxHealth / (m_states - m_maxState);
        if (m_constructing)
        {
            for (int i = 0; i < (int) COUNT; i++)
            {
                m_entityTimePoint[i] = steady_clock::now();
            }
        }
        m_constructing = false;
    }
}

void ArmyCamp::select()
{
    if (cityView.m_selected.x == m_objectRect.x && cityView.m_selected.y == m_objectRect.y)
    {
        if (!m_getPosition)
        {
            if (GetAsyncKeyState(VK_RETURN))
            {
                m_getPosition = true;
            }
            if (GetAsyncKeyState(VK_ESCAPE))
            {
                m_health = 0;
                m_useObject = false;
                m_getPosition = true;
            }
            Sleep(100);
        }
    }
    else
    {
        m_getPosition = true;
    }
}

void ArmyCamp::getState()
{
    limitHealth();
    if (m_constructing &&
            milliseconds(duration_cast<milliseconds>(steady_clock::now() - m_currTimePoint).count()) >= milliseconds(m_duration))
    {
        m_health += m_healthPerState;
        m_currTimePoint = steady_clock::now();
    }

    if (m_constructing)
    {
        m_currState = m_health / m_healthPerState;
        m_drawState.y = (m_maxState - m_currState) * (m_height / (m_maxState + 1));
        m_objectRect.h = (cityView.m_hexagonHeight / (m_maxState + 1)) * (m_currState + 1);
        m_objectRect.y -= m_adjustment;
        m_adjustment = cityView.m_hexagonHeight - (cityView.m_hexagonHeight / (m_maxState + 1)) * (m_currState + 1);
        m_objectRect.y += m_adjustment;
    }
    else
    {
        m_currState = (m_maxHealth - m_health) / m_healthPerState + m_maxState;
        m_drawState.y = (m_currState - m_maxState) * (m_height / (m_states - m_maxState + 1));
        m_objectRect.h = (cityView.m_hexagonHeight / (m_states - m_maxState)) * (m_states - m_currState);
        m_objectRect.y -= m_adjustment;
        m_adjustment = - cityView.m_hexagonHeight + (cityView.m_hexagonHeight / (m_maxState + 1)) * (m_currState + 1);
        m_objectRect.y += m_adjustment;
    }
}

void ArmyCamp::createEntities()
{
    //TODO: interaction with user
    if (m_health == m_maxHealth)
    {
        for (int i = 1; i < (int) COUNT; i++){
            if (m_entity[i] &&
                milliseconds(duration_cast<milliseconds>(steady_clock::now() - m_entityTimePoint[i]).count()) >= milliseconds(m_entityWaitDuration[i]))
            {
                m_entityCount[i]++;
                cityView.m_entityCount[i]++;
                m_entityTimePoint[i] = steady_clock::now();
            }
        }
    }
}

void ArmyCamp::update()
{
    select();
    if (!m_getPosition)
    {
        m_currTimePoint = steady_clock::now();
    }
    if (m_useObject && m_getPosition)
    {
        getState();
        createEntities();
    }
}
