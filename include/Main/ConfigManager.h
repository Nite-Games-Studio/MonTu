#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <SDL.h>
#include <string>

#include "Tile.h"
#include "Building.h"
#include "HealthManager.h"

#include "SpearSquad.h"
#include "HookSquad.h"
#include "UI.h"
#include "UISelectable.h"
#include "Buttons.h"
#include "Castle.h"
#include "ArmyCamp.h"
#include "Bridge.h"

using namespace std;

class ConfigManager
{
public:
    ConfigManager();
    virtual ~ConfigManager();

    Tile* modelTileGrass = NULL;
    Tile* modelTileWater = NULL;
    Tile* modelTileMountain = NULL;
    Tile* modelTileForest = NULL;
    Tile* modelTileDesert = NULL;
    Tile* modelTileStone = NULL;
    Tile* modelTileVolcano = NULL;
    Tile* modelTileLava = NULL;

    HookSquad* modelSquadWarrior = NULL;
    Squad* modelSquadArcher = NULL;
    Squad* modelSquadSpearmen = NULL;
    Squad* modelSquadCrossbowmen = NULL;
    Squad* modelSquadKnights = NULL;

    Castle* modelCastle = NULL;
    Building* modelArchery = NULL;
    Building* modelShop = NULL;
    Building* modelStreet = NULL;

    ArmyCamp* modelArmyCamp = NULL;
    Bridge* modelBridge = NULL;

    UI* modelUI = NULL;
    UISelectable* modelUISelectable = NULL;
    UISelectable* modelUISelectableSelected = NULL;

    Buttons* modelButtons = NULL;

    void init(string configFile, SDL_Renderer* renderer, HealthManager* hm);

protected:

private:
};

#endif // CONFIGMANAGER_H
