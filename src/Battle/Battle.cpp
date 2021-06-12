#include "Battle.h"
#include "World.h"

extern World world;

Battle::Battle()
{
    m_selected.x = 0;
    m_selected.y = 0;
    m_selectedSquad = nullptr;
    m_playerTurn = PLAYER1;
    m_showFillBtn = true;
    m_showAttackTiles = false;
    m_squadIsWalking = false;
    m_currentMovingSquad = nullptr;
}

Battle::~Battle()
{
    //dtor
}
//{ INIT
void Battle::initBattle(string configFile)
{
    m_renderer = world.m_main_renderer;

    configFile = "config\\" + configFile;

    fstream stream;

    string tmp;

    string selectedImg, attackTileImg;
    string skipBtnFillImg;
    string skipBtnTransImg;

    stream.open(configFile.c_str());
    stream >> tmp >> m_colls >> m_rows;
    stream >> tmp >> selectedImg;
    stream >> tmp >> attackTileImg;
    stream >> tmp >> skipBtnFillImg;
    stream >> tmp >> skipBtnTransImg;
    stream >> tmp >> m_skipTurnFillBtn.objRect.x >> m_skipTurnFillBtn.objRect.y >> m_skipTurnFillBtn.objRect.w >> m_skipTurnFillBtn.objRect.h;
    stream.close();

    D(m_colls);
    D(m_rows);

    m_popUpWriter.init("PopUpWriter.txt", m_renderer);


    m_unwalkableTiles = new bool*[m_rows];

    for(short i = 0; i < m_rows; i ++)
    {
        m_unwalkableTiles[i] = new bool[m_colls]();
    }

    m_selectedTileUI.objTexture = LoadTexture(selectedImg, m_renderer);
    m_attackTileUI.objTexture = LoadTexture(attackTileImg, m_renderer);
    m_skipTurnFillBtn.objTexture = LoadTexture(skipBtnFillImg, m_renderer);
    m_skipTurnTransBtn.objTexture = LoadTexture(skipBtnTransImg, m_renderer);

    m_skipTurnTransBtn.objRect = m_skipTurnFillBtn.objRect;

    initDirection("directions.txt");
}

void Battle::initDirection(string configFile)
{
    // Declaring the value of the directions used for the giveNeighbor function
    configFile = "config\\" + configFile;
    fstream stream;

    stream.open(configFile.c_str());

    for(int i = 0; i < 2; i ++)
    {
        for (int j = 0; j < 6; j++)
        {
            stream >> directions[i][j].x >> directions[i][j].y;
        }
    }
    stream.close();
}

void Battle::initGameSession(unsigned short mapIndex, unsigned short playerArmyIndex, unsigned short enemyArmyIndex)
{
    cout << "---------INIT SESSION--------- \n";
    m_enemyAI.init("enemyAI.txt");
    m_armyManager.deployArmy(playerArmyIndex, PLAYER1);
    m_armyManager.deployArmy(enemyArmyIndex, PLAYER2);
    cout << "--------INITED SESSION-------- \n";
}

