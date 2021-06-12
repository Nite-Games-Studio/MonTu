#include "HookSquad.h"

#include "World.h"

extern World world;

HookSquad::HookSquad()
{
    //dtor
}

HookSquad::HookSquad(const HookSquad& model, coordinates* cameraOffset, Tile* tile, OWNER owner, HealthManager hm)
{
    m_startNumberOfUnits = model.m_startNumberOfUnits;
    m_startAttackDamage = model.m_startAttackDamage;
    m_startSpeed = model.m_startSpeed;
    m_startAttackRange = model.m_startAttackRange;
    m_startFaith = model.m_startFaith;

    m_cameraOffset = cameraOffset;

    m_tileTaken = tile;

    m_mapCoor = tile->m_mapCoordinates;

    m_objectRect.x = tile->m_drawCoordinates.x;
    m_objectRect.y = tile->m_drawCoordinates.y;
    m_objectRect.w = model.m_objectRect.w;
    m_objectRect.h = model.m_objectRect.h;

    m_numberOfUnits = m_startNumberOfUnits;
    m_attackDamage = m_startAttackDamage;
    m_speed = m_startSpeed;
    m_attackRange = m_startAttackRange;
    m_faith = m_startFaith;

    if(owner == PLAYER1)
    {
        m_objectTexture = model.m_playerTexture;
    }
    else
    {
        m_objectTexture = model.m_enemyTexture;
    }

    m_renderer = model.m_renderer;

    m_owner = owner;

    m_hm = hm;
    m_hm.m_numberOfUnits = &m_numberOfUnits;
    m_hm.updateUnitsNumber();

    m_moved = false;
    m_attacked = false;
}


HookSquad::~HookSquad()
{
    //dtor
}

void HookSquad::attack(Squad* defender)
{
    coordinates defenderCoor;
    defenderCoor.x = defender->m_objectRect.x;
    defenderCoor.y = defender->m_objectRect.y;

    coordinates attackerCoor;
    attackerCoor.x = m_objectRect.x;
    attackerCoor.y = m_objectRect.y;

    defenderCoor - attackerCoor;

    double angle = returnAngleByCoordinates(defenderCoor);

    Tile* neighbor = NULL;

    neighbor = world.m_battle.giveNeighbor(defender->m_mapCoor, world.m_battle.angleToDirectionReverse(angle));

    if(neighbor != NULL && world.m_battle.findSquadByCoor(neighbor->m_mapCoordinates) == NULL)
    {
        defender->m_path.push(neighbor);
    }
}

