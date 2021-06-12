#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <chrono>
#include "Engine.h"

using namespace std;

class Animator
{
    public:
        Animator();
        Animator(const Animator&, SDL_Rect objRect);
        virtual ~Animator();

        SDL_Rect m_objRect;
        SDL_Renderer* m_renderer;

        ///TO-DO make loop animation
        unsigned short m_duration;
        SDL_Texture* m_sprites;
        SDL_Rect m_srcRect;
        unsigned short m_frames;
        double m_timePerFrame;

        bool isFinished();

        void initAnimation(string configFile);
        void start();
        void update();
        void draw();
    protected:

    private:
        bool m_animationEnded;

        chrono::time_point<chrono::steady_clock> m_lastFrameTime = chrono::steady_clock::now();
        chrono::time_point<chrono::steady_clock> m_startFrame = chrono::steady_clock::now();
        chrono::duration<double> diff;
};

#endif // ANIMATOR_H
