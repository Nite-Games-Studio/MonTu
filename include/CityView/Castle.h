#ifndef CASTLE_H
#define CASTLE_H

#include <Building.h>

class Castle : public Building
{
    public:
        Castle();
        Castle(const Castle& model);
        virtual ~Castle();

        void load(ifstream& stream);
        void load(string configFile, SDL_Renderer* renderer);
        void setMapCoordinates(int a, int b);

        virtual void update();
        virtual void draw(SDL_Renderer* renderer);

    protected:

    private:
};

#endif // CASTLE_H
