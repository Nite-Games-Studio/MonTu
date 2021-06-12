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
	string tmp, winImg, loseImg;

	stream.open(configFile.c_str());
	

	stream.close();
}

