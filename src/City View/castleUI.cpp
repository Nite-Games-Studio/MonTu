#include "castleUI.h"
#include "World.h"
#include "squad_manager.h"

extern World world;

///INIT
//{
castleUI::castleUI()
{
    m_tab = 0;
}

castleUI::~castleUI()
{
    //dtor
}

void castleUI::init(string configFile, string cityName, SDL_Renderer* renderer)
{
    m_cityName = cityName;

    m_draggedSquad = -1;

    m_renderer = renderer;

    configFile = "config\\" + configFile;
    fstream stream;
    string tmp;
    string mainWindow[3];
    string slotTexture;
    string arrowTextureLocation;
    string createSquadWindowLocation;
    string createSquadBtn;

    USHORT margin;
    USHORT slotSize;
    USHORT bonusSlotSize;
    coordinates slotsStartCoor;

    stream.open(configFile);

    /// Main windows init
    stream >> tmp >> mainWindow[0] >> mainWindow[1] >> mainWindow[2];
    stream >> tmp >> m_mainWindow[0].objRect.x >> m_mainWindow[0].objRect.y >> m_mainWindow[0].objRect.w >> m_mainWindow[0].objRect.h;
    stream >> tmp >> m_tabButtons[0].x >> m_tabButtons[0].y >> m_tabButtons[0].w >> m_tabButtons[0].h;
    stream >> tmp >> m_tabButtons[1].x >> m_tabButtons[1].y >> m_tabButtons[1].w >> m_tabButtons[1].h;
    stream >> tmp >> m_tabButtons[2].x >> m_tabButtons[2].y >> m_tabButtons[2].w >> m_tabButtons[2].h;

    /// City Army
    stream >> tmp >> slotsStartCoor.x >> slotsStartCoor.y;
    stream >> tmp >> margin;
    stream >> tmp >> slotSize;
    stream >> tmp >> bonusSlotSize;
    stream >> tmp >> slotTexture;

    /// Create Squad
    stream >> tmp >> m_startOfCreateSquad.x >> m_startOfCreateSquad.y;
    stream >> tmp >> m_createSquadMargin;
    stream >> tmp >> arrowTextureLocation;
    stream >> tmp >> m_startOfArrows;
    stream >> tmp >> createSquadWindowLocation >> m_createSquadWindow.objRect.x >> m_createSquadWindow.objRect.y >> m_createSquadWindow.objRect.w >> m_createSquadWindow.objRect.h;
    stream >> tmp >> m_squadElWidth;
    stream >> tmp >> m_arrowSize;
    stream >> tmp >> m_startOfNewSquadWindow.x >> m_startOfNewSquadWindow.y;
    stream >> tmp >> m_newSquadMargin;
    stream >> tmp >> m_newSquadWidth;
    stream >> tmp >> m_newSquadArrowWidth;
    stream >> tmp >> createSquadBtn;
    stream >> tmp >> m_createSquadButton.startRect.x >> m_createSquadButton.startRect.y >> m_createSquadButton.startRect.w >> m_createSquadButton.startRect.h;

    stream.close();

    m_mainWindow[0].objTexture = LoadTexture(mainWindow[0], m_renderer);
    m_mainWindow[1].objTexture = LoadTexture(mainWindow[1], m_renderer);
    m_mainWindow[2].objTexture = LoadTexture(mainWindow[2], m_renderer);

    m_arrowTexture = LoadTexture(arrowTextureLocation, m_renderer);
    m_createSquadWindow.objTexture = LoadTexture(createSquadWindowLocation, m_renderer);

    /// Initializing the createSquadButton
    m_createSquadButton.objTexture = LoadTexture(createSquadBtn, m_renderer);
    m_createSquadButton.maxWidth = 32;
    m_createSquadButton.maxHeigth = 18;
    m_createSquadButton.bonusW = 6.4;
    m_createSquadButton.bonusH = 3.6;
    m_createSquadButton.objectRect = m_createSquadButton.startRect;

    /// Save the data needed for the hover animation of every slot, when hovered
    m_hoveredSlot.startRect.w = slotSize;
    m_hoveredSlot.startRect.h = slotSize;
    m_hoveredSlot.objectRect.w = slotSize;
    m_hoveredSlot.objectRect.h = slotSize;
    m_hoveredSlot.bonusW = 3;
    m_hoveredSlot.bonusH = 3;
    m_hoveredSlot.maxWidth = bonusSlotSize;
    m_hoveredSlot.maxHeigth = bonusSlotSize;
    m_hoveredSlot.objTexture = LoadTexture(slotTexture, m_renderer);

    for (USHORT r = 0; r < 3; r ++)
    {
        for (USHORT c = 0; c < 8; c ++)
        {
            m_colliders[r][c].y = slotsStartCoor.y + (slotSize + margin) * r;
            m_colliders[r][c].x = slotsStartCoor.x + (slotSize + margin) * c;
            m_colliders[r][c].w = slotSize;
            m_colliders[r][c].h = slotSize;
        }
    }
}

