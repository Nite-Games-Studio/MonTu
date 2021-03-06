#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <SDL.h>
#include <string>

#include "Tile.h"
#include "Building.h"

#include "SpearSquad.h"
#include "HookSquad.h"
#include "UI.h"
#include "UISelectable.h"
#include "Buttons.h"
#include "Castle.h"
#include "ArmyCamp.h"
#include "Bridge.h"
#include "Particle.h"

using namespace std;

class ConfigManager
{
public:
    ConfigManager();
    virtual ~ConfigManager();


    Tile* modelTileGrass = nullptr;
    Tile* modelTileWater = nullptr;
    Tile* modelTileMountain = nullptr;
    Tile* modelTileForest = nullptr;
    Tile* modelTileDesert = nullptr;
    Tile* modelTileStone = nullptr;
    Tile* modelTileVolcano = nullptr;
    Tile* modelTileLava = nullptr;


    Squad* modelSquadWarrior = nullptr;
    Squad* modelSquadArcher = nullptr;
    SpearSquad* modelSquadSpearmen = nullptr;
    HookSquad* modelSquadCrossbowmen = nullptr;
    Squad* modelSquadKnights = nullptr;


    Castle* modelCastle = NULL;
    Building* modelArchery = NULL;
    Building* modelShop = NULL;
    Building* modelStreet = NULL;
    Building* modelBarracks = NULL;
    Building* modelStable = NULL;

    ArmyCamp* modelArmyCamp = NULL;
    Bridge* modelBridge = NULL;

    UI* modelUI = NULL;
    UISelectable* modelUISelectable = NULL;
    UISelectable* modelUISelectableSelected = NULL;

    Buttons* modelButtons = nullptr;

    Particle* modelMeleeHit = nullptr;

    HealthManager* modelHealthManager = nullptr;

    void init(string configFile, SDL_Renderer* renderer);

protected:

private:
};

#endif // CONFIGMANAGER_H
