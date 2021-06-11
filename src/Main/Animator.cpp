#include "Animator.h"
#include "World.h"
#include <fstream>

using namespace std;

extern World world;

Animator::Animator()
{
    m_animationEnded = false;
}

Animator::~Animator()
{
    delete m_sprites;
}

Animator::Animator(const Animator& model, SDL_Rect rect)
{
    m_duration = model.m_duration;
    m_frames = model.m_frames;
    m_srcRect = model.m_srcRect;

    m_renderer = world.m_main_renderer;

    m_sprites = model.m_sprites;

    m_timePerFrame = model.m_timePerFrame;

    m_objRect = rect;
}

void Animator::initAnimation(string configFile)
{
    configFile = "config//Animations//" + configFile;

    fstream stream;
    string tmp;
    string spritesLocation;
    unsigned short numberOfSprites;

    stream.open(configFile);

    stream >> tmp >> m_duration;
    stream >> tmp >> m_frames;
    stream >> tmp >> m_srcRect.w >> m_srcRect.h;
    stream >> tmp >> spritesLocation;

    stream.close();

    m_srcRect.x = 0;
    m_srcRect.y = 0;

    m_renderer = world.m_main_renderer;

    m_sprites = LoadTexture(spritesLocation, m_renderer);

    m_timePerFrame = (double) m_duration / (double)m_frames;
}

void Animator::start()
{
    m_lastFrameTime = chrono::steady_clock::now();
    m_startFrame = chrono::steady_clock::now();
}

void Animator::update()
{
    diff = chrono::steady_clock::now() - m_lastFrameTime;

    if(diff.count() > m_timePerFrame)
    {
        m_srcRect.x += m_srcRect.w;

        m_lastFrameTime = chrono::steady_clock::now();

        diff = chrono::steady_clock::now() - m_startFrame;
        if(diff.count() > m_duration)
        {
            m_animationEnded = true;
        }
    }
}

void Animator::draw()
{
    SDL_RenderCopy(m_renderer, m_sprites, &m_srcRect, &m_objRect);
}

bool Animator::isFinished()
{
    return m_animationEnded;
}
