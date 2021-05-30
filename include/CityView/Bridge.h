#ifndef BRIDGE_H
#define BRIDGE_H

#include <Building.h>


class Bridge : public Building
{
    public:
        Bridge();
        Bridge(const Bridge& model);
        virtual ~Bridge();

        const double MOVEMENT = 30.0;

        short int m_imgRows;
        short int m_imgCols;
        double m_rotationAngle;

        virtual void load(ifstream& stream);
        virtual void load(string configFile, SDL_Renderer* renderer);
        virtual void update();
        virtual void draw(SDL_Renderer* renderer);

        virtual void limitHealth();
        virtual void select();
        virtual void getState();

    protected:

    private:

};

#endif // BRIDGE_H
