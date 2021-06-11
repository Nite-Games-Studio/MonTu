#include "Particle.h"

#include <fstream>

Particle::Particle()
{
    //ctor
}

Particle::~Particle()
{
    //dtor
}

Particle::Particle(const Particle& model, SDL_Rect rect)
{
    m_duration = model.m_duration;
    m_frames = model.m_frames;
    m_srcRect =  model.m_srcRect;

    m_renderer = model.m_renderer;

    m_sprites = model.m_sprites;

    m_timePerFrame = model.m_timePerFrame;

    m_objRect = rect;
}
