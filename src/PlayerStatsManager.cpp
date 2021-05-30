#include "PlayerStatsManager.h"

PlayerStatsManager::PlayerStatsManager()
{
    //ctor
}

PlayerStatsManager::~PlayerStatsManager()
{
    //dtor
}

void PlayerStatsManager::init(string configFile)
{
    m_configFile = "config\\PlayerStats\\" + configFile;
}

void PlayerStatsManager::readAllValues()
{
    fstream stream;

    string tmp;

    stream.open(m_configFile.c_str());
    stream >> tmp >> m_money;
    m_tmps.push(tmp);
    stream >> tmp >> m_foodInCapital;
    m_tmps.push(tmp);
    stream.close();
}

void PlayerStatsManager::changeValues(PLAYER_STAT playerStat, int newValue)
{
    switch(playerStat)
    {
        case MONEY:m_money = newValue;
                    break;
        case FOOD: m_foodInCapital = newValue;
                    break;
        default :
                    cout << "no stat given in changeValues function \n";
    }
}

void PlayerStatsManager::writeAllValues()
{
    fstream stream;

    stream.open(m_configFile.c_str());
    stream << m_tmps.front() << " " << m_money << endl;
    m_tmps.pop();
    stream << m_tmps.front() << " " << m_foodInCapital << endl;
    m_tmps.pop();
    stream.close();
}
