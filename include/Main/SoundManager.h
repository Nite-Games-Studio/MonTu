#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <SDL.h>

#include <fstream>
#include <string>
#include <iostream>

using namespace std;

class SoundManager
{
public:
    SoundManager();
    virtual ~SoundManager();

    void init(string configFile);
    void destroy();
    void play_sound(string sound);

    string General_Music_str;

    Mix_Chunk* General;

protected:

private:
};

#endif
