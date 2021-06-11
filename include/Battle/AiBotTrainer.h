#ifndef AIBOTTRAINER_H
#define AIBOTTRAINER_H

#include <map>

#include "defines.h"

using namespace std;

class AiBotTrainer
{
    public:
        AiBotTrainer();
        virtual ~AiBotTrainer();

        /// Earned from getting closer to enemies
        /// Used only before first attack
        unsigned short m_Bdistance;
        /// Close to allies
        unsigned short m_Bteamwork;
        /// Damage done
        unsigned short m_Baggression;

        unsigned short m_BgenCounter;

        map<SQUAD, double> BvalueSquadMap{
            {ARCHER, 4},
            {WARRIOR, 1},
            {SPEARMEN, 2},
            {CROSSBOWMEN, 5},
            {KNIGHTS, 7}
        };

        ///Decision Parameters

        /// Earned from getting closer to enemies
        /// Used only before first attack
        unsigned short m_distance;
        /// Close to allies
        unsigned short m_teamwork;
        /// Damage done
        unsigned short m_aggression;

        unsigned short m_genCounter;

        map<SQUAD, double> valueSquadMap{
            {ARCHER, 4},
            {WARRIOR, 1},
            {SPEARMEN, 2},
            {CROSSBOWMEN, 5},
            {KNIGHTS, 7}
        };

        void readProgress(string configFile);
        void mutate();
    protected:

    private:
};

#endif // AIBOTTRAINER_H