void Battle::initTiles(string configFile)
{
    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    short int sizeOfHexagon;

    stream.open(configFile.c_str());
    stream >> tmp >> sizeOfHexagon;
    stream.close();

    m_hexagonWidth = sqrt(3) * sizeOfHexagon;
    m_hexagonHeight = 2 * sizeOfHexagon;


    Tile* tile = NULL;
    Building* building = NULL;
    SDL_Point* buffPoint = NULL;

    for (short int r = 0; r < m_rows; r ++)
    {
        m_tiles.push_back(vector<Tile*>());
        for (short int c = 0; c < m_colls; c ++)
        {
            switch(world.field[c][r])
            {
            case 'G':
                tile = new Tile(*(world.m_configManager.modelTileGrass), &(world.m_cameraOffset));
                break;
            case 'W':
                tile = new Tile(*(world.m_configManager.modelTileWater), &(world.m_cameraOffset));
                break;
            case 'M':
                tile = new Tile(*(world.m_configManager.modelTileMountain), &(world.m_cameraOffset));
                break;
            case 'F':
                tile = new Tile(*(world.m_configManager.modelTileForest), &(world.m_cameraOffset));
                break;
            case 'D':
                tile = new Tile(*(world.m_configManager.modelTileDesert), &(world.m_cameraOffset));
                break;
            case 'S':
                tile = new Tile(*(world.m_configManager.modelTileStone), &(world.m_cameraOffset));
                break;
            case 'V':
                tile = new Tile(*(world.m_configManager.modelTileVolcano), &(world.m_cameraOffset));
                break;
            case 'L':
                tile = new Tile(*(world.m_configManager.modelTileLava), &(world.m_cameraOffset));
                break;
            }
            m_tiles[r].push_back(tile);

            if(r % 2 == 0)
            {
                tile->m_drawCoordinates.x = c * m_hexagonWidth;
            }
            else
            {
                tile->m_drawCoordinates.x = m_hexagonWidth / 2 + c * m_hexagonWidth;
            }

            tile->m_drawCoordinates.y = r * m_hexagonHeight * 3 / 4;
            tile->m_mapCoordinates.x = c;
            tile->m_mapCoordinates.y = r;

            CoordinatesToRect(tile->m_drawCoordinates, tile->m_objectRect);
            tile->m_objectRect.w = m_hexagonWidth;
            tile->m_objectRect.h = m_hexagonHeight;

            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + m_hexagonWidth / 2;
            buffPoint->y = tile->m_drawCoordinates.y + 0;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + 0;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight / 4;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + 0;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight / 4 * 3;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + m_hexagonWidth / 2;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + m_hexagonWidth;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight / 4 * 3;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + m_hexagonWidth;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight / 4;
            tile->m_collisionPoints.push_back(buffPoint);
        }
    }
}
//}

//{ MAIN
void Battle::update()
{
    world.cameraShake();

    selectTile();

    m_selectedTileUI.objRect = m_tiles[m_selected.y][m_selected.x]->m_objectRect;
    squadActionsCheck();
    if(world.m_mouseIsPressed)
    {
        if(m_showFillBtn && checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_skipTurnFillBtn.objRect))
        {
            switchTurn();
        }
    }
    checkForTurnSwitch();

    for(int i = 0; i < m_particles.size(); i ++)
    {
        m_particles[i]->update();
    }

    cleaner();
}

void Battle::draw()
{
    SDL_RenderClear(m_renderer);

    for(vector<vector <Tile*> > :: iterator vit = m_tiles.begin(); vit != m_tiles.end(); vit++)
    {
        for(vector<Tile*> :: iterator it = (*vit).begin(); it != (*vit).end(); it++)
        {
            (*it) -> draw(m_renderer);
        }
    }
    SDL_RenderCopy(m_renderer, m_selectedTileUI.objTexture, NULL, &(m_selectedTileUI.objRect));

    for(vector <Tile*> :: iterator it = m_availableWalkTiles.begin(); it != m_availableWalkTiles.end(); it++)
    {
        SDL_RenderCopy(m_renderer, m_selectedTileUI.objTexture, NULL, &((*it) -> m_objectRect));
    }

    for(vector <Tile*> :: iterator it = m_availableShootTiles.begin(); it != m_availableShootTiles.end(); it++)
    {
        SDL_RenderCopy(m_renderer, m_attackTileUI.objTexture, NULL, &((*it) -> m_objectRect));
    }

    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        (*it) -> draw();
    }

    for(int i = 0; i < m_particles.size(); i ++)
    {
        m_particles[i]->draw();
    }


    m_popUpWriter.draw(m_tiles[m_selected.y][m_selected.x]->m_objectRect, m_popUpWriter.m_buildingListRect, m_popUpWriter.m_buildingListTexture);

    if (m_showFillBtn)
    {
        SDL_RenderCopy(m_renderer, m_skipTurnFillBtn.objTexture, NULL, &(m_skipTurnFillBtn.objRect));
    }

    SDL_RenderPresent(m_renderer);

}

void Battle::cleaner()
{
    for(int i = 0; i < m_squads.size(); i ++)
    {
        if(m_squads[i]->m_numberOfUnits <= 0)
        {
            //TODO(konstantin #3): add dead animation
            delete m_squads[i];
            m_squads.erase(m_squads.begin() + i);
            i--;
        }
    }
    for (int i = 0; i < m_particles.size(); i ++)
    {
        if(m_particles[i]->isFinished())
        {
            // TODO: UNKNOWN BUG CAUSE
            //delete m_particles[i];
            m_particles.erase(m_particles.begin() + i);
            i--;
        }
    }
}
//}


