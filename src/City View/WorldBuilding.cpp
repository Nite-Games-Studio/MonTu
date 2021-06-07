#include "WorldBuilding.h"
#include "World.h"
extern World world;

WorldBuilding::WorldBuilding()
{
    m_selected.x = 0;
    m_selected.y = 0;
    m_uiBoard = NULL;
    m_entityCount[(int)COUNT] = {0};
    type = NOBUILD;
}

WorldBuilding::~WorldBuilding()
{
    //dtor
}

void WorldBuilding::initCity(string configFile)
{

    configFile = "config\\" + configFile;

    fstream stream;
    stream.open(configFile.c_str());
    string tmp;
    string selectTileTex;

    stream >> tmp >> m_colls >> m_rows;
    stream >> tmp >> selectTileTex;

    stream >> tmp >> m_CP1.r >> m_CP1.g >> m_CP1.b;
    stream >> tmp >> m_CP2.r >> m_CP2.g >> m_CP2.b;

    stream.close();

    m_selectedTileUI.objTexture = LoadTexture(selectTileTex,world.m_main_renderer);
    m_renderer = world.m_main_renderer;

    initBackground("cityBackground.txt");

    initMap("CityView.txt");
    initTiles("cityMap.txt");

    if(castle == NULL){

        castle = new Castle(*(world.m_configManager.modelCastle));

        castle->m_mapCoordinates.x = castleMap_x;
        castle->m_mapCoordinates.y = castleMap_y;

        castle->m_objectRect.x = castle_x - m_hexagonWidth*0.5;
        castle->m_objectRect.y = castle_y - m_hexagonHeight*0.55;
        m_buildings.push_back(castle);


    }

    loadBuildings("buildingsSave.txt");

}

void WorldBuilding::initMap(string configFile)
{
    configFile = "config\\Maps\\" + configFile;


    fstream in_file;

    in_file.open(configFile.c_str());

    for(int r = 0; r < m_rows; r++)
    {
        for(int c = 0; c < m_colls; c++)
        {
            in_file >> field[c][r];
        }
    }

    in_file.close();
}

void WorldBuilding::initBackground(string configFile)
{

    configFile = "config\\" + configFile;

    fstream stream;

    string tmp;

    stream.open(configFile.c_str());
    stream >> tmp >> backgroundImg;
    stream.close();

    backgroundTexture = LoadTexture(backgroundImg, world.m_main_renderer);
}

void WorldBuilding::saveBuildings(string configFile)
{

    configFile = "data\\" + configFile;

    ofstream out_file;

    out_file.clear();
    out_file.open(configFile.c_str());

    out_file << "Buildings:";

    if(m_buildings.size() != 0){
        for(short int i = 0 ; i < m_buildings.size(); i++)
        {

            m_buildings[i]->save(m_buildings[i] , i );
            out_file << "\n" << m_buildings[i]->m_tempConfigFile;
            //cout << endl << endl;

        }
    }

    out_file.close();

}

void WorldBuilding::loadBuildings(string configFile)
{

    //cout << "#==========#" << endl;
    configFile = "data\\" + configFile;
    Building* building = NULL;

    ifstream in_file;

    in_file.open(configFile.c_str());

    string tmp,tmp2;
    in_file >> tmp;

    while(!in_file.eof())
    {


        in_file >> tmp2 ;
        tmp2 = "data\\city\\" + tmp2;

        if(tmp2 != "data\\city\\")
        {

            if(tmp2 == " "){

                //cout << "YELLO" << endl;
                break;

            }
            if(tmp2 != "data\\city\\0-castle.txt"){

                //cout << tmp2 << endl;
                building = new Building();
                building->load(tmp2 , world.m_main_renderer);

                m_buildings.push_back(building);

            }


        }

        /*cout << "#==========#" << endl;
        cout << m_buildings.size() << endl;
        cout << "#==========#" << endl << endl << endl;*/

    }




    in_file.close();
}

