#include "AiBotTrainer.h"

#include <fstream>

AiBotTrainer::AiBotTrainer()
{
    //ctor
}

AiBotTrainer::~AiBotTrainer()
{
    //dtor
}

void AiBotTrainer::readProgress(string configFile)
{
    configFile = "data//bot training//" + configFile;

    fstream stream;
    stream.open(configFile);
    stream >> m_Bdistance >> m_Bteamwork >> m_Baggression >> BvalueSquadMap[ARCHER] >> BvalueSquadMap[WARRIOR] >> BvalueSquadMap[SPEARMEN] >> BvalueSquadMap[CROSSBOWMEN] >> BvalueSquadMap[KNIGHTS] >> BvalueSquadMap[HOOK] >> BvalueSquadMap[RAINOFARROWS];
    stream.close();
}

void AiBotTrainer::mutate()
{

}

/**
1. Binary search с гранични стойности на параметрите.
*/
