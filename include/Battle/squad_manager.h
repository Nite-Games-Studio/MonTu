#ifndef SQUAD_MANAGER_H
#define SQUAD_MANAGER_H
#include "defines.h"
#include<cstring>
#include <cstdio>
using namespace std;

class squad_manager
{
    public:
        squad_manager();
        virtual ~squad_manager();
        int squadCount;
        vector <soldier> soldiers;

        ///squad api

        void init();///
        int addSquad(string city);///
        squad_data readSquad(int squad);//
        void changeState(int squad, string newCity);///
        void deleteSquad(int squad);///

        ///soldier api

        void addSoldier(int squad, int numberOfSoldiers, SQUAD type, int coordX, int coordY);///
        void changeUnits(int squad, SQUAD unitType, int currCordX, int currCordY, int currUnits, int value);///
        void changeSoldierCoords(int squad, int currCordX, int currCordY, int newCoordX, int newCoordY);///
        void killSoldier(int squad, int type, int coordX, int coordY);///

    protected:

    private:
};

#endif // SQUAD_MANAGER_H
