#ifndef BATTLE_H
#define BATTLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <SDL.h>

#include "EnemyAI.h"
#include "ArmyManager.h"
#include "PopUpWriter.h"

#include "Tile.h"
#include "Squad.h"
#include "SpearSquad.h"
#include "HookSquad.h"

#include "Particle.h"

class Battle
{
    public:
        Battle();
        virtual ~Battle();

        SDL_Renderer* m_renderer;

        short int m_colls;
        short int m_rows;
        OWNER m_playerTurn;
        int m_hexagonWidth;
        int m_hexagonHeight;

        EnemyAI m_enemyAI;
        ArmyManager m_armyManager;
        PopUpWriter m_popUpWriter;

        UI_object m_selectedTileUI;
        UI_object m_attackTileUI;
        UI_object m_skipTurnFillBtn;
        UI_object m_skipTurnTransBtn;

        bool m_showFillBtn;
        bool m_showAttackTiles;

        bool m_squadIsWalking;
        Squad* m_currentMovingSquad;

        coordinates m_selected;
        Squad* m_selectedSquad;
        Tile* m_selectedTile;

        // Those are the coordinates that we use for determining the neighbors of a tile
        coordinates directions[2][6];

        bool** m_unwalkableTiles;
        vector<vector<Tile*>> m_tiles;
        vector<Squad*> m_squads;
        vector<Particle*> m_particles;

        vector<Tile*> m_availableWalkTiles;
        vector<Tile*> m_availableShootTiles;

        bool canTravel(Squad* squad, coordinates desiredPosition);
        bool canShoot(Squad* squad, coordinates targetPosition);
        short angleToDirection(short angle);
        short angleToDirectionReverse(short angle);
        Tile* giveNeighbor(coordinates coor, int direction);
        coordinates* giveNeighborCoor(coordinates coor, int direction);
        Squad* giveNeighborSquad(coordinates coor, int directions);
        Squad* findSquadByCoor (coordinates coor);
        vector<Tile*> showAvailableWalkTiles(Squad* squad);
        vector<Tile*> showAvailableShootTiles(Squad* squad);

        void update();
        void draw();
        void cleaner();

        void initDirection(string configFile);
        void initBattle(string configFile);
        void initTiles(string configFile);
        void initGameSession(unsigned short mapIndex, unsigned short playerArmyIndex, unsigned short enemyArmyIndex);
        void selectTile();
        void initSquad(SQUAD type, coordinates mapCoor, unsigned short numberOfUnits, OWNER owner);
        void initSquads(string configFile);

        void checkForTurnSwitch();
        void switchTurn();
        void shoot(Squad* attackingSquad, Squad* defendingSquad);
        void squadActionsCheck();
        void takeDamage(Squad* attacker, Squad* defender);
    protected:

    private:
};

#endif // BATTLE_H
