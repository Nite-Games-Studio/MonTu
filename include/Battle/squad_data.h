#ifndef SQUAD_DATA_H
#define SQUAD_DATA_H
#include<vector>
#include<string>
using namespace std;

struct coordinates{
    int x;
    int y;
};

struct soldier{
    string type;
    int health;
    coordinates coord;
};

struct squad_data{
    ///0 - city
    ///1 - map
    int state;
    string city;
    coordinates coords;
    string soldierFile;
};

struct soldier_data{
    vector <soldier> soldiers;
};

#endif
