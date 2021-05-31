#include "Bridge.h"
#include "WorldBuilding.h"

extern WorldBuilding cityView;

Bridge::Bridge()
{
    //ctor
}

Bridge::Bridge(const Bridge& model)
{
    m_walkDifficulty = model.m_walkDifficulty;
    m_objectTexture = model.m_objectTexture;
    m_height = model.m_height;
    m_width = model.m_width;
    m_imgRows = model.m_imgRows;
    m_imgCols = model.m_imgCols;
    m_rotationAngle = model.m_rotationAngle;
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

    /*m_objectRect.x = tile->m_drawCoordinates.x;
    m_objectRect.y = tile->m_drawCoordinates.y;
    m_objectRect.h = cityView.m_hexagonHeight;
    m_objectRect.w = cityView.m_hexagonWidth;*/
}

Bridge::~Bridge()
{
    SDL_DestroyTexture(m_objectTexture);
}

void Bridge::load(ifstream& stream)
{
    string tmp;
    stream >> tmp >> m_height;
    stream >> tmp >> m_width;
    stream >> tmp >> m_imgRows >> m_imgCols;
    stream >> tmp >> m_rotationAngle;
    stream >> tmp >> m_maxHealth;
    stream >> tmp >> m_states;
    stream >> tmp >> m_maxState;
    stream >> tmp >> m_duration;
    stream >> tmp >> m_getPosition;
    stream >> tmp >> m_currState;
    stream >> tmp >> m_health;
    stream >> tmp >> m_constructing;
}

void Bridge::load(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;
    ifstream stream;
    stream.open(configFile.c_str());

    Tile::load(stream);
    load(stream);

    stream.close();

    m_objectTexture = LoadTexture(m_img, renderer);

    m_healthPerState = m_maxHealth / m_maxState;

    m_useObject = true;

    m_drawState.h = m_height;
    m_drawState.w = m_width;
    m_drawState.x = 0;
}

void Bridge::limitHealth()
{
    if (m_health >= m_maxHealth)
    {
        m_health = m_maxHealth;
        m_healthPerState = m_maxHealth / (m_states - m_maxState);
        m_constructing = false;
    }
}

void Bridge::select()
{
    if (cityView.m_selected.x == m_objectRect.x && cityView.m_selected.y == m_objectRect.y)
    {
        if (!m_getPosition)
        {
            /*if (GetAsyncKeyState(VK_LEFT))
            {
                m_rotationAngle -= MOVEMENT;
            }
            if (GetAsyncKeyState(VK_RIGHT))
            {
                m_rotationAngle += MOVEMENT;
            }
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
            Sleep(100);*/
            m_currTimePoint = steady_clock::now();
            m_currState = 0;
        }
    }
    else
    {
        m_getPosition = true;
    }
}

void Bridge::getState()
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
        m_drawState.h = m_height / m_imgRows;
        m_drawState.w = m_width / m_imgCols;
        m_currState = m_health / m_healthPerState;
    }
    else
    {
        m_currState = (m_maxHealth - m_health) / m_healthPerState + m_maxState;
    }
    m_drawState.x = ((m_width / m_imgCols) * m_currState) % m_width;
    m_drawState.y = (m_height / m_imgRows) * (m_currState / m_imgCols);
    if (m_currState == m_states)
    {
        m_drawState.h = 0;
        m_drawState.w = 0;
    }
}

void Bridge::update()
{
    select();
    if (m_useObject)
    {
        getState();
    }
}

void Bridge::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopyEx(renderer, m_objectTexture, &m_drawState, &m_objectRect, m_rotationAngle, NULL, SDL_FLIP_NONE);
}
