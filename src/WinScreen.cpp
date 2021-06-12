#include "WinScreen.h"
#include "World.h"

extern World world;

WinScreen::WinScreen()
{

}

WinScreen::~WinScreen()
{

}

void WinScreen::init(string configFile, OWNER owner)
{
	configFile = "config\\" + configFile;

	fstream stream;
	string tmp, winImg, loseImg, buttonImg;

	stream.open(configFile.c_str());
	
	stream >> tmp >> winImg;
	stream >> tmp >> loseImg;
	stream >> tmp >> buttonImg;
	stream >> tmp >> m_button.objectRect.x >> m_button.objectRect.y >> m_button.objectRect.w >> m_button.objectRect.h;
	stream >> tmp >> widthChange >> heightChange;

	stream.close();

	m_winTexture = LoadTexture(winImg, world.m_main_renderer);
	m_loseTexture = LoadTexture(loseImg, world.m_main_renderer);
	m_button.objTexture = LoadTexture(buttonImg, world.m_main_renderer);

	if (owner == PLAYER1)
	{
		m_ownerStr = "player1";
	}
	else if(owner == PLAYER2)
	{
		m_ownerStr = "player2";
	}

	m_button.startRect = m_button.objectRect;

	m_button.bonusW = 1.6;
	m_button.bonusH = 0.9;
}

void WinScreen::update()
{
	if (checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_button.objectRect) && world.m_mouseIsPressed)
	{
		world.m_quitScene = true;
		world.m_gameState = WORLD_MAP;
	}

	world.m_menu.buttonHover(&m_button);
}

void WinScreen::draw(SDL_Renderer* renderer)
{
	if (m_ownerStr == "player1")
	{
		SDL_RenderCopy(renderer, m_winTexture, NULL, NULL);
	}
	else if (m_ownerStr == "player2")
	{
		SDL_RenderCopy(renderer, m_loseTexture, NULL, NULL);
	}

	SDL_RenderCopy(renderer, m_button.objTexture, NULL, &(m_button.objectRect));
}