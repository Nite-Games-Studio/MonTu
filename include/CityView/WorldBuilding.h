#ifndef WORLDBUILDING_H
#define WORLDBUILDING_H
#include "Building.h"
#include "BuildingUI.h"

#define _WIN32_WINNT 0x0500

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <SDL.h>
#include <winuser.h>

#include "ConfigManager.h"
#include "SoundManager.h"
#include "PopUpWriter.h"
#include "PickAndBan.h"
#include "EnemyAI.h"
#include "PlayerStatsManager.h"
#include "ArmyManager.h"
#include "Engine.h"

#include "Tile.h"
#include "Squad.h"
#include "Building.h"
#include "Ui.h"
#include "Buttons.h"
#include "castleUI.h"

class WorldBuilding
{
    public:
        WorldBuilding();
        virtual ~WorldBuilding();

        TTF_Font* m_font;

        SDL_Renderer* m_renderer;

        short int m_colls;
        short int m_rows;
        short int m_entityCount[(int) COUNT];
        int m_hexagonWidth;
        int m_hexagonHeight;


        //SDL_Texture* m_battleButtonTexture;
        UI_object m_selectedTileUI;

        bool m_showFillBtn;

        coordinates m_cameraOffset;
        short int m_cameraShakeDuration = 1;
        short int m_cameraShakeMagnitude = 2;
        short int castle_x = 0;
        short int castle_y = 0;
        short int castleMap_x = 0;
        short int castleMap_y = 0;
        time_t m_startShake;
        bool m_shake;
        bool m_isSelected;
        bool m_castleUIIsActive;
        bool m_buildingTypeChange;

        string backgroundImg;
        SDL_Texture* backgroundTexture;

        char field[19][10];
        char fieldCastle[13][8];

        // Those are the coordinates that we use for determining the neighbors of a tile
        coordinates directions[2][6];

        coordinates m_selected;
        Tile* m_selectedTile;

        UI* m_uiBoard;

        UISelectable* m_selectable;

        Buttons* m_button;

        UI_ICON_TYPE type;

        Castle* castle = NULL;

        castleUI* m_castleUI = NULL;

        vector<vector<Tile*> > m_tiles;

        vector<Building*> m_buildings;

        vector<UI*> m_userInterfaces;

        vector<UISelectable*> m_selectables;

        vector<Buttons*> m_buttons;


        color m_CP1;
        color m_CP2;

        void initCity(string configFile);
        void initMap(string configFile);
        void initTiles(string configFile);
        void initCastle(string configFile);
        void initButtons();
        void initBackground(string configFile);

        void building();
        void showUI();
        void enlargeSelectable();

        void saveBuildings(string configFile);
        void loadBuildings(string configFile);

        void selectTile();

        void updateBuilding();
        void drawBuilding();
        void build(int x,int y,UI_ICON_TYPE type);

        void cleaner();

        bool alreadyBuilt(int x, int y);

        Tile* giveNeighbor(coordinates coor, int direction);

};

#endif // WORLDBUILDING_H