void Battle::selectTile()
{
    if(world.m_mouseIsPressed)
    {
        for(short int r = 0; r < m_tiles.size(); r++)
        {
            for(short int c = 0; c < m_tiles[r].size(); c++)
            {
                if(isInsideAHexagon(m_tiles[r][c]->m_collisionPoints, LoadPoint(world.m_mouse)))
                {
                    m_popUpWriter.m_presentBuildingList = (m_selected.x == c && m_selected.y == r) ? true : false;
                    cout << "selected " << c << " " << r << endl;

                    m_selected.x = c;
                    m_selected.y = r;
                }
            }
        }
    }
}

//{ HELPERS
Tile* Battle::giveNeighbor(coordinates coor, int direction)
{
    /**
    the value of direction should be the following:
    0 - right
    1 - top right
    2 - top left
    3 - left
    4 - down left
    5 - down right
    */

    if(direction < 0 && direction > 5)
    {
        return nullptr;
    }
    int evenR = (coor.y % 2 == 0) ? 0 : 1;
    //selects the coordinates that we add to the current position
    coordinates addedCoor = directions[evenR][direction];

    if(coor.y + addedCoor.y < 0 || coor.y + addedCoor.y >= m_rows || coor.x + addedCoor.x < 0 || coor.x + addedCoor.x >= m_colls)
    {
        return nullptr;
    }
    return m_tiles[coor.y + addedCoor.y][coor.x + addedCoor.x];
}

coordinates* Battle::giveNeighborCoor(coordinates coor, int direction)
{
    /**
    the value of direction should be the following:
    0 - right
    1 - top right
    2 - top left
    3 - left
    4 - down left
    5 - down right
    */

    int evenR = (coor.y % 2 == 0) ? 0 : 1;
    //selects the coordinates that we add to the current position
    coordinates addedCoor = directions[evenR][direction];

    if(coor.y + addedCoor.y < 0 || coor.y + addedCoor.y >= m_rows || coor.x + addedCoor.x < 0 || coor.x + addedCoor.x >= m_colls)
    {
        return NULL;
    }

    coor.x += addedCoor.x;
    coor.y += addedCoor.y;

    return &m_tiles[coor.y][coor.x]->m_mapCoordinates;
}

Squad* Battle::giveNeighborSquad(coordinates coor, int direction)
{
    /**
    the value of direction should be the following:
    0 - right
    1 - top right
    2 - top left
    3 - left
    4 - down left
    5 - down right
    */

    int evenR = (coor.y % 2 == 0) ? 0 : 1;
    //selects the coordinates that we add to the current position
    coordinates addedCoor = directions[evenR][direction];

    if(coor.y + addedCoor.y < 0 || coor.y + addedCoor.y >= m_rows || coor.x + addedCoor.x < 0 || coor.x + addedCoor.x >= m_colls)
    {
        return NULL;
    }

    coor.x += addedCoor.x;
    coor.y += addedCoor.y;

    Squad* squad = findSquadByCoor(coor);
    if(squad != nullptr)
    return squad;
}

// Function used to determine the direction of a vector.
// For example we want to find the direction of a vector
// coming from hexagon A to hexagon B.
// We use our formula for finding angle between two points.
// This function will tell us the direction of this vector.
// It is a number between 0 and 5 (because the hexagon has 6 sides)
// You can check any of the giveNeighbor functions to see what is the
// side according to the return value of the function
short Battle::angleToDirection(short angle)
{
    if (angle > 0 && angle < 60)
    {
        return 1;
    }
    if (angle > 60 && angle < 120)
    {
        return 0;
    }
    if (angle > 120 && angle < 180)
    {
        return 5;
    }
    if (angle < 0 && angle > -60)
    {
        return 2;
    }
    if (angle < -60 && angle > -120)
    {
        return 3;
    }
    if (angle < -120 && angle > -180)
    {
        return 4;
    }
    return -1;
}