//}

///LOAD
//{
void castleUI::loadData(string configFile)
{
    fstream stream;

    configFile = "data//squads//soldier data//" + configFile;

    stream.open(configFile);

    USHORT type;

    string tmp;

    squadSlot* squadBuff;
    soldier* dataBuff;
    createSquadElement* squadElBuff;

    for (short i = 0; i < 5; i ++)
    {
        squadElBuff = new createSquadElement;

        squadElBuff->m_pic.objTexture = loadSquadTexture((SQUAD)(i + 1));

        squadElBuff->m_pic.objRect.x = m_startOfCreateSquad.x;
        squadElBuff->m_pic.objRect.w = m_squadElWidth - 5;
        squadElBuff->m_pic.objRect.h = m_squadElWidth - 5;

        squadElBuff->m_upBtnRect = {squadElBuff->m_pic.objRect.x + m_startOfArrows, squadElBuff->m_pic.objRect.y + (m_squadElWidth - 2 * m_arrowSize) / 3, m_arrowSize, m_arrowSize};
        squadElBuff->m_downBtnRect = {squadElBuff->m_pic.objRect.x + m_startOfArrows, squadElBuff->m_pic.objRect.y + m_arrowSize + (m_squadElWidth - 2 * m_arrowSize) / 3 * 2, m_arrowSize, m_arrowSize};
        squadElBuff->m_typeName = loadSquadName((SQUAD)(i + 1));

        m_createSquadEl[i] = squadElBuff;
    }

    for(short i = 0; i < 5; i++)
    {
        m_newSquadData[i] = new newSquadData;
        m_newSquadData[i]->m_numberOfSoldiers = 0;

        m_newSquadData[i]->m_upBtnRect.x = m_startOfNewSquadWindow.x + 180;
        m_newSquadData[i]->m_upBtnRect.w = m_newSquadArrowWidth;
        m_newSquadData[i]->m_upBtnRect.h = m_newSquadArrowWidth;
        m_newSquadData[i]->m_downBtnRect.x = m_startOfNewSquadWindow.x + 180;
        m_newSquadData[i]->m_downBtnRect.w = m_newSquadArrowWidth;
        m_newSquadData[i]->m_downBtnRect.h = m_newSquadArrowWidth;
    }

    while(stream >> type)
    {
        dataBuff = new soldier;
        squadBuff = new squadSlot;
        stream >> tmp >> dataBuff->numberOfSoldiers >> tmp >> dataBuff->coord.x >> dataBuff->coord.y;

        dataBuff->type = (SQUAD)type;
        squadBuff->data = dataBuff;
        squadBuff->bonusW = m_hoveredSlot.bonusW;
        squadBuff->bonusH = m_hoveredSlot.bonusH;
        squadBuff->maxWidth = m_hoveredSlot.maxWidth;
        squadBuff->maxHeigth = m_hoveredSlot.maxHeigth;
        squadBuff->startRect = m_colliders[squadBuff->data->coord.y][squadBuff->data->coord.x];
        squadBuff->objectRect = m_colliders[squadBuff->data->coord.y][squadBuff->data->coord.x];
        squadBuff->objTexture = loadSquadTexture(squadBuff->data->type);

        m_squads.push_back(squadBuff);
        m_data.push_back(dataBuff);
        m_createSquadEl[type - 1]->m_numberOfSoldiers += dataBuff->numberOfSoldiers;
    }

    stream.close();

    updateCreateSquadElements();
}