void WorldBuilding::initTiles(string configFile)
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
    Tile* castle = NULL;
    SDL_Point* buffPoint = NULL;

    for (short int r = 0; r < m_rows; r ++)
    {
        m_tiles.push_back(vector<Tile*>());
        for (short int c = 0; c < m_colls; c ++)
        {
            switch(field[c][r])
            {
            case 'G':
                tile = new Tile(*(world.m_configManager.modelTileGrass), &(world.m_cameraOffset));
                break;
            case 'S':
                tile = new Tile(*(world.m_configManager.modelTileGrass), &(world.m_cameraOffset));
                break;
            case 'C':
                castle = new Tile(*(world.m_configManager.modelTileGrass), &(world.m_cameraOffset));
                tile = new Tile(*(world.m_configManager.modelTileGrass), &(world.m_cameraOffset));
                break;
            case 'A':
                tile = new Tile(*(world.m_configManager.modelArchery), &(world.m_cameraOffset));
                break;
            case '_':
                tile = new Tile(*(world.m_configManager.modelTileForest), &(world.m_cameraOffset));
                break;
            case 'D':
                tile = new Tile(*(world.m_configManager.modelTileDesert), &(world.m_cameraOffset));
                break;
            case 'R':
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
            tile->m_objectRect.x = tile->m_drawCoordinates.x;
            tile->m_objectRect.y = tile->m_drawCoordinates.y;


            if(castle !=  NULL)
            {

                castle_x = tile->m_drawCoordinates.x;
                castle_y = tile->m_drawCoordinates.y;
                castleMap_x = tile->m_mapCoordinates.x;
                castleMap_y = tile->m_mapCoordinates.y;
                delete castle;
                castle = NULL;
            }


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

void WorldBuilding::initButtons()
{

    m_button = new Buttons(*(world.m_configManager.modelButtons));
    m_buttons.push_back(m_button);
}

void WorldBuilding::selectTile()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_PollEvent(&(world.m_event));

    for(short int r = 0; r < m_tiles.size(); r++)
    {
        for(short int c = 0; c < m_tiles[r].size(); c++)
        {
            if(isInsideAHexagon(m_tiles[r][c]->m_collisionPoints, LoadPoint(world.m_mouse)))
            {

                if(world.m_mouse.y <= 1080)
                {

                    m_selected.x = c;
                    m_selected.y = r;

                }
            }
        }
    }

    showUI();
    building();
}

void WorldBuilding::building()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_PollEvent(&(world.m_event));
    if(/*state[SDL_SCANCODE_B] && world.m_buttonDown*/world.m_mouseIsPressed)
    {
        if(m_uiBoard == NULL)
        {
            m_uiBoard = new UI(*(world.m_configManager.modelUI));
            m_userInterfaces.push_back(m_uiBoard);
            for(int i=0; i<11; i++)
            {
                m_selectable = new UISelectable(*(world.m_configManager.modelUISelectable),i);
                m_selectables.push_back(m_selectable);
                switch(i)
                {
                case 0:
                    m_selectables[i]->assignSelectable(world.m_main_renderer,STREET);
                    break;
                case 1:
                    m_selectables[i]->assignSelectable(world.m_main_renderer,ARCHERY);
                    break;
                case 2:
                    m_selectables[i]->assignSelectable(world.m_main_renderer,SHOP);
                    break;

                }
            }
        }
        else
        {
            if(type!= NOBUILDING)
            {
                if(alreadyBuilt(m_selected.x,m_selected.y) == false)
                {
                    /*while(true)
                    {
                        if(state[SDL_SCANCODE_TAB] & world.m_buttonDown)
                        {
                            cout << "HERE" << endl;
                            switch(type){

                                case STREET:
                                    type = SQUARE;
                                case SQUARE:
                                    type = SHOP;
                                    break;
                                case SHOP:
                                    type = ARCHERY;
                                    break;
                                case ARCHERY:
                                    type = STREET;
                                    break;

                            }

                        }
                    }*/
                    build(m_selected.x,m_selected.y,type);
                }

            }
        }
    }
}

void WorldBuilding::showUI()
{
    if(m_uiBoard != NULL)
    {
        if(checkForMouseCollision(world.m_mouse.x,world.m_mouse.y,m_uiBoard->m_objectRect))
        {
            for(int i=0; i<11; i++)
            {
                if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_selectables[i] -> button.objectRect))
                {
                    if(m_selectables[i] -> button.objectRect.w <= m_selectables[i] -> button.maxRect.w)
                    {
                        m_selectables[i] -> button.currentBonusW += m_selectables[i] -> button.bonusW;
                        m_selectables[i] -> button.currentBonusH += m_selectables[i] -> button.bonusH;

                        m_selectables[i] -> button.objectRect.w = m_selectables[i] -> button.minRect.w + m_selectables[i] -> button.currentBonusW;
                        m_selectables[i] -> button.objectRect.h = m_selectables[i] -> button.minRect.h + m_selectables[i] -> button.currentBonusH;
                        m_selectables[i] -> button.objectRect.x = m_selectables[i] -> button.minRect.x - m_selectables[i] -> button.currentBonusW / 2;
                        m_selectables[i] -> button.objectRect.y = m_selectables[i] -> button.minRect.y - m_selectables[i] -> button.currentBonusH / 2;

                    }
                    if(world.m_mouseIsPressed)
                    {
                        type = m_selectables[i]->m_type;
                    }
                }
                else
                {
                    m_selectables[i] -> button.currentBonusW = 0;
                    m_selectables[i] -> button.currentBonusH = 0;

                    m_selectables[i] -> button.objectRect = m_selectables[i] ->button.minRect;
                }
            }
        }
    }
}

