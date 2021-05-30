#ifndef ARMYCAMP_H
#define ARMYCAMP_H

#include <Building.h>


class ArmyCamp : public Building
{
    public:
        ArmyCamp();
        ArmyCamp(const ArmyCamp& model);
        virtual ~ArmyCamp();

        short int m_capacity;
        short int m_usedCapacity;

        bool m_entity[(int) COUNT];
        steady_clock::time_point m_entityTimePoint[(int) COUNT];
        short int m_entityWaitDuration[(int) COUNT];
        short int m_entityCount[(int) COUNT];

        virtual void load(ifstream& stream);
        virtual void load(string configFile, SDL_Renderer* renderer);
        virtual void update();

        virtual void limitHealth();
        virtual void select();
        virtual void getState();

    protected:

    private:
        void createEntities();
};

#endif // ARMYCAMP_H