SDL_Texture* castleUI::loadSquadTexture(SQUAD type)
{
    SDL_Texture* returnData = nullptr;

    D(type);

    switch(type)
    {
    case WARRIOR:
        returnData = LoadTexture("warrior.bmp", m_renderer);
        break;
    case ARCHER:
        returnData = LoadTexture("archer.bmp", m_renderer);
        break;
    case SPEARMEN:
        returnData = LoadTexture("spearmen.bmp", m_renderer);
        break;
    case KNIGHTS:
        returnData = LoadTexture("knights.bmp", m_renderer);
        break;
    case CROSSBOWMEN:
        returnData = LoadTexture("crossbowmen.bmp", m_renderer);
        break;
    default:
        returnData = NULL;
        break;
    }

    return  returnData;
}

string castleUI::loadSquadName(SQUAD type)
{
    string returnData;

    switch(type)
    {
    case WARRIOR:
        returnData = "WARRIOR";
        break;
    case ARCHER:
        returnData = "ARCHER";
        break;
    case SPEARMEN:
        returnData = "SPEARMEN";
        break;
    case KNIGHTS:
        returnData = "KNIGHTS";
        break;
    case CROSSBOWMEN:
        returnData = "CROSSBOWMEN";
        break;
    default:
        returnData = "NONE";
        break;
    }

    return  returnData;
}

//}

void castleUI::hover()
{
    /// Check all slots with squads inside
    for (USHORT i = 0; i < m_squads.size(); i ++)
    {
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_squads[i]->objectRect))
        {
            if(m_squads[i]->objectRect.w < m_squads[i]->startRect.w + m_squads[i]->maxWidth)
            {
                m_squads[i]->currentBonusW += m_squads[i]->bonusW;
                m_squads[i]->currentBonusH += m_squads[i]->bonusH;

                m_squads[i]->objectRect.w = m_squads[i]->startRect.w + m_squads[i]->currentBonusW;
                m_squads[i]->objectRect.h = m_squads[i]->startRect.h + m_squads[i]->currentBonusH;
                m_squads[i]->objectRect.x = m_squads[i]->startRect.x - m_squads[i]->currentBonusW / 2;
                m_squads[i]->objectRect.y = m_squads[i]->startRect.y - m_squads[i]->currentBonusH / 2;
            }
            break;
        }
        else
        {
            m_squads[i]->objectRect = m_squads[i]->startRect;

            m_squads[i]->currentBonusW = 0;
            m_squads[i]->currentBonusH = 0;
        }
    }

    for(USHORT r = 0; r < 3; r ++)
    {
        for (USHORT c = 0; c < 8; c ++)
        {
            if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_colliders[r][c]))
            {
                if(m_hoveredSlot.objectRect.w < m_hoveredSlot.startRect.w + m_hoveredSlot.maxWidth)
                {
                    m_lastHoveredSlot.x = r;
                    m_lastHoveredSlot.y = c;

                    m_hoveredSlot.currentBonusW += m_hoveredSlot.bonusW;
                    m_hoveredSlot.currentBonusH += m_hoveredSlot.bonusH;

                    m_hoveredSlot.objectRect.w = m_hoveredSlot.startRect.w + m_hoveredSlot.currentBonusW;
                    m_hoveredSlot.objectRect.h = m_hoveredSlot.startRect.h + m_hoveredSlot.currentBonusH;
                    m_hoveredSlot.objectRect.x = m_colliders[r][c].x - m_hoveredSlot.currentBonusW / 2;
                    m_hoveredSlot.objectRect.y = m_colliders[r][c].y - m_hoveredSlot.currentBonusH / 2;
                }
                /// if your mouse is over a slot, stop the func
                return;
            }
        }
    }

    /// If we are here than the mouse is not on a tab, then we can make the hoveredSlot with currentBonus = 0
    m_hoveredSlot.objectRect = m_hoveredSlot.startRect;

    m_hoveredSlot.objectRect.w = 0;
    m_hoveredSlot.objectRect.h = 0;
    m_hoveredSlot.currentBonusW = 0;
    m_hoveredSlot.currentBonusH = 0;
}

void castleUI::moveSquad()
{
    if(world.m_mouseIsReleased && m_draggedSquad != -1)
    {
        m_squads[m_draggedSquad]->startRect = m_colliders[m_lastHoveredSlot.x][m_lastHoveredSlot.y];
        m_squads[m_draggedSquad]->objectRect = m_colliders[m_lastHoveredSlot.x][m_lastHoveredSlot.y];
        m_squads[m_draggedSquad]->data->coord.x = m_lastHoveredSlot.x;
        m_squads[m_draggedSquad]->data->coord.y = m_lastHoveredSlot.y;
    }
}