short Battle::angleToDirectionReverse(short angle)
{
    if (angle > 0 && angle < 60)
    {
        return 4;
    }
    if (angle > 60 && angle < 120)
    {
        return 3;
    }
    if (angle > 120 && angle < 180)
    {
        return 2;
    }
    if (angle < 0 && angle > -60)
    {
        return 5;
    }
    if (angle < -60 && angle > -120)
    {
        return 0;
    }
    if (angle < -120 && angle > -180)
    {
        return 1;
    }
    return -1;
}


bool Battle::canTravel(Squad* squad, coordinates desiredPosition)
{
    if(m_unwalkableTiles[desiredPosition.y][desiredPosition.y])
    {
        return false;
    }
    int** movementMap = new int* [m_rows];

    for (int i = 0; i < m_rows; i++)
    {

        movementMap[i] = new int[m_colls];

    }
    // Takes the position and speed of the Squad
    coordinates position = squad->m_tileTaken->m_mapCoordinates;
    int movement = squad->m_speed;
    // Makes all tiles uncrossable (giving impossible values)
    for (short int r = 0; r < m_rows; r ++)
    {
        for (short int c = 0; c < m_colls; c ++)
        {
            movementMap[r][c] = -1;
        }
    }
    // We need 0 movement for traveling to our own position
    movementMap[position.y][position.x] = 0;

    // Used to find the neighbor with smallest walkingDifficulty
    int minimum;
    int indexBuff;
    coordinates buff;
    bool valueFound = false;

    while(!valueFound)
    {
        valueFound = true;
        for (short int r = 0; r < m_rows; r ++)
        {
            for (short int c = 0; c < m_colls; c ++)
            {
                buff.x = c;
                buff.y = r;

                // If we have found a cell that has assigned value than we continue the algorithm
                if (movementMap[r][c] != -1)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        if(giveNeighbor(buff, i) != nullptr)
                        {
                            if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] != -1)
                            {
                                if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] > movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty)
                                {
                                    valueFound = false;
                                    movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                                }
                            }
                            else
                            {
                                valueFound = false;
                                movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                            }
                        }
                    }
                }
            }
        }
    }
    if (movementMap[desiredPosition.y][desiredPosition.x] <= movement)
    {
        movement -= movementMap[desiredPosition.y][desiredPosition.x];
        squad->m_speed = movement;
        if (movement == 0)
        {
            squad->m_moved = true;
            /// cout << "INFO: The squad has " << squad->m_speed << "left \n";
        }
        else
        {
            /// cout << "INFO: The squad has " << squad->m_speed << "left \n";
        }

        // We want to find the road tile by tile
        valueFound = false;
        // Here we store the path
        stack<Tile*> path;
        buff.x = desiredPosition.x;
        buff.y = desiredPosition.y;
        while(!valueFound)
        {
            short int minimumIndex = 0;
            short int minimumValue = 1000;
            for (int i = 0; i < 6; i++)
            {
                if (giveNeighbor(buff, i) != nullptr)
                {
                    if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] <
                    minimumValue )
                    {
                        minimumValue = movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x];
                        minimumIndex = i;
                    }
                }
            }
            // We store the road
            path.push(m_tiles[buff.y][buff.x]);
            /// cout << "INFO: Added tile " << buff.y << " " << buff.x << "to the queue \n";
            // Then we move to the next tile
            buff = giveNeighbor(buff, minimumIndex)->m_mapCoordinates;
            if(movementMap[buff.y][buff.x] == 0)
            {
                //m_walkableTiles[buff.y][buff.x] = false;
                valueFound = true;
            }
        }
        /// cout << "INFO: Moving is possible " << movementMap[buff.y][buff.x] << " " << movement << endl;
        squad->m_path = path;
        squad->m_mapCoor = desiredPosition;

        // Check if the squad is surrounded with impossible to walk-through tiles, but has speed
        if(movement != 0)
        {
            bool hasPossibleMove = false;
            for (int i = 0; i < 6; i++)
            {
                if (giveNeighbor(buff, i) != nullptr)
                {
                    if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x]
                    < squad->m_speed)
                    {
                        hasPossibleMove = true;
                    }
                }
            }
            if (!hasPossibleMove)
            {
                squad->m_moved = true;
            }
        }
        return true;
    }
    else
    {
        /// cout << "INFO: Moving is not possible " << movementMap[buff.y][buff.x] << " " << movement << endl;
        return false;
    }
}

