#ifndef BUILDING_H
#define BUILDING_H

#include <chrono>

#include <Tile.h>

using namespace chrono;

class Building : public Tile
{
    public:
        Building();
        Building(const Building& model/*, Tile* tile, OWNER owner*/);
        virtual ~Building();

        short int m_height;
        short int m_width;
        short int m_adjustment;
        SDL_Rect m_drawState;

        string m_type;
        string m_tempConfigFile;
        string m_ownerImgP1;
        string m_ownerImgP2;
        string m_buildImg;
        string m_damageImg;
        SDL_Texture* m_ownerTextureP1;
        SDL_Texture* m_ownerTextureP2;
        SDL_Texture* m_buildTexture;
        SDL_Texture* m_damageTexture;

        short int m_duration;
        steady_clock::time_point m_currTimePoint;

        bool m_getPosition;
        bool m_useObject;

        short int m_maxHealth;
        short int m_healthPerState;
        short int m_health;
        short int m_states;
        short int m_maxState;
        short int m_currState;

        bool m_constructing;

        virtual void load(ifstream& stream);
        virtual void load(string configFile, SDL_Renderer* renderer);
        virtual void save(Building* building , short int position);
        virtual void update();
        virtual void draw(SDL_Renderer* renderer);

        virtual void limitHealth();
        virtual void select();
        virtual void getState();
        virtual void setTextureDetails(SDL_Renderer* renderer);

    protected:

    private:

};

#endif // BUILDING_H
