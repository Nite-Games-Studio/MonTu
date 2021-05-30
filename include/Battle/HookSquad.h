#ifndef HOOKSQUAD_H
#define HOOKSQUAD_H

#include "Squad.h"

class HookSquad : public Squad
{
    public:
        HookSquad();
        virtual ~HookSquad();

        HookSquad(const HookSquad& model, coordinates* cameraOffset, Tile* tile, OWNER owner);

        virtual void attack(Squad* defender);
    protected:

    private:
};

#endif // HOOKSQUAD_H
