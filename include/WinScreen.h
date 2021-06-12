#pragma once

#include <fstream>
#include <SDL.h>

#include "defines.h"#
#include "Engine.h"

class WinScreen
{
public:
	WinScreen();
	virtual ~WinScreen();

	Button m_button;
	SDL_Texture* m_winTexture;
	SDL_Texture* m_loseTexture;

	string m_ownerStr;

	int widthChange, heightChange;
	
	void init(string configFile, OWNER owner);
	void draw(SDL_Renderer* renderer);
	void update();

};