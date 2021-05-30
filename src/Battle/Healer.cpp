#include "Healer.h"
#include "World.h"

extern World world;

Healer::Healer()
{
    //ctor
}

Healer::Healer(const Healer& model, coordinates* cameraOffset, Tile* tile, OWNER owner)
{
    m_startHealth = model.m_startHealth;
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

    m_health = m_startHealth;
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

    m_hm = model.m_hm;

    m_heal = model.m_heal;

    m_moved = false;
    m_shooted = false;
}

Healer::~Healer()
{
    //dtor
}

void Healer::heal()
{
    Tile* neighbor = NULL;
    for(int i = 0; i < 6; i++)
    {
        neighbor = world.m_battle.giveNeighbor(m_mapCoor, i);
        if(neighbor != NULL)
        {
            if(world.m_battle.findSquadByCoor(neighbor->m_mapCoordinates)!= NULL)
            {
                Squad* squad = world.m_battle.findSquadByCoor(neighbor->m_mapCoordinates);
                squad->m_health += m_heal;
            }
        }
    }
}
