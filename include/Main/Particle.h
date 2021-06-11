#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL.h>

#include "Animator.h"

using namespace std;

class Particle : public Animator
{
    public:
        Particle();
        Particle(const Particle& model, SDL_Rect objRect);
        virtual ~Particle();

        SDL_Rect m_objectRect;


    protected:

    private:
        double timePerFrame;
};

#endif // PARTICLE_H
