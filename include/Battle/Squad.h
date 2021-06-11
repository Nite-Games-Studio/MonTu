#ifndef SQUAD_H
#define SQUAD_H

#include <string>
#include <fstream>
#include <iostream>
#include <stack>

#include "defines.h"
#include "Engine.h"
#include "Tile.h"
#include "HealthManager.h"

class Squad
{
    public:
        Squad();
        Squad(const Squad& model, coordinates* cameraOffset, Tile* tile, OWNER owner, HealthManager hm);
        virtual ~Squad();

        SDL_Renderer* m_renderer;
        SDL_Texture* m_objectTexture;
        SDL_Texture* m_playerTexture;
        SDL_Texture* m_enemyTexture;

        SDL_Rect m_objectRect;
        SDL_Rect m_presentRect;
        SDL_Rect m_healthBarRect;
        coordinates m_mapCoor;

        double m_speedMultiplier;

        //{WALK
        // parameters used ONLY in the walk function
        fcoordinates m_walkCoor;
        int m_directionAngle;
        //}
        HealthManager m_hm;

        coordinates* m_cameraOffset;

        SQUAD m_type;

        OWNER m_owner;
        short m_startNumberOfUnits;
        short m_numberOfUnits;
        double m_startSpeed;
        double m_speed;
        double m_startAttackRange;
        double m_attackRange;
        double m_startAttackDamage;
        double m_attackDamage;
        double m_startFaith;
        double m_faith;

        unsigned short m_idleAnimationRange = 5;
        unsigned short m_idleAnimationCounter = 0;
        bool m_moveUp = true;

        bool m_traveling;
        bool m_moved;
        bool m_attacked;

        Tile* m_tileTaken;
        // A tile that is used in the walk function
        // When we want to perform movement, goal tile is used to remember
        // on which tile we want to step and perform a slow movement,
        // instead of teleporting
        Tile* m_goalTile;

        stack<Tile*> m_path;

        int m_maxFramesPerStep = 100;
        int m_framesPerStep = 0;

        virtual void load(string configFile, SDL_Renderer* renderer);
        virtual void update();
        virtual void draw();
        virtual void attack(Squad* defender);
        void syncCoor();
        void idleAnimation();
        void walk();

    protected:

    private:
};

#endif // SQUAD_H