bool Battle::canShoot(Squad* squad, coordinates targetPosition)
{

    vector<coordinates*> returnVector;

    vector<coordinates*>* thisStep;
    vector<coordinates*>* nextStep;

    bool foundResult = false;
    unsigned short step = 0;

    coordinates* buff = nullptr;
    thisStep = new vector<coordinates*>;
    thisStep->push_back(&squad->m_tileTaken->m_mapCoordinates);

    while(step < squad->m_attackRange)
    {
        nextStep = new vector<coordinates*>;
        for(unsigned short i = 0; i < thisStep->size() && !foundResult; i ++)
        {
            for(unsigned short j = 0; j < 6 && !foundResult; j ++)
            {
                buff = giveNeighborCoor(*(*thisStep)[i], j);
                if(buff != nullptr)
                {
                    for (unsigned short k = 0; k < returnVector.size(); k ++)
                    {
                        if(buff == returnVector[k])
                        {
                            buff = nullptr;
                            break;
                        }
                    }
                    if(buff != nullptr)
                    {
                        if(buff->x == targetPosition.x && buff->y == targetPosition.y)
                        {
                            foundResult = true;
                        }
                        returnVector.push_back(buff);
                        nextStep->push_back(buff);
                    }
                }
            }
        }
        thisStep = nextStep;
        step++;
    }
    /*
    while(step < squad->m_attackRange)
    {


        //cout << "HERE" << endl;

        for(unsigned short i = 0; i < surrounding.size(); i ++)
        {
           /// cout << __LINE__ << endl;
            for(unsigned short j = 0; j < 6; j ++)
            {
                buff = giveNeighborCoor((*surrounding[i]), j);
                if(buff != nullptr)
                {
                    ///cout << __LINE__ << " " << i << " " << surrounding.size() << endl;
                    for (unsigned short k = 0; k < surrounding.size(); k ++)
                    {
                        if(buff == surrounding[k])
                        {
                            ///cout << __LINE__ << endl;
                            buff = nullptr;
                            break;
                        }
                    }
                    if(buff != nullptr)
                    {
                        if(buff->x == targetPosition.x && buff->y == targetPosition.y)
                        {
                            foundResult = true;
                        }
                        cout << "WE HAVE " << buff->x << " " << buff->y << endl;
                        surrounding.push_back(buff);
                    }
                }
            }
        }
        step++;

    }
    */
    // If has the same owner
    if(!foundResult || findSquadByCoor(targetPosition) == nullptr)
    {
        return false;
        if (findSquadByCoor(targetPosition)->m_owner == squad->m_owner)
        {
            /// cout << "CAN'T SHOOT " << squad->m_tileTaken->m_mapCoordinates.x << " " << squad->m_tileTaken->m_mapCoordinates.y << " "  << targetPosition.x << " " << targetPosition.y <<  "\n";
            return false;
        }
    }
    /* A part of code the is not used for now
       it aims to give all the tiles between the attacker and defender
    // Using this bool to determine if the function is done
    bool finished = false;
    // The angle used to calculate how we will travel to the target position
    short rotationAngle;
    // We find the rotation angle by using the fnct in the Engine
    coordinates direction;
    coordinates drawTargetPos;
    drawTargetPos.x = m_tiles[targetPosition.y][targetPosition.x]->m_objectRect.x
                    + (m_tiles[targetPosition.y][targetPosition.x]->m_objectRect.w) / 2;
    drawTargetPos.y = m_tiles[targetPosition.y][targetPosition.x]->m_objectRect.y
                    + (m_tiles[targetPosition.y][targetPosition.x]->m_objectRect.h) / 2;

    // Find the direction of the shot
    direction.x = drawTargetPos.x - squad->m_objectRect.x;
    direction.y = drawTargetPos.y - squad->m_objectRect.y;
    // changing the target position to drawing coor
    rotationAngle = returnAngleByCoordinates(direction);
    /// cout << rotationAngle << endl;
    stack<Tile*> passing;

    // cast a ray in order to check for the passing tiles;
    coordinates scanRay;
    while(!finished)
    {
        position.x += (sin(rotationAngle * PI / 180) * 6);
        position.y -= (cos(rotationAngle * PI / 180) * 6);

        for(short r = 0; r < m_tiles.size(); r++)
        {
            for(short c = 0; c < m_tiles[r].size(); c++)
            {
                if(m_tiles[r][c] != passing.top)
                {
                    if(isInsideAHexagon(m_tiles[r][c]->m_collisionPoints, LoadPoint(position)))
                    {
                        passing.push_back(m_tiles[r][c]);
                        /// cout << logicalTargetPosition.y << " " << logicalTargetPosition.x << endl;
                        if(c == targetPosition.x && r == targetPosition.y)
                        {
                            finished = true;
                        }
                    }
                }
            }
        }
    }
    */
    return true;
}

