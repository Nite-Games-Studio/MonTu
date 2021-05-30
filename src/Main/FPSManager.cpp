#include "FPSManager.h"
#include <SDL.h>

FPSManager::FPSManager()
{
    //ctor
}

FPSManager::~FPSManager()
{
    //dtor
}

void FPSManager::start() {
	stime = (int)SDL_GetTicks();
}

int FPSManager::elapsedTime() {
	return ((int)SDL_GetTicks()) - stime;
}