void castleUI::update()
{
    if(world.m_mouseIsPressed)
    {
        for(usi i = 0; i < 3; i++)
        {
            if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_tabButtons[i]))
            {
                m_tab = i;
                return;
            }
        }
    }

    switch(m_tab)
    {
    case 0:
        updateCitySquad();
        break;
    case 1:
        updateCreateSquad();
        break;
    }
}

void castleUI::draw()
{
    SDL_RenderClear(m_renderer);

    SDL_RenderCopy(m_renderer, m_mainWindow[m_tab].objTexture, NULL, &(m_mainWindow[0].objRect));

    switch(m_tab)
    {
    case 0:
        drawCitySquad();
        break;
    case 1:
        drawCreateSquad();
        break;
    }

    SDL_RenderPresent(m_renderer);
}

void castleUI::updateCitySquad()
{
    hover();

    moveSquad();

    saveData("squad1.txt");

    if(world.m_mouseIsPressed)
    {
        for (USHORT i = 0; i < m_squads.size(); i ++)
        {
            if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_squads[i]->objectRect))
            {
                m_draggedSlot.objTexture = m_squads[i]->objTexture;
                m_draggedSquad = i;
                break;
            }
        }
    }

    if(!world.m_drag)
    {
        m_draggedSlot.objRect.w = 0;
        m_draggedSlot.objRect.h = 0;
        m_draggedSlot.objTexture = nullptr;
        m_draggedSquad = -1;
    }
    else
    {
        m_draggedSlot.objRect.w = m_squads[0]->objectRect.w;
        m_draggedSlot.objRect.h = m_squads[0]->objectRect.h;
    }

    m_draggedSlot.objRect.x = world.m_mouse.x - m_draggedSlot.objRect.w / 2;
    m_draggedSlot.objRect.y = world.m_mouse.y - m_draggedSlot.objRect.h / 2;
}

void castleUI::drawCitySquad()
{
    SDL_RenderCopy(m_renderer, m_hoveredSlot.objTexture, NULL, &(m_hoveredSlot.objectRect));

    for(unsigned short i = 0; i < m_squads.size(); i ++)
    {
        if(!world.m_drag)
        {
            SDL_RenderCopy(m_renderer, m_squads[i]->objTexture, NULL, &(m_squads[i]->objectRect));
        }
        else
        {
            if(i != m_draggedSquad)
            {
                SDL_RenderCopy(m_renderer, m_squads[i]->objTexture, NULL, &(m_squads[i]->objectRect));
            }
        }
    }

    SDL_RenderCopy(m_renderer, m_draggedSlot.objTexture, NULL, &(m_draggedSlot.objRect));
}

void castleUI::saveData(string configFile)
{
    fstream stream;

    configFile = "data//" + configFile;

    stream.open(configFile);

    for(vector<soldier*> :: iterator it = m_data.begin(); it != m_data.end(); it ++)
    {
        stream << (int)((*it) -> type) << " NUMBER_OF_SOLDIERS: " << (*it) -> numberOfSoldiers << " COORDINATES: " << (*it) -> coord.y << " " << (*it) -> coord.x << endl;
    }

    stream.close();
}

void castleUI::removeUnits()
{
    for(int i = 0; i < 5; i ++)
    {
        while(m_newSquadData[i]->m_numberOfSoldiers != 0)
        {
            for(int j = 0; j < m_data.size(); j ++)
            {
                if(m_data[j]->type != i + 1)
                {
                    continue;
                }
                if(m_data[j]->numberOfSoldiers >= m_newSquadData[i]->m_numberOfSoldiers)
                {
                    D(m_data[j]->numberOfSoldiers);
                    world.m_squadManager.changeUnits(0, m_data[j]->type, m_data[j]->coord.x, m_data[j]->coord.y, m_data[j]->numberOfSoldiers, -(m_newSquadData[i]->m_numberOfSoldiers));
                    m_data[j]->numberOfSoldiers -= m_newSquadData[i]->m_numberOfSoldiers;
                    m_newSquadData[i]->m_numberOfSoldiers = 0;
                    continue;
                }
                if(m_data[j]->numberOfSoldiers != 0)
                {
                    world.m_squadManager.changeUnits(0, m_data[j]->type, m_data[j]->coord.x, m_data[j]->coord.y, m_data[j]->numberOfSoldiers, -(m_data[j]->numberOfSoldiers));
                    m_newSquadData[i]->m_numberOfSoldiers -= m_data[j]->numberOfSoldiers;
                    m_data[j]->numberOfSoldiers = 0;
                    break;
                }
            }
        }
    }
    ///world.m_squadManager.changeUnits(1, ARCHER, 2, 2, 35, -18);
}