Squad* Battle::findSquadByCoor(coordinates coor)
{
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        if((*it)->m_mapCoor == coor)
        {
            return (*it);
        }
    }
    return nullptr;
}
//}

vector<Tile*> Battle::showAvailableWalkTiles(Squad* squad)
{
    // take the position and the speed
    coordinates position = squad->m_mapCoor;
    int movement = squad->m_speed;
    // The vector, that we return
    vector<Tile*> returnVector;    
    int** movementMap = new int* [m_rows];

    for (int i = 0; i < m_rows; i++)
    {

        movementMap[i] = new int[m_colls];

    }
    // Makes all tiles uncrossable (giving impossible values)
    for (short int r = 0; r < m_rows; r ++)
    {
        for (short int c = 0; c < m_colls; c ++)
        {
            movementMap[r][c] = -1;
        }
    }
    // We need 0 movement for traveling to our own position
    movementMap[position.y][position.x] = 0;

    // Used to find the neighbor with smallest walkingDifficulty
    int minimum;
    int indexBuff;
    coordinates buff;
    bool valueFound = false;

    while(!valueFound)
    {
        valueFound = true;
        for (short int r = 0; r < m_rows; r ++)
        {
            for (short int c = 0; c < m_colls; c ++)
            {
                // We set the value to true as default, because if we don't change any values in
                // the movementMap than we have found the value of all cells
                buff.x = c;
                buff.y = r;

                // If we have found a cell that has assigned value than we continue the algorithm
                if (movementMap[r][c] != -1)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        if(giveNeighbor(buff, i) != nullptr)
                        {
                            if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] != -1)
                            {
                                if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] > movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty)
                                {
                                    valueFound = false;
                                    movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                                }
                            }
                            else
                            {
                                valueFound = false;
                                movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                            }
                        }
                    }
                }

                ///cout << r << " " << c << " " << movementMap[r][c] << endl;
            }
        }
    }
    for (short int r = 0; r < m_rows; r ++)
    {
        for (short int c = 0; c < m_colls; c ++)
        {
            coordinates buff;
            buff.y = r;
            buff.x = c;
            if(movementMap[r][c] <= movement && !(r == position.y && c == position.x) && findSquadByCoor(buff) == nullptr)
            {
                returnVector.push_back(m_tiles[r][c]);
            }
        }
    }
    return returnVector;
}

vector<Tile*> Battle::showAvailableShootTiles(Squad* squad)
{
    // the vector that we return
    vector<Tile*> returnVector;

    vector<Tile*>* thisStep;
    vector<Tile*>* nextStep;

    bool foundResult = false;
    unsigned short step = 0;

    Tile* buff = nullptr;
    thisStep = new vector<Tile*>;
    thisStep->push_back(squad->m_tileTaken);
    while(step < squad->m_attackRange)
    {
        nextStep = new vector<Tile*>;
        for(unsigned short i = 0; i < thisStep->size(); i ++)
        {
            for(unsigned short j = 0; j < 6; j ++)
            {
                buff = giveNeighbor(((*thisStep)[i]->m_mapCoordinates), j);
                if(buff != nullptr)
                {
                    for (unsigned short k = 0; k < returnVector.size(); k ++)
                    {
                        if(buff == returnVector[k])
                        {
                            buff = nullptr;
                            break;
                        }
                    }
                    if(buff != nullptr)
                    {
                        returnVector.push_back(buff);
                        nextStep->push_back(buff);
                    }
                }
            }
        }
        thisStep = nextStep;
        step++;
    }
    return returnVector;
}

