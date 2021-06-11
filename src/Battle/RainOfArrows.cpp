#include "RainOfArrows.h"
#include "World.h"

extern World world;

RainOfArrows::RainOfArrows()
{
    //ctor
}

RainOfArrows::~RainOfArrows()
{
    //dtor
}

/*
* The function deals
*/
void RainOfArrows::attack(Squad* defender)
{
    vector<Squad*> surrounding;

    for (int i = 0; i < 6; i ++)
    {
        if (world.m_battle.giveNeighbor(defender->m_mapCoor, i) != nullptr)
        {
            surrounding.push_back(world.m_battle.giveNeighborSquad(defender->m_mapCoor, i));
        }
    }

    defender->m_hm.takeDamage(m_attackDamage);

    for(int i = 0; i < surrounding.size(); i ++)
    {
        surrounding[i]->m_hm.takeDamage(m_attackDamage);
    }
}
