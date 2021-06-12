#include "Squad.h"
#include "Particle.h"
#include "World.h"
extern World world;

Squad::Squad()
{
    m_goalTile = nullptr;
    m_speedMultiplier = 1.0;
}

Squad::~Squad()
{

}

Squad::Squad(const Squad& model, coordinates* cameraOffset, Tile* tile, OWNER owner, HealthManager hm)
{
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
        m_maxFramesPerStep /= 10;
        m_speedMultiplier = 1.5f;
    }

    m_renderer = model.m_renderer;

    m_owner = owner;

    m_hm = hm;
    m_hm.m_numberOfUnits = &m_numberOfUnits;
    m_hm.updateUnitsNumber();

    m_moved = false;
    m_attacked = false;

    m_goalTile = nullptr;
}


void Squad::load(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;
    ifstream stream;
    string tmp, imgPlayer, imgEnemy;
    stream.open(configFile.c_str());

    stream >> tmp >> imgPlayer >> imgEnemy;
    stream >> tmp >> m_objectRect.w >> m_objectRect.h;

    stream >> tmp >> m_startAttackDamage;
    stream >> tmp >> m_startSpeed;
    stream >> tmp >> m_startAttackRange;
    stream >> tmp >> m_startFaith;

    stream.close();

    m_playerTexture = LoadTexture(imgPlayer, renderer);
    m_enemyTexture = LoadTexture(imgEnemy, renderer);

    m_renderer = renderer;
}

void Squad::update()
{
//    if (!(m_path.empty()) && m_framesPerStep >= m_maxFramesPerStep)
//    {
//         cout << "INFO: The size of the walk queue is " << m_path.size() << endl;
//        m_tileTaken = m_path.top();
//
//        m_path.pop();
//
//        m_mapCoor = m_tileTaken->m_mapCoordinates;
//
//        m_objectRect.x = m_tileTaken->m_drawCoordinates.x;
//        m_objectRect.y = m_tileTaken->m_drawCoordinates.y;
//
//        m_framesPerStep = 0;
//    }
//    else if (!(m_path.empty()))
//    {
//        m_framesPerStep ++;
//    }
//    idleAnimation();
    walk();
}

void Squad::draw()
{
    m_presentRect.x = m_objectRect.x + m_cameraOffset->x;
    m_presentRect.y = m_objectRect.y + m_cameraOffset->y;
    m_presentRect.w = m_objectRect.w;
    m_presentRect.h = m_objectRect.h;
    SDL_RenderCopy(m_renderer, m_objectTexture, NULL, &m_presentRect);
    m_hm.drawHealthbar(m_renderer, m_presentRect);
}

void Squad::idleAnimation()
{
    if(!m_moved || !m_attacked)
    {
        if(m_moveUp)
        {
            if(m_idleAnimationCounter >= m_idleAnimationRange)
            {
                m_moveUp = false;
            }
            else
            {
                m_objectRect.y += 1;
                m_idleAnimationCounter += 1;
            }
        }
        else
        {
            if(m_idleAnimationCounter <= -m_idleAnimationRange)
            {
                m_moveUp = true;
            }
            else
            {
                m_objectRect.y -= 1;
                m_idleAnimationCounter -= 1;
            }
        }
    }
}

void Squad::attack(Squad* defender)
{
    defender->m_hm.takeDamage(m_attackDamage);
    m_attacked = true;
    m_moved = true;
    Particle* particle = new Particle(*world.m_configManager.modelMeleeHit, defender->m_objectRect);
    particle->start();
    world.m_battle.m_particles.push_back(particle);
}

void Squad::syncCoor()
{
}

void Squad::walk()
{
    if(m_goalTile == nullptr)
    {
        if(!(m_path.empty()) && !world.m_battle.m_squadIsWalking)
        {
            // Loading the goalTile in the begging of the walk
            m_goalTile = m_path.top();
            m_tileTaken = m_path.top();
            m_path.pop();

            coordinates direction;
            direction.x = m_goalTile->m_objectRect.x - m_objectRect.x;
            direction.y = m_goalTile->m_objectRect.y - m_objectRect.y;

            m_directionAngle = returnAngleByCoordinates(direction);

            m_walkCoor.x = m_objectRect.x;
            m_walkCoor.y = m_objectRect.y;

            world.m_battle.m_squadIsWalking = true;
        }
    }else{
        m_walkCoor.x += sin(m_directionAngle * PI / 180) * 4.5;
        m_walkCoor.y -= cos(m_directionAngle * PI / 180) * 4.5;

        m_objectRect.x = m_walkCoor.x;
        m_objectRect.y = m_walkCoor.y;

        if(distance(m_objectRect, m_goalTile->m_objectRect) < 6)
        {
            // We are close enough to the target
            m_objectRect.x = m_goalTile->m_objectRect.x;
            m_objectRect.y = m_goalTile->m_objectRect.y;

            // Load the next tile, if there is one
            if(!(m_path.empty()))
            {
                m_goalTile = m_path.top();
                m_tileTaken = m_path.top();
                m_path.pop();

                coordinates direction;
                direction.x = m_goalTile->m_objectRect.x - m_objectRect.x;
                direction.y = m_goalTile->m_objectRect.y - m_objectRect.y;

                m_directionAngle = returnAngleByCoordinates(direction);

                m_walkCoor.x = m_objectRect.x;
                m_walkCoor.y = m_objectRect.y;
            }else{
                world.m_battle.m_squadIsWalking = false;
                m_goalTile = nullptr;
                if (m_attackGoal != nullptr)
                {
                    attack(m_attackGoal);
                }
            }
        }
    }
}
