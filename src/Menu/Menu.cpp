#include "Menu.h"
#include "World.h"

extern World world;

Menu::Menu()
{
    m_menuTexture = nullptr;
}

Menu::~Menu()
{
    //dtor
}

void Menu::init(string configFile)
{
    renderer = world.m_main_renderer;

    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    string backgroundMap;
    string Map1Img;
    string Map2Img;
    string Map3Img;
    string Map4Img;
    string Map1PickImg;
    string Map2PickImg;
    string Map3PickImg;
    string Map4PickImg;
    string menuImg;
    string PlayButtonImg;
    string ExitButtonImg;
    string OptionsButtonImg;
    string BattleButtonImg;
    string BackButtonImg;

    stream.open(configFile.c_str());

    stream >> tmp >> menuImg;
    stream >> tmp >> BattleButtonImg;
    stream >> tmp >> battleButton.objectRect.x >> battleButton.objectRect.y >> battleButton.objectRect.w >> battleButton.objectRect.h;
    stream >> tmp >> playButton.objectRect.x >> playButton.objectRect.y >> playButton.objectRect.w >> playButton.objectRect.h;
    stream >> tmp >> backgroundMap;
    stream >> tmp >> exitButton.objectRect.x >> exitButton.objectRect.y >> exitButton.objectRect.w >> exitButton.objectRect.h;
    stream >> tmp >> PlayButtonImg;
    stream >> tmp >> ExitButtonImg;
    stream >> tmp >> OptionsButtonImg;
    stream >> tmp >> BackButtonImg;
    stream >> tmp >> backButton.objectRect.x >> backButton.objectRect.y >> backButton.objectRect.w >> backButton.objectRect.h;
    stream >> tmp >> optionsButton.objectRect.x >> optionsButton.objectRect.y >> optionsButton.objectRect.w >> optionsButton.objectRect.h;
    stream >> tmp >> Map1Img;
    stream >> tmp >> Map2Img;
    stream >> tmp >> Map3Img;
    stream >> tmp >> Map4Img;
    stream >> tmp >> Map1PickImg;
    stream >> tmp >> Map2PickImg;
    stream >> tmp >> Map3PickImg;
    stream >> tmp >> Map4PickImg;
    stream >> tmp >> m_Map1Button.x >> m_Map1Button.y >> m_Map1Button.w >> m_Map1Button.h;
    stream >> tmp >> m_Map2Button.x >> m_Map2Button.y >> m_Map2Button.w >> m_Map2Button.h;
    stream >> tmp >> m_Map3Button.x >> m_Map3Button.y >> m_Map3Button.w >> m_Map3Button.h;
    stream >> tmp >> m_Map4Button.x >> m_Map4Button.y >> m_Map4Button.w >> m_Map4Button.h;
    stream >> tmp >> widthChange >> heightChange;

    stream.close();

    m_menuTexture = LoadTexture(menuImg, renderer);
    m_Map1Texture = LoadTexture(Map1Img, renderer);
    m_Map2Texture = LoadTexture(Map2Img, renderer);
    m_Map3Texture = LoadTexture(Map3Img, renderer);
    m_Map4Texture = LoadTexture(Map4Img, renderer);
    m_backgroundMapTexture = LoadTexture(backgroundMap, renderer);
    m_Map1PickTexture = LoadTexture(Map1PickImg, renderer);
    m_Map2PickTexture = LoadTexture(Map2PickImg, renderer);
    m_Map3PickTexture = LoadTexture(Map3PickImg, renderer);
    m_Map4PickTexture = LoadTexture(Map4PickImg, renderer);
    playButton.objTexture = LoadTexture(PlayButtonImg, renderer);
    optionsButton.objTexture = LoadTexture(OptionsButtonImg, renderer);
    exitButton.objTexture = LoadTexture(ExitButtonImg, renderer);
    backButton.objTexture = LoadTexture(BackButtonImg, renderer);
    battleButton.objTexture = LoadTexture(BattleButtonImg, renderer);

    playButton.startRect = playButton.objectRect;
    optionsButton.startRect = optionsButton.objectRect;
    exitButton.startRect = exitButton.objectRect;
    backButton.startRect = backButton.objectRect;
    battleButton.startRect = battleButton.objectRect;

    playButton.bonusW = 1.6;
    playButton.bonusH = 0.9;

    optionsButton.bonusW = 1.6;
    optionsButton.bonusH = 0.9;

    exitButton.bonusW = 1;
    exitButton.bonusH = 1;

    backButton.bonusW = 0.8;
    backButton.bonusH = 1.1;

    battleButton.bonusW = 1.6;
    battleButton.bonusH = 0.9;
}

