#include  <SDL.h>

#include "World.h"
#include "FPSManager.h"
#include "WorldBuilding.h"
#include "TitleScreen.h"

#undef main

World world;
FPSManager frameManager;

WorldBuilding cityView;

TitleScreen screen;

int main(int argc, char* argv[])
{
    world.initSDL("world.txt");

    world.m_playerStatsManager.readAllValues();
    world.m_playerStatsManager.changeValues(MONEY, 1111);
    world.m_playerStatsManager.writeAllValues();

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_PollEvent(&(world.m_event));

    /*
    NOSCENE,
    MENU,
    PICK_BAN,
    GAME,
    STATS,
    MAP_CHOOSING,
    WORLD_MAP,
    CITYBUILDING,
    TITLESCREEN,
    EXIT*/

    world.m_gameState = MENU;

    screen.load("TitleScreens\\NiTe_logo.txt", world.m_main_renderer);

    while(true)
    {
        if(world.m_gameState == TITLESCREEN)
        {
            while(!world.m_quitScene)
            {
                world.input();
                screen.update();
                screen.draw(world.m_main_renderer);
            }
            world.m_quitScene = false;
        }

        if(world.m_gameState == MENU)
        {
            while(!world.m_quitScene)
            {                
                world.input();
                world.m_menu.update();
                world.m_menu.draw();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == MAP_CHOOSING)
        {
            while(!world.m_quitScene)
            {
                world.input();
                world.m_menu.Choose_Map();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == PICK_BAN)
        {
            while(!world.m_quitScene)
            {
                world.input();
                ///world.m_pickAndBan.update();
                ///world.m_pickAndBan.draw();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == GAME)
        {
            world.m_battle.initGameSession(3, 0, 1);
            world.m_battle.m_enemyAI.takeBattlefield();
            world.m_battle.m_enemyAI.returnBattlefield();


            while(!world.m_quitScene)
            {
                world.input();
                world.m_battle.update();
                world.m_battle.draw();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == WORLD_MAP)
        {
            world.m_worldMap.init("worldMap.txt");
            world.m_worldMap.loadMap("worldMapPieces.txt");
            world.m_worldMap.loadCity("all_cities.txt");

            while(!world.m_quitScene)
            {
                world.input();
                world.m_worldMap.update();
                world.m_worldMap.draw();
            }
            world.m_quitScene = false;
        }

        if(world.m_gameState == CITYBUILDING)
        {
            cityView.initCity("cityView.txt");
            while(!world.m_quitScene)
            {
                world.input();
                cityView.updateBuilding();
                cityView.drawBuilding();
                //cityView.saveBuildings("buildingsSave.txt");
            }

            world.m_quitScene = false;
        }

        if(world.m_gameState == EXIT)
        {
            world.destroySDL();
        }

        while(frameManager.elapsedTime() < MIN_FRAMETIME) SDL_Delay(1);
        frameManager.start();
    }

    return EXIT_SUCCESS;
}
