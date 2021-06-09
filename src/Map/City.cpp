#include "City.h"
#include "World.h"

extern World world;

City::City()
{

}

City::City(const City& model)
{
	m_objRect = model.m_objRect;
	m_cityTexture = model.m_cityTexture;
}

City::~City()
{

}

void City::load(string configFile)
{
	configFile = "config\\" + configFile;
	fstream stream;
	string tmp, img;

	stream.open(configFile.c_str());

	stream >> tmp >> m_objRect.x >> m_objRect.y >> m_objRect.w >> m_objRect.h;
	stream >> tmp >> img;

	stream.close();

	m_cityTexture = LoadTexture(img, world.m_main_renderer);
}


void City::draw()
{
	SDL_RenderCopy(world.m_main_renderer, m_cityTexture, NULL, &(m_objRect));
}