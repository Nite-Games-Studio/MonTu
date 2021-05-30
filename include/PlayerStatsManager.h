#ifndef PLAYERSTATSMANAGER_H
#define PLAYERSTATSMANAGER_H

#include <iostream>
#include <fstream>
#include <queue>

#include "defines.h"

using namespace std;

class PlayerStatsManager
{
    public:
        PlayerStatsManager();
        virtual ~PlayerStatsManager();

        string m_configFile;

        int m_foodInCapital;
        int m_money;

        queue <string> m_tmps;

        void init(string configFile);
        void readAllValues();
        void changeValues(PLAYER_STAT playerStat, int newValue);
        void writeAllValues();

    protected:

    private:
};

#endif // PLAYERSTATSMANAGER_H
