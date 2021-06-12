#include "ArmyManager.h"
#include "World.h"

extern World world;

ArmyManager::ArmyManager()
{
    //ctor
}

ArmyManager::~ArmyManager()
{
    //dtor
}

void ArmyManager::deployArmy(unsigned short index, OWNER owner)
{
    if(owner == PLAYER1)
    {
        armyFormationPath = "data\\squads\\soldier data\\soldier_data_" + to_string(index) + ".txt";
    }
    else
    {
        armyFormationPath = "data\\enemies\\enemy_army_" + to_string(index) + ".txt";
    }

    fstream stream;

    char** formation = new char* [world.m_battle.m_rows];

    for (int i = 0; i < world.m_battle.m_rows; i++)
    {
        formation[i] = new char[3];

    }

    string tmp;
    coordinates coorBuff;
    int intBuff;
    SQUAD enumBuff;

    stream.open(armyFormationPath.c_str());
    
    if (stream.is_open())
    {
        while (!stream.eof())
        {
            stream >> intBuff;
            enumBuff = (SQUAD)intBuff;
            cout << intBuff << " ";
            stream >> tmp >> intBuff >> tmp >> coorBuff.y >> coorBuff.x;
            cout << tmp << " " << intBuff << " " << tmp << " " << coorBuff.y << " " << coorBuff.x << endl;
            world.m_battle.initSquad(enumBuff, coorBuff, intBuff, owner);
        }
        stream.close();
    }
    else
    {
        cout << "FATAL ERROR: File not found in: " << armyFormationPath << endl;
    }
}
