#ifndef ARMYMANAGER_H
#define ARMYMANAGER_H

#include <iostream>
#include <fstream>

#include "defines.h"

// TODO (konstantin#5#): finish army manager
class ArmyManager
{
    public:
        ArmyManager();
        virtual ~ArmyManager();

        string armyFormationPath;

        void deployArmy(unsigned short index, OWNER owner);
    protected:

    private:
};

#endif // ARMYMANAGER_H
