#include "SoundManager.h"

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::init(string configFile)
{
    configFile = "config\\" + configFile;
    fstream stream;
    string tmp;

    stream.open(configFile.c_str());

    stream >> tmp >> General_Music_str;

    stream.close();

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
    {
        printf("%s", Mix_GetError());
    }

    General = Mix_LoadWAV(("music\\" + General_Music_str).c_str());

}

void SoundManager::play_sound(string sound)
{
    Mix_AllocateChannels(16);

    if (sound == General_Music_str)
    {
        Mix_PlayChannel(7, General, INT_MAX);
        Mix_Volume(7, 10);
    }
}

void SoundManager::destroy()
{
    Mix_FreeChunk(General);
    General = NULL;

    Mix_Quit();
}