void Battle::initSquad(SQUAD type, coordinates mapCoor, unsigned short numberOfUnits, OWNER owner)
{
    Squad* squad = nullptr;
    D(m_tiles.size());
    Tile* tile = m_tiles[mapCoor.y][mapCoor.x];

    switch(type)
    {
    case WARRIOR:
        squad = new Squad(*(world.m_configManager.modelSquadWarrior), (&world.m_cameraOffset), tile, owner, *world.m_configManager.modelHealthManager);
        break;
    case ARCHER:
        squad = new Squad(*(world.m_configManager.modelSquadArcher), (&world.m_cameraOffset), tile, owner, *world.m_configManager.modelHealthManager);
        break;
    case SPEARMEN:
        squad = new SpearSquad(*(world.m_configManager.modelSquadSpearmen), (&world.m_cameraOffset), tile, owner, *world.m_configManager.modelHealthManager);
        break;
    case CROSSBOWMEN:
        squad = new HookSquad(*(world.m_configManager.modelSquadCrossbowmen), (&world.m_cameraOffset), tile, owner, *world.m_configManager.modelHealthManager);
        break;
    default:
        squad = nullptr;
        break;
    }

    if(squad != nullptr)
    {
        squad->m_numberOfUnits = numberOfUnits;
        squad->m_hm.updateUnitsNumber();
        m_squads.push_back(squad);
        m_unwalkableTiles[mapCoor.y][mapCoor.x] = true;
    }
    else
    {
        cout << "Tried to create a squad from an unknown type: " << to_string((int)type) << endl;
    }
}

void Battle::switchTurn()
{
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        if((*it)->m_owner != m_playerTurn)
        {
            (*it)->m_moved = false;
            (*it)->m_attacked = false;
            (*it)->m_speed = (*it)->m_startSpeed;
        }
        else
        {
            (*it)->m_attacked = true;
            (*it)->m_moved = true;
        }
    }

    if (m_playerTurn == PLAYER1)
    {
        cout << "AI MAKING TURN" << endl;
        m_playerTurn = PLAYER2;
        m_enemyAI.takeBattlefield();
        m_enemyAI.makeTurn();
        m_enemyAI.returnBattlefield();
        switchTurn();
    }
    else if(m_playerTurn == PLAYER2)
    {
        m_playerTurn = PLAYER1;
    }
}

void Battle::takeDamage(Squad* attacker, Squad* defender)
{
    defender->m_hm.takeDamage(attacker->m_attackDamage);
}

