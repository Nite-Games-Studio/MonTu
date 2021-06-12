#ifndef CITY_H
#define CITY_H

#include <iostream>
#include <fstream>
#include <SDL.h>

using namespace std;

class City
{
public:
    City();
    City(const City& model);
    virtual ~City();

    SDL_Rect m_objRect;
    SDL_Texture* m_cityTexture;
    string m_cityName;

    void draw();
    void load(string configFile);

protected:

private:
};

#endif // CITY_H
