#include "EnemyAI.h"
#include "World.h"

#include "defines.h"

extern World world;

EnemyAI::EnemyAI()
{
    //ctor
}

EnemyAI::~EnemyAI()
{
    //dtor
}

void EnemyAI::init(string configFile)
{
    configFile = "config//" + configFile;

    fstream stream;
    stream.open(configFile);

    stream >> m_teamwork >> m_aggression >> valueSquadMap[ARCHER] >> valueSquadMap[WARRIOR] >> valueSquadMap[SPEARMEN] >> valueSquadMap[CROSSBOWMEN] >> valueSquadMap[KNIGHTS];

    stream.close();
}

void EnemyAI::clearAI()
{
    m_battlefield.clear();
    m_aiSquads.clear();
    m_playerSquads.clear();
}

void EnemyAI::takeBattlefield()
{
    clearAI();
    m_battlefield = world.m_battle.m_tiles;
    for (vector<Squad*> :: iterator it = world.m_battle.m_squads.begin(); it != world.m_battle.m_squads.end(); it++)
    {
        if ((*it)->m_owner == PLAYER1)
        {
            m_playerSquads.push_back((*it));
        }
        else
        {
            m_aiSquads.push_back((*it));
        }
    }
}

void EnemyAI::chooseBestActionForUnit(Squad* squad, unsigned short unitIndex)
{
    startPosition = squad->m_tileTaken;
    availableToWalkTiles = world.m_battle.showAvailableWalkTiles(squad);
    bestPosition = nullptr;
    bestVictim = nullptr;
    bestScore = 0;
    bool willAttack = false;
    bestDistanceScore = INT_MAX;
    distanceScore = 0;
    cout << "START OF MOVE CHOOSE \n";
    /// Go trough all available to walk tiles and choose the best move
    for (vector <Tile*> :: iterator it = availableToWalkTiles.begin(); it != availableToWalkTiles.end(); it ++)
    {
        distanceScore = 0;
        if(!willAttack)
        {
            for(int i = 0; i < m_playerSquads.size(); i ++)
            {
                distanceScore += distance((*it)->m_objectRect, m_playerSquads[i]->m_objectRect);
            }
            if (distanceScore < bestDistanceScore)
            {
                bestDistanceScore = distanceScore;
                bestPosition = (*it);
            }
        }

        /// Move the squad
        squad->m_mapCoor = (*it)->m_mapCoordinates;
        squad->m_tileTaken = (*it);
        score = 0;
        for(vector <Squad*> :: iterator squadIt = m_playerSquads.begin(); squadIt != m_playerSquads.end(); squadIt ++ )
        {
            /// calculate the score for this move
            if (world.m_battle.canShoot(squad, (*squadIt)->m_mapCoor))
            {
                /// f( currentHealth / maxHealth * peaceValue / count )

                /// TODO: add numberOfUnits
                for(int i = 0; i < m_aiSquads.size(); i ++)
                {
                    score += distance(squad->m_tileTaken->m_objectRect, m_aiSquads[i]->m_objectRect);
                }
                score *= m_teamwork;
                score += ((double)(*squadIt) -> m_numberOfUnits) * valueSquadMap[(*squadIt) -> m_type] * m_aggression;

                /// TODO: if the performance is good, check all possible player actions for attack on the ai
                if (world.m_battle.canShoot((*squadIt), squad->m_mapCoor))
                {
                    score -= (*squadIt)->m_attackDamage * valueSquadMap[squad -> m_type] * (50 - m_aggression);
                }
                willAttack = true;
                if (score > bestScore)
                {
                    cout << "Best pos cahnge!" << endl;
                    bestScore = score;
                    bestPosition = (*it);
                    bestVictim = (*squadIt);
                }
            }
        }
    }

    squad->m_tileTaken = startPosition;
    squad->m_mapCoor = squad->m_tileTaken->m_mapCoordinates;
    if(willAttack)
    {
        /// CAN ATTACK
        world.m_battle.canTravel(squad, bestPosition->m_mapCoordinates);
        if (bestVictim != NULL)
        {
            squad->attack(bestVictim);
        }
    }else{
        /// NO ENEMIES FOR AN ATTACK
        world.m_battle.canTravel(squad, bestPosition->m_mapCoordinates);
        squad->m_attacked = true;
    }
}

void EnemyAI::makeTurn()
{
    for (unsigned short i = 0; i < m_aiSquads.size(); i ++)
    {
        chooseBestActionForUnit(m_aiSquads[i], i);
    }
}

void EnemyAI::returnBattlefield()
{
    world.m_battle.m_squads.clear();
    for (vector<Squad*> :: iterator it = m_aiSquads.begin(); it != m_aiSquads.end(); it ++)
    {
        world.m_battle.m_squads.push_back((*it));
    }
    for (vector<Squad*> :: iterator it = m_playerSquads.begin(); it != m_playerSquads.end(); it ++)
    {
        world.m_battle.m_squads.push_back((*it));
    }
}