void Menu::menu()
{

}

void Menu::draw()
{
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, m_menuTexture, NULL, NULL);

    SDL_RenderCopy(renderer, playButton.objTexture, NULL, &(playButton.objectRect));

    SDL_RenderCopy(renderer, optionsButton.objTexture, NULL, &(optionsButton.objectRect));

    SDL_RenderCopy(renderer, exitButton.objTexture, NULL, &(exitButton.objectRect));

    SDL_RenderPresent(renderer);
}

void Menu::buttonHover(Button* button)
{
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, button -> objectRect))
    {
        if(button -> objectRect.w <= button -> startRect.w + widthChange)
        {
            button -> currentBonusW += button -> bonusW;
            button -> currentBonusH += button -> bonusH;

            button -> objectRect.w = button -> startRect.w + button -> currentBonusW;
            button -> objectRect.h = button -> startRect.h + button -> currentBonusH;
            button -> objectRect.x = button -> startRect.x - button -> currentBonusW / 2;
            button -> objectRect.y = button -> startRect.y - button -> currentBonusH / 2;
        }
    }
    else
    {
        button -> currentBonusW = 0;
        button -> currentBonusH = 0;

        button -> objectRect = button -> startRect;
    }
}

void Menu::update()
{
    if(world.m_mouseIsPressed)
    {
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, playButton.objectRect))
        {
            world.m_quitScene = true;
            world.m_gameState = WORLD_MAP;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, optionsButton.objectRect))
        {
            world.m_quitScene = true;
            world.m_gameState = CITYBUILDING;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, exitButton.objectRect))
        {
            world.m_quitScene = true;
            world.m_gameState = EXIT;
        }
    }

    buttonHover(&playButton);
    buttonHover(&optionsButton);
    buttonHover(&exitButton);
}

void Menu::Choose_Map()
{
    SDL_RenderCopy(renderer, m_backgroundMapTexture, NULL, NULL);
    SDL_RenderCopy(renderer, m_Map1Texture, NULL, &(m_Map1Button));
    SDL_RenderCopy(renderer, m_Map2Texture, NULL, &(m_Map2Button));
    SDL_RenderCopy(renderer, m_Map3Texture, NULL, &(m_Map3Button));
    SDL_RenderCopy(renderer, m_Map4Texture, NULL, &(m_Map4Button));

    if(world.m_mouseIsPressed)
    {
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.x, m_Map1Button))
        {
            cout << "HERE ";
            world.initMap("Map1.txt");
            world.m_battle.initTiles("tileMap.txt");
            world.m_quitScene = true;
            world.m_gameState = GAME;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.x, m_Map2Button))
        {
            world.initMap("Map2.txt");
            world.m_battle.initTiles("tileMap.txt");
            world.m_quitScene = true;
            world.m_gameState = GAME;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map3Button))
        {
            world.initMap("Map3.txt");
            world.m_battle.initTiles("tileMap.txt");
            world.m_quitScene = true;
            world.m_gameState = GAME;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map4Button))
        {
            world.initMap("Map4.txt");
            world.m_battle.initTiles("tileMap.txt");
            world.m_quitScene = true;
            world.m_gameState = GAME;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, backButton.objectRect))
        {
            world.m_quitScene = true;
            world.m_gameState = MENU;
        }
    }

    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map1Button))
    {
        SDL_RenderCopy(renderer, m_Map1PickTexture, NULL, &(m_Map1Button));
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map2Button))
    {
        SDL_RenderCopy(renderer, m_Map2PickTexture, NULL, &(m_Map2Button));
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map3Button))
    {
        SDL_RenderCopy(renderer, m_Map3PickTexture, NULL, &(m_Map3Button));
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map4Button))
    {
        SDL_RenderCopy(renderer, m_Map4PickTexture, NULL, &(m_Map4Button));
    }

    buttonHover(&backButton);

    SDL_RenderCopy(renderer, backButton.objTexture, NULL, &(backButton.objectRect));

    SDL_RenderPresent(renderer);
}
