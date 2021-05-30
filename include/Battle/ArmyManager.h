#ifndef ARMYMANAGER_H
#define ARMYMANAGER_H

#include <iostream>
#include <fstream>

#include "defines.h"

// TODO (kostakk#1#): finish army manager
class ArmyManager
{
    public:
        ArmyManager();
        virtual ~ArmyManager();

        string armyFormationPath;

        void init(string configFile);
        void deployArmy(OWNER owner);

    protected:

    private:
};

#endif // ARMYMANAGER_H
