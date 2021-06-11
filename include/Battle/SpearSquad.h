#ifndef SPEARSQUAD_H
#define SPEARSQUAD_H

#include "Squad.h"

class SpearSquad : public Squad
{
    public:
        SpearSquad();
        SpearSquad(const SpearSquad& model, coordinates* cameraOffset, Tile* tile, OWNER owner, HealthManager hm);

        virtual ~SpearSquad();

        virtual void attack(Squad* defender) override;

    protected:

    private:
};

#endif // SPEARSQUAD_H
