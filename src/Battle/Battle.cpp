#include "Battle.h"
#include "World.h"

extern World world;

Battle::Battle()
{
    m_selected.x = 0;
    m_selected.y = 0;
    m_selectedSquad = NULL;
    m_playerTurn = PLAYER1;
    m_showFillBtn = true;
    m_showAttackTiles = false;
    m_rows = 26;
    m_colls = 16;
}

Battle::~Battle()
{
    //dtor
}

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


    m_armyManager.init("armyManager.txt");
    m_popUpWriter.init("PopUpWriter.txt", m_renderer);


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

void Battle::initGameSession()
{
    coordinates buff;
    buff.x = 3;
    buff.y = 5;
    initSquad(WARRIOR, buff, PLAYER2);
    m_armyManager.deployArmy(PLAYER1);

}

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

    m_popUpWriter.draw(m_tiles[m_selected.y][m_selected.x]->m_objectRect, m_popUpWriter.m_buildingListRect, m_popUpWriter.m_buildingListTexture);

    if (m_showFillBtn)
    {
        SDL_RenderCopy(m_renderer, m_skipTurnFillBtn.objTexture, NULL, &(m_skipTurnFillBtn.objRect));
    }

    //SDL_RenderPresent(m_renderer);

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

Tile* Battle::giveNeighbor(coordinates coor, int direction)
{
    /*
    the value of direction should be the following:
    *short version* - starts from right and goes backwards
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
    return m_tiles[coor.y + addedCoor.y][coor.x + addedCoor.x];
}

bool Battle::canTravel(Squad* squad, coordinates desiredPosition)
{
    int** movementMap = new int* [m_rows];

    for (int i = 0; i < m_rows; i++)
    {

        movementMap[i] = new int [m_colls];

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
                        if(giveNeighbor(buff, i) != NULL)
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
                if (giveNeighbor(buff, i) != NULL)
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
                valueFound = true;
            }
        }
        /// cout << "INFO: Moving is possible " << movementMap[buff.y][buff.x] << " " << movement << endl;
        squad->m_path = path;

        // Check if the squad is surrounded with impossible to walk-through tiles, but has speed
        if(movement != 0)
        {
            bool hasPossibleMove = false;
            for (int i = 0; i < 6; i++)
            {
                if (giveNeighbor(buff, i) != NULL)
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
    int range = squad->m_attackRange;

    // Converting the logical coordinates to real coordinates
    coordinates logicalPosition = squad->m_tileTaken->m_mapCoordinates;
    coordinates position;
    position.x = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.x
                    + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.w) / 2;
    position.y = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.y
                    + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.h) / 2;
    /// cout << position.x << " " << position.y << endl;

    coordinates logicalTargetPosition = targetPosition;
    targetPosition.x = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.x
                        + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.w) / 2;
    targetPosition.y = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.y
                        + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.h) / 2;
    /// cout << targetPosition.x << " " << targetPosition.y << endl;

    // Using the pythagorean theorem, we can check if we have big enough range
    short int a = abs(position.x - targetPosition.x);
    short int b = abs(position.y - targetPosition.y);
    short int c = sqrt(a*a + b*b);

    if(c > range)
    {
        // If we are out of range or if there isn't a squad on this coor than we stop the function
        return false;
    }
    if(findSquadByCoor(logicalTargetPosition) == NULL || findSquadByCoor(logicalTargetPosition)->m_owner == squad->m_owner)
    {
        return false;
    }

    // Using this bool to determine if the function is done
    bool finished = false;
    // The angle used to calculate how we will travel to the target position
    short int rotationAngle;
    // We find the rotation angle by using the fnct in the Engine
    coordinates direction;
    direction.x = targetPosition.x - position.x;
    direction.y = targetPosition.y - position.y;
    rotationAngle = returnAngleByCoordinates(direction);
    /// cout << rotationAngle << endl;
    vector<Tile*> passing;

    while(!finished)
    {
        position.x += (sin(rotationAngle * PI / 180) * 6);
        position.y -= (cos(rotationAngle * PI / 180) * 6);

        for(short int r = 0; r < m_tiles.size(); r++)
        {
            for(short int c = 0; c < m_tiles[r].size(); c++)
            {
                if(isInsideAHexagon(m_tiles[r][c]->m_collisionPoints, LoadPoint(position)))
                {
                    passing.push_back(m_tiles[r][c]);
                    /// cout << logicalTargetPosition.y << " " << logicalTargetPosition.x << endl;
                    if(c == logicalTargetPosition.x && r == logicalTargetPosition.y)
                    {
                        finished = true;
                    }
                }
            }
        }
    }
    return true;
}

vector<Tile*> Battle::showAvailableWalkTiles(Squad* squad)
{
    int** movementMap = new int* [m_rows];

    for (int i = 0; i < m_rows; i++)
    {

        movementMap[i] = new int[m_colls];

    }
    // take the position and the speed
    coordinates position = squad->m_mapCoor;
    int movement = squad->m_speed;
    // The vector, that we return
    vector<Tile*> returnVector;
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
                        if(giveNeighbor(buff, i) != NULL)
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
            if(movementMap[r][c] <= movement && !(r == position.y && c == position.x) && findSquadByCoor(buff) == NULL)
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
    // buff variable used to pass the coordinates, to the canShoot function
    coordinates buffCoor;
    for (short int r = 0; r < m_rows; r ++)
    {
        for (short int c = 0; c < m_colls; c ++)
        {
            buffCoor.x = c;
            buffCoor.y = r;

            int range = squad->m_attackRange;
            // Converting the logical coordinates to real coordinates
            coordinates logicalPosition = squad->m_mapCoor;
            coordinates position;
            position.x = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.x + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.w) / 2;
            position.y = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.y + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.h) / 2;

            coordinates logicalTargetPosition = buffCoor;
            coordinates targetPosition;
            targetPosition.x = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.x + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.w) / 2;
            targetPosition.y = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.y + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.h) / 2;

            // Using the pythagorean theorem, we can check if we have big enough range
            short int a = abs(position.x - targetPosition.x);
            short int b = abs(position.y - targetPosition.y);
            short int distance = sqrt(a*a + b*b);

            if (distance < range)
            {
                returnVector.push_back(m_tiles[r][c]);
            }
        }
    }
    return returnVector;
}

void Battle::initSquad(SQUAD type, coordinates mapCoor, OWNER owner)
{
    Squad* squad;
    Tile* tile = m_tiles[mapCoor.y][mapCoor.x];
    switch(type)
    {
        case WARRIOR:
            squad = new HookSquad(*(world.m_configManager.modelSquadWarrior), (&world.m_cameraOffset), tile, owner);
            break;
        case ARCHER:
            squad = new Squad(*(world.m_configManager.modelSquadArcher), (&world.m_cameraOffset), tile, owner);
            break;
        case SPEARMEN:
            squad = new Squad(*(world.m_configManager.modelSquadSpearmen), (&world.m_cameraOffset), tile, owner);
            break;
        case CROSSBOWMEN:
            squad = new Squad(*(world.m_configManager.modelSquadCrossbowmen), (&world.m_cameraOffset), tile, owner);
            break;
        case KNIGHTS:
            squad = new Squad(*(world.m_configManager.modelSquadKnights), (&world.m_cameraOffset), tile, owner);
            break;
        default:
            squad = NULL;
            break;
    }
    if(tile != NULL)
    {
        m_squads.push_back(squad);
    }
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
    return NULL;
}

void Battle::switchTurn()
{
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        if((*it)->m_owner != m_playerTurn)
        {
            (*it)->m_moved = false;
            (*it)->m_shooted = false;
            (*it)->m_speed = (*it)->m_startSpeed;
        }
        else
        {
            (*it)->m_shooted = true;
            (*it)->m_moved = true;
        }
    }

    if (m_playerTurn == PLAYER1)
    {
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
    defender->m_health -= attacker->m_attackDamage;
}

void Battle::squadActionsCheck()
{
    if(world.m_mouseIsPressed)
    {
        if (m_selectedSquad != NULL)
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
                if (m_selectedSquad->m_owner == m_playerTurn)
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
                            m_selectedSquad = NULL;
                        }
                    }
                    else if(m_selectedSquad->m_shooted == false)
                    {
                        /// If we haven't shot, than we can shoot
                        canShoot(m_selectedSquad, m_selected);
                        m_availableShootTiles.clear();
                        m_selectedSquad = NULL;
                    }
                }
            }
            else
            {
                /// We have selected a Squad
                if(oldSquad != m_selectedSquad && !(oldSquad->m_shooted) && canShoot(oldSquad, m_selectedSquad->m_mapCoor) && m_selectedSquad->m_owner != oldSquad ->m_owner)
                {
                    /// We can shoot it
                    oldSquad->attack(m_selectedSquad);
                    world.m_startShake = time(NULL);
                    oldSquad->m_shooted = true;
                    oldSquad->m_moved = true;
                    m_availableShootTiles.clear();
                    m_availableWalkTiles.clear();
                    m_selectedSquad = NULL;
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
            /// We have selected
            if(seletedASquad && m_selectedSquad->m_owner == m_playerTurn)
            {
                if(!(m_selectedSquad->m_moved))
                {
                    m_showFillBtn = false;
                    m_showAttackTiles = false;
                    m_availableWalkTiles = showAvailableWalkTiles(m_selectedSquad);
                }
                else if(!(m_selectedSquad->m_shooted))
                {
                    m_showFillBtn = false;
                    m_showAttackTiles = true;
                    m_availableShootTiles = showAvailableShootTiles(m_selectedSquad);
                }
            }
            else if (!seletedASquad)
            {
                m_selectedSquad = NULL;
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
        if(m_selectedSquad != NULL)
        {
            if(canShoot(m_selectedSquad, (*it)->m_mapCoor) && m_selectedSquad != (*it) && m_availableWalkTiles.size() != 0)
            {
                m_availableShootTiles.push_back(m_tiles[(*it)->m_mapCoor.y][(*it)->m_mapCoor.x]);
            }
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
            if(!((*it)->m_moved) || !((*it)->m_shooted))
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
