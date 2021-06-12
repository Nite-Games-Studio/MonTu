#pragma once

#include <fstream>
#include <SDL.h>

#include "defines.h"

class WinScreen
{
public:
	WinScreen();
	virtual ~WinScreen();

	SDL_Rect m_ButtonRect;
	SDL_Texture* m_winTexture;
	SDL_Texture* m_loseTexture;
	
	void init(string configFile, OWNER owner);
	void draw();
	void update();

};