#ifndef HEALER_H
#define HEALER_H

#include "Squad.h"

class Healer : public Squad
{
    public:
        Healer();
        Healer(const Healer& model, coordinates* cameraOffset, Tile* tile, OWNER owner);
        virtual ~Healer();

        int m_heal;

        void heal();
    protected:

    private:
};

#endif // HEALER_H
