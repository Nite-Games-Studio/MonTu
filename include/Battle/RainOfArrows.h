#ifndef RAINOFARROWS_H
#define RAINOFARROWS_H

#include "Squad.h"

class RainOfArrows : public Squad
{
    public:
        RainOfArrows();
        RainOfArrows(RainOfArrows* model);
        virtual ~RainOfArrows();

        void attack(Squad* defender);

        protected:

    private:
};

#endif // RAINOFARROWS_H