void WorldBuilding::build(int c,int r,UI_ICON_TYPE type)
{
    Building* building = nullptr;
    switch(type)
    {
    case STREET:
        building = new Building(*(world.m_configManager.modelStreet));
        m_selectedTileUI.objTexture = LoadTexture("Street_selected.bmp",world.m_main_renderer);
        break;
    case SQUARE:
        building = new Building(*(world.m_configManager.modelStreet));
        m_selectedTileUI.objTexture = LoadTexture("Street_selected.bmp",world.m_main_renderer);
        break;
    case SHOP:
        building = new Building(*(world.m_configManager.modelShop));
        m_selectedTileUI.objTexture = LoadTexture("Shop_selected.bmp",world.m_main_renderer);
        break;
    case ARCHERY:
        building = new Building(*(world.m_configManager.modelArchery));
        m_selectedTileUI.objTexture = LoadTexture("Archery_selected.bmp",world.m_main_renderer);
        break;

    }

    if(building != nullptr)
    {
        building->m_mapCoordinates.x = c;
        building->m_mapCoordinates.y = r;
        building->m_drawCoordinates = m_tiles[r][c]->m_drawCoordinates;
        building->m_owner = PLAYER1;
        building->m_objectRect = m_tiles[r][c]->m_objectRect;
    }

    m_buildings.push_back(building);

    //SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
    //                     "Built",
    //                     "You've just constructed a building",
    //                     world.m_main_window);
}

void WorldBuilding::updateBuilding()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    selectTile();

    if(state[SDL_SCANCODE_ESCAPE] && world.m_buttonDown)
    {

        if(m_userInterfaces.size()!=0)
        {
            m_uiBoard=NULL;
            m_userInterfaces.clear();
            m_selectables.clear();
            //goto ;
        }else
        {

            saveBuildings("buildingsSave.txt");
            m_buildings.clear();
            world.m_quitScene = true;
            world.m_gameState = MENU;

        }
    }


    for(vector <Building*> :: iterator it = m_buildings.begin(); it != m_buildings.end(); it++)
    {
        (*it) -> update();
    }

    for(vector <UISelectable*> :: iterator it = m_selectables.begin(); it != m_selectables.end(); it++)
    {
        (*it) -> update();
    }
    for(vector <Buttons*> :: iterator it = m_buttons.begin(); it != m_buttons.end(); it++)
    {
        (*it) -> update();
    }

     m_selectedTileUI.objRect = m_tiles[m_selected.y][m_selected.x]->m_objectRect;



    /*m_selectedTileUI.objRect.w *= 3;
    m_selectedTileUI.objRect.h *= 3;
    m_selectedTileUI.objRect.x -= m_hexagonWidth;
    m_selectedTileUI.objRect.y -= m_hexagonHeight;*/


    //saveBuildings("buildingsSave.txt");

    //cleaner();

}