void Battle::squadActionsCheck()
{
    if(world.m_mouseIsPressed)
    {
        if (m_selectedSquad != nullptr)
        {
            /// Check if this is the squad that we have selected before
            bool seletedASquad = false;
            Squad* oldSquad = m_selectedSquad;
            for(int i = 0; i < m_squads.size(); i++)
            {
                if (m_squads[i]->m_mapCoor == m_selected)
                {
                    m_selectedSquad = m_squads[i];
                    seletedASquad = true;
                }
            }
            if(!seletedASquad)
            {
                /// Is it selected by the player in turn
                if (m_selectedSquad->m_owner == m_playerTurn && m_selectedSquad->m_path.empty())
                {
                    /// Have we moved the squad
                    if (m_selectedSquad->m_moved == false)
                    {
                        /// Have we clicked on an empty tile or on a squad
                        if (m_selectedSquad == oldSquad)
                        {
                            /// Can we travel to the selected tile
                            canTravel(m_selectedSquad, m_tiles[m_selected.y][m_selected.x]->m_mapCoordinates);
                            m_availableWalkTiles.clear();
                            m_availableShootTiles.clear();
                            m_selectedSquad = nullptr;
                        }
                    }
                    else if(m_selectedSquad->m_attacked == false)
                    {
                        /// If we haven't shot, than we can shoot
                        canShoot(m_selectedSquad, m_selected);
                        m_availableShootTiles.clear();
                        m_selectedSquad = nullptr;
                    }
                }
            }
            else
            {
                /// We have selected a Squad
                if(oldSquad != m_selectedSquad && !(oldSquad->m_attacked) && canShoot(oldSquad, m_selectedSquad->m_tileTaken->m_mapCoordinates) && m_selectedSquad->m_owner != oldSquad ->m_owner)
                {
                    /// We can shoot it
                    oldSquad->attack(m_selectedSquad);
                    world.m_startShake = time(nullptr);
                    oldSquad->m_attacked = true;
                    oldSquad->m_moved = true;
                    m_availableShootTiles.clear();
                    m_availableWalkTiles.clear();
                    m_selectedSquad = nullptr;
                }else if(m_selectedSquad->m_owner == m_playerTurn && m_selectedSquad != oldSquad)
                {
                    /// We have selected another of out own units, cancel the show Walk/Shooting tiles of the old one
                    m_availableShootTiles.clear();
                    m_availableWalkTiles.clear();
                    m_selectedSquad = nullptr;
                }
            }
        }
        else
        {
            bool seletedASquad = false;
            for(int i = 0; i < m_squads.size(); i++)
            {
                if (m_squads[i]->m_mapCoor == m_selected && m_squads[i]->m_owner == m_playerTurn)
                {
                    m_selectedSquad = m_squads[i];
                    seletedASquad = true;
                }
            }
            // TODO (konstantin #1#): Fix bug with clicking a tile before the squad arrives also with shooting
            /// We have selected the same squad
            if(seletedASquad && m_selectedSquad->m_owner == m_playerTurn)
            {
                if(m_selectedSquad->m_path.empty())
                {
                    if(!(m_selectedSquad->m_moved))
                    {
                        m_showFillBtn = true;
                        m_showAttackTiles = false;
                        m_availableWalkTiles = showAvailableWalkTiles(m_selectedSquad);
                    }
                    else if(!(m_selectedSquad->m_attacked))
                    {
                        m_showFillBtn = true;
                        m_showAttackTiles = true;
                        m_availableShootTiles = showAvailableShootTiles(m_selectedSquad);
                    }
                }
            }
            else if (!seletedASquad)
            {
                m_selectedSquad = nullptr;
            }
        }
    }
    // Hide the skip_turn_btn if there is a squad behind it
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {

        (*it) -> update();

        if (checkForCollisionBetweenRects(m_skipTurnFillBtn.objRect, (*it)->m_objectRect))
        {
            m_showFillBtn = false;
        }
        if(m_selectedSquad != nullptr)
        {
            /*
            if(canShoot(m_selectedSquad, (*it)->m_mapCoor) && m_selectedSquad != (*it) && m_availableWalkTiles.size() != 0)
            {
                m_availableShootTiles.push_back(m_tiles[(*it)->m_mapCoor.y][(*it)->m_mapCoor.x]);
            }
            */
        }
    }
}

void Battle::checkForTurnSwitch()
{
    bool checkForSwitch = true;
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        if((*it)->m_owner == m_playerTurn)
        {
            if(!((*it)->m_moved) || !((*it)->m_attacked))
            {
                checkForSwitch = false;
            }
        }
    }
    if (checkForSwitch)
    {
        switchTurn();
    }
}

/*
vector<Tile*> returnVector;

    vector<Tile*>* thisStep;
    vector<Tile*>* nextStep;

    bool foundResult = false;
    unsigned short step = 0;

    Tile* buff = nullptr;
    thisStep = new vector<Tile*>;
    thisStep->push_back(squad->m_tileTaken);

    cout << "START OF CAN SHOOT \n";
    while(step < squad->m_attackRange)
    {
        nextStep = new vector<Tile*>;
        for(unsigned short i = 0; i < thisStep->size() && !foundResult; i ++)
        {
            for(unsigned short j = 0; j < 6 && !foundResult; j ++)
            {
                buff = giveNeighbor(((*thisStep)[i]->m_mapCoordinates), j);
                if(buff != nullptr)
                {
                    for (unsigned short k = 0; k < returnVector.size(); k ++)
                    {
                        if(buff == returnVector[k])
                        {
                            buff = nullptr;
                            break;
                        }
                    }
                    if(buff != nullptr)
                    {
                        if(buff->m_mapCoordinates.x == targetPosition.x && buff->m_mapCoordinates.y == targetPosition.y)
                        {
                            foundResult = true;
                        }
                        cout << "HERE\n";
                        returnVector.push_back(buff);
                        cout << __LINE__ << endl;
                        nextStep->push_back(buff);
                        cout << "Here\n";
                    }
                }
            }
        }
        thisStep = nextStep;
        step++;
    }

*/