void castleUI::createSquad()
{
    D(m_cityName)
    int squadIndex = world.m_squadManager.addSquad("TSAREVO");

    D(squadIndex);

    for (unsigned short i = 0; i < 5; i ++)
    {
        if(m_newSquadData[i]->m_numberOfSoldiers > 0)
        {
            world.m_squadManager.addSoldier(squadIndex, m_newSquadData[i]->m_numberOfSoldiers, (SQUAD)i, i % 8, i / 8);
        }
    }

    removeUnits();
}

void castleUI::updateCreateSquad()
{
    if (world.m_mouseIsPressed || world.m_mouseIsDoubleClicked)
    {
        /// Update the main ones
        for (unsigned short i = 0; i < 5; i ++)
        {
            if(m_createSquadEl[i]->m_numberOfSoldiers > 0)
            {
                if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_createSquadEl[i]->m_upBtnRect))
                {
                    m_createSquadEl[i]->m_numberOfSoldiers --;
                    m_newSquadData[i]->m_numberOfSoldiers ++;
                    updateCreateSquadElements();
                    break;
                }
                else if (checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_createSquadEl[i]->m_downBtnRect))
                {
                    m_createSquadEl[i]->m_numberOfSoldiers ++;
                    m_newSquadData[i]->m_numberOfSoldiers --;
                    updateCreateSquadElements();
                    break;
                }
            }
            if(m_newSquadData[i]->m_numberOfSoldiers > 0)
            {
                if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_newSquadData[i]->m_upBtnRect))
                {
                    m_createSquadEl[i]->m_numberOfSoldiers --;
                    m_newSquadData[i]->m_numberOfSoldiers ++;
                    updateCreateSquadElements();
                    break;
                }
                else if (checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_newSquadData[i]->m_downBtnRect))
                {
                    m_createSquadEl[i]->m_numberOfSoldiers ++;
                    m_newSquadData[i]->m_numberOfSoldiers --;
                    if(m_createSquadEl[i]->m_numberOfSoldiers <= 0)
                    {
                    }
                    updateCreateSquadElements();
                    break;
                }
            }
        }

        /// Update the ones in the side menu
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_createSquadButton.objectRect))
        {
            createSquad();
        }
    }
    createSquadHover();
}

void castleUI::updateCreateSquadElements()
{
    short drawedUnits = 0;

    for (int i = 0; i < 5; i ++)
    {
        if(m_createSquadEl[i]->m_numberOfSoldiers > 0)
        {
            m_createSquadEl[i]->m_pic.objRect.y = m_startOfCreateSquad.y + drawedUnits * (m_createSquadMargin + m_squadElWidth);

            m_createSquadEl[i]->m_upBtnRect.y = m_createSquadEl[i]->m_pic.objRect.y + (m_squadElWidth - 2 * m_arrowSize) / 3;
            m_createSquadEl[i]->m_downBtnRect.y = m_createSquadEl[i]->m_pic.objRect.y + m_arrowSize + (m_squadElWidth - 2 * m_arrowSize) / 3 * 2;

            drawedUnits ++;
        }
    }

    drawedUnits = 0;
    for (int i = 0; i < 5; i ++)
    {
        if (m_newSquadData[i]->m_numberOfSoldiers > 0)
        {
            m_newSquadData[i]->m_pic.x = m_startOfNewSquadWindow.x;
            m_newSquadData[i]->m_pic.y = m_startOfNewSquadWindow.y + drawedUnits * (m_newSquadMargin + m_newSquadWidth);

            m_newSquadData[i]->m_pic.w = m_newSquadWidth;
            m_newSquadData[i]->m_pic.h = m_newSquadWidth;

            m_newSquadData[i]->m_upBtnRect.y = m_newSquadData[i]->m_pic.y + (m_newSquadWidth - 2 * m_newSquadArrowWidth) / 3;
            m_newSquadData[i]->m_downBtnRect.y = m_newSquadData[i]->m_pic.y + m_newSquadArrowWidth + (m_newSquadWidth - 2 * m_newSquadArrowWidth) / 3 * 2;

            drawedUnits ++;
        }
        else
        {
            m_newSquadData[i]->m_pic.w = 0;
        }
    }
}

