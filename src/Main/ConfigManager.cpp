#include "ConfigManager.h"
#include <fstream>

ConfigManager::ConfigManager()
{
    //ctor
}

ConfigManager::~ConfigManager()
{
    //dtor
}

void ConfigManager::init(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;

    fstream stream;
    string tmp, buff;
    stream.open(configFile.c_str());

    stream >> tmp >> buff;
    modelTileGrass = new Tile;
    modelTileGrass->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileWater = new Tile;
    modelTileWater->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileMountain = new Tile;
    modelTileMountain->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileForest = new Tile;
    modelTileForest->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileDesert = new Tile;
    modelTileDesert->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileStone = new Tile;
    modelTileStone->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileVolcano = new Tile;
    modelTileVolcano->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileLava = new Tile;
    modelTileLava->load(buff, renderer);

    stream >> tmp >> buff;
    modelHealthManager = new HealthManager;
    modelHealthManager->init(buff, renderer);

    stream >> tmp >> buff;
    modelSquadWarrior = new HookSquad;
    modelSquadWarrior->load(buff, renderer);

    stream >> tmp >> buff;
    modelSquadArcher = new Squad;
    modelSquadArcher->load(buff, renderer);

    stream >> tmp >> buff;
    modelSquadSpearmen = new SpearSquad;
    modelSquadSpearmen->load(buff, renderer);

    stream >> tmp >> buff;
    modelSquadCrossbowmen = new HookSquad;
    modelSquadCrossbowmen->load(buff, renderer);

    stream >> tmp >> buff;
    modelSquadKnights = new Squad;
    modelSquadKnights->load(buff, renderer);

    stream >> tmp >> buff;
    modelArchery = new Building;
    modelArchery->load(buff, renderer);

    stream >> tmp >> buff;
    modelShop = new Building;
    modelShop->load(buff, renderer);

    stream >> tmp >> buff;
    modelStreet = new Building;
    modelStreet->load(buff, renderer);

    stream >> tmp >> buff;
    modelCastle = new Castle;
    modelCastle->load(buff, renderer);

    stream >> tmp >> buff;
    modelArmyCamp = new ArmyCamp;
    modelArmyCamp->load(buff, renderer);

    stream >> tmp >> buff;
    modelBridge = new Bridge;
    modelBridge->load(buff, renderer);

    stream >> tmp >> buff;
    modelUI = new UI;
    modelUI->load(buff, renderer);

    stream >> tmp >> buff;
    modelUISelectable = new UISelectable();
    modelUISelectable->load(buff,renderer);

    stream >> tmp >> buff;
    modelUISelectableSelected = new UISelectable();
    modelUISelectableSelected->load(buff,renderer);

    stream >> tmp >> buff;
    modelButtons = new Buttons();
    modelButtons->load(buff,renderer);

    stream >> tmp >> buff;
    modelMeleeHit = new Particle;
    modelMeleeHit->initAnimation("warriorAttack.txt");

    stream.close();

    cout << "****************\n";
    cout << "LOADED ALL MODELS IN CONFIG MANAGER\n";
    cout << "****************\n";
}
