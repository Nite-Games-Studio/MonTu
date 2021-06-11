#ifndef ENEMYAI_H
#define ENEMYAI_H

#include <vector>

#include <Tile.h>
#include <Squad.h>

class EnemyAI
{
    public:
        EnemyAI();
        virtual ~EnemyAI();

        vector<vector<Tile*>> m_battlefield;
        vector<Squad*> m_aiSquads;
        vector<Squad*> m_playerSquads;

        Tile* startPosition;
        vector<Tile*> availableToWalkTiles;
        double score;
        double bestScore;
        Tile* bestPosition;
        Squad* bestVictim;

        ///Decision Parameters

        /// Earned from getting closer to enemies
        /// Used only before first attack
        unsigned short m_distance;
        /// Close to allies
        unsigned short m_teamwork;
        /// Damage done
        unsigned short m_aggression;

        int teamworkScore;
        int distanceScore;
        int aggressionScore;

        int bestTeamworkScore;
        int bestDistanceScore;

        map<SQUAD, double> valueSquadMap{
            {ARCHER, 4},
            {WARRIOR, 1},
            {SPEARMEN, 2},
            {CROSSBOWMEN, 5},
            {KNIGHTS, 7}
        };

        void init(string confgiFile);
        void takeBattlefield();
        void makeTurn();
        void returnBattlefield();
        void clearAI();

    protected:

    private:
        void chooseBestActionForUnit(Squad* squad, unsigned short unitIndex);

};


#endif // ENEMYAI_H