void castleUI::drawCreateSquad()
{
    for (unsigned short i = 0; i < 5; i ++)
    {
        if (m_createSquadEl[i]->m_numberOfSoldiers > 0)
        {
            SDL_RenderCopy(m_renderer, m_createSquadEl[i]->m_pic.objTexture, NULL, &(m_createSquadEl[i]->m_pic.objRect));
            SDL_RenderCopy(m_renderer, m_arrowTexture, NULL, &(m_createSquadEl[i]->m_upBtnRect));
            SDL_RenderCopyEx(m_renderer, m_arrowTexture, NULL, &(m_createSquadEl[i]->m_downBtnRect), 0, 0, SDL_FLIP_VERTICAL);

            coordinates buff;
            buff.x = m_createSquadEl[i]->m_pic.objRect.x + m_createSquadEl[i]->m_pic.objRect.w;
            buff.y = m_createSquadEl[i]->m_pic.objRect.y + 10;

            coordinates buff2;
            buff2.x = m_createSquadEl[i]->m_pic.objRect.x + m_createSquadEl[i]->m_pic.objRect.w;
            buff2.y = m_createSquadEl[i]->m_pic.objRect.y + 20 + 42;

            write(m_createSquadEl[i]->m_typeName, (buff), m_renderer, 42);
            write("UNITS: " + to_string(m_createSquadEl[i]->m_numberOfSoldiers), buff2, m_renderer, 36);
        }
    }

    SDL_RenderCopy(m_renderer, m_createSquadWindow.objTexture, NULL, &(m_createSquadWindow.objRect));

    /// draw the side window
    for (int i = 0; i < 5; i ++)
    {
        if (m_newSquadData[i]->m_numberOfSoldiers > 0)
        {
            SDL_RenderCopy(m_renderer, m_createSquadEl[i]->m_pic.objTexture, NULL, &(m_newSquadData[i]->m_pic));
            SDL_RenderCopy(m_renderer, m_arrowTexture, NULL, &(m_newSquadData[i]->m_upBtnRect));
            SDL_RenderCopyEx(m_renderer, m_arrowTexture, NULL, &(m_newSquadData[i]->m_downBtnRect), 0, 0, SDL_FLIP_VERTICAL);

            coordinates buff;
            buff.x = m_newSquadData[i]->m_pic.x + m_newSquadWidth + 30;
            buff.y = m_newSquadData[i]->m_pic.y + alignCenter(m_newSquadWidth, 36);

            write("X " + to_string(m_newSquadData[i]->m_numberOfSoldiers), buff, m_renderer, 36);
        }
    }

    SDL_RenderCopy(m_renderer, m_createSquadButton.objTexture, NULL, &(m_createSquadButton.objectRect));
}

void castleUI::createSquadHover()
{
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_createSquadButton.objectRect))
    {
        if(m_createSquadButton.objectRect.w < m_createSquadButton.startRect.w + m_createSquadButton.maxWidth)
        {
            m_createSquadButton.currentBonusW += m_createSquadButton.bonusW;
            m_createSquadButton.currentBonusH += m_createSquadButton.bonusH;

            m_createSquadButton.objectRect.w = m_createSquadButton.startRect.w + m_createSquadButton.currentBonusW;
            m_createSquadButton.objectRect.h = m_createSquadButton.startRect.h + m_createSquadButton.currentBonusH;
            m_createSquadButton.objectRect.x = m_createSquadButton.startRect.x - m_createSquadButton.currentBonusW / 2;
            m_createSquadButton.objectRect.y = m_createSquadButton.startRect.y - m_createSquadButton.currentBonusH / 2;
        }
    }
    else
    {
        m_createSquadButton.objectRect = m_createSquadButton.startRect;

        m_createSquadButton.currentBonusW = 0;
        m_createSquadButton.currentBonusH = 0;
    }
}