void WorldBuilding::drawBuilding()
{
    SDL_RenderClear(m_renderer);

    SDL_RenderCopy(world.m_main_renderer, backgroundTexture, NULL, NULL);

    for(vector<vector <Tile*> > :: iterator vit = m_tiles.begin(); vit != m_tiles.end(); vit++)
    {
        for(vector<Tile*> :: iterator it = (*vit).begin(); it != (*vit).end(); it++)
        {
            (*it) -> draw(world.m_main_renderer);
        }
    }
    for(vector <Building*> :: iterator it = m_buildings.begin(); it != m_buildings.end(); it++)
    {
        (*it) -> draw(world.m_main_renderer);
    }

    for(vector <UI*> :: iterator it = m_userInterfaces.begin(); it != m_userInterfaces.end(); it++)
    {
        (*it) -> draw(world.m_main_renderer);
    }

    for(vector <UISelectable*> :: iterator it = m_selectables.begin(); it != m_selectables.end(); it++)
    {
        (*it) -> draw(world.m_main_renderer);
    }
    for(vector <Buttons*> :: iterator it = m_buttons.begin(); it != m_buttons.end(); it++)
    {
        (*it) -> draw(world.m_main_renderer);
    }

    SDL_RenderCopy(world.m_main_renderer, m_selectedTileUI.objTexture, NULL, &(m_selectedTileUI.objRect));


    SDL_RenderPresent(world.m_main_renderer);
}

void WorldBuilding::cleaner()
{

    for(vector<Building*>::iterator it = m_buildings.begin(); it != m_buildings.end(); it++)
    {
        if((*it) -> m_health <= 0)
        {
            it = m_buildings.erase(it);
            it--;
        }

    }
}

bool WorldBuilding::alreadyBuilt(int x, int y)
{

    for(vector<Building*>::iterator it = m_buildings.begin(); it != m_buildings.end(); it++)
    {
        if((*it) -> m_mapCoordinates.x == x && (*it) -> m_mapCoordinates.y == y)
        {
            return true;
        }
    }
    return false;
}

/*
void WorldBuilding::initBuildings(string configFile)
{
    int cfgSize;
    configFile = "config\\" + configFile;

    fstream stream;
    coordinates coord;
    int inType;
    int inOwner;
    BUILDING type;
    OWNER owner;

    stream.open(configFile.c_str());
    stream >> cfgSize;
    while(cfgSize > 0)
    {
        stream >> coord.x >> coord.y >> inType >> inOwner;
        type = (BUILDING) inType;
        owner = (OWNER) inOwner;
        initBuilding(type, coord, owner);
        cfgSize--;
    }
}

void WorldBuilding::initBuilding(BUILDING type, coordinates mapCoor, OWNER owner)
{

    Tile* tile = m_tiles[mapCoor.y][mapCoor.x];
    if(alreadyBuilt(mapCoor.x,mapCoor.y)) {return;}
    Building* building = new Building();


    switch(type)
    {
        case CASTLE:
            building = new Castle(*(world.m_configManager.modelCastle), tile, owner);
            tile = giveNeighbor(mapCoor,0);
            building = new Castle(*(world.m_configManager.modelCastle), tile, owner);
            tile = giveNeighbor(mapCoor,1);
            building = new Castle(*(world.m_configManager.modelCastle), tile, owner);
            tile = giveNeighbor(mapCoor,2);
            building = new Castle(*(world.m_configManager.modelCastle), tile, owner);
            tile = giveNeighbor(mapCoor,3);
            building = new Castle(*(world.m_configManager.modelCastle), tile, owner);
            tile = giveNeighbor(mapCoor,4);
            building = new Castle(*(world.m_configManager.modelCastle), tile, owner);
            tile = giveNeighbor(mapCoor,5);
            building = new Castle(*(world.m_configManager.modelCastle), tile, owner);
            break;
        case ARMYCAMP:
            building = new ArmyCamp(*(world.m_configManager.modelArmyCamp), tile, owner);
            break;
        case BRIDGE:
            building = new Bridge(*(world.m_configManager.modelBridge), tile, owner);
            break;
        case STREET:
            building = new Building(*(world.m_configManager.modelStreet), tile, owner);
            break;
        case SHOP:
            building = new Building(*(world.m_configManager.modelShop), tile, owner);
            break;
        case ARCHERY:
            building = new Building(*(world.m_configManager.modelArchery), tile, owner);
            break;
        default:
           building = new Bridge(*(world.m_configManager.modelBridge), tile, owner);;
            break;
    }


    if(tile != NULL)
    {
        m_buildings.push_back(building);
    }
}
*/

Tile* WorldBuilding::giveNeighbor(coordinates coor, int direction)
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
