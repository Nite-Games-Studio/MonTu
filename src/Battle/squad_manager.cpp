#include "squad_manager.h"
#include<fstream>
#include<iostream>

squad_manager::squad_manager()
{
    //ctor
}

squad_manager::~squad_manager()
{
    //dtor
}

void squad_manager::init(){
    ifstream stream;
    stream.open("data//squadCount.txt");
    stream >> squadCount;
    stream.close();
}
///SQUAD API
int squad_manager::addSquad(string city){
    string fileName = "squad" + to_string(squadCount) + ".txt";
    fstream stream;
    ofstream ostream;
    ofstream oostream;
    ofstream ooostream;

    stream.open("data//all_squads.txt", ios::app);
    stream.seekg(stream.tellg(), ios::end);
    stream << fileName << "\n";
    stream.close();

    ostream.open("data//squads//" + fileName);
    ostream << "state: 0\n";
    ostream << "city: " << city << "\n";
    ostream << "soldier_data_" << to_string(squadCount) << ".txt";
    ostream.close();

    ooostream.open("data//squads//soldier data//soldier_data_" + to_string(squadCount) + ".txt");
    ooostream.close();

    squadCount++;
    oostream.open("data//squadCount.txt");
    oostream << squadCount;
    oostream.close();

    return squadCount - 1;
}

void squad_manager::changeState(int squad, string newCity){
    string fileName = "squad" + to_string(squad) + ".txt";
    ifstream iistream;
    fstream stream;
    string tmp;
    int currentState;

    iistream.open("data//squads//" + fileName);
    iistream >> tmp >> currentState;
    iistream.close();

    switch(currentState){
        case 0: currentState = 1; break;
        case 1: currentState = 0; break;
        default: cout << "Current state of squad " << squad << " is not valid. , 1 is for 'in a city', 0 is for 'on the map'.";
    }

    stream.open("data//squads//" + fileName);
    stream << "state: " << currentState << "\n";
    if(currentState == 0){ stream << "city: " << newCity; }else{ stream << "city: " << "                "; }
    stream.close();
}

void squad_manager::deleteSquad(int squad){
    string fileName = "squad" + to_string(squad) + ".txt";
    string p0, tmp, str_search = "squad" + to_string(squad) + ".txt";
    string str_replace = "", str_remove = "data//squads//" + fileName;
    string str_remove1 = "data//squads//soldier data//soldier_data_" + to_string(squad) + ".txt";
    fstream stream;
    ofstream oostream;
    ofstream ostream;

    stream.open("data//all_squads.txt");
    for(int i = 0; i < squadCount; i++){
        stream >> tmp;
        p0 += tmp + "\n";
    }
    stream.close();

    oostream.open("data//all_squads.txt");
    int ind = p0.find(str_search);
    p0.replace(ind, string(str_search).length(), str_replace);
    oostream << p0;
    oostream.close();

    remove(str_remove.c_str());
    remove(str_remove1.c_str());
    /*squadCount--;
    ostream.open("data//squadCount.txt");
    ostream << squadCount;
    ostream.close();*/
}
///SOLDIERS API
void squad_manager::addSoldier(int squad, int numberOfSoldiers, SQUAD type, int coordX, int coordY){
    string filename = "soldier_data_" + to_string(squad) + ".txt";
    fstream streamf;

    /*soldier newSoldier;
    newSoldier.health = 100;
    newSoldier.type = type;
    newSoldier.coord.x = coordX;
    newSoldier.coord.y = coordY;*/

    D(filename);

    streamf.open("data//squads//soldier data//" + filename);
    //soldiers.push_back();
    streamf.seekg(streamf.tellg(), ios::end);
    streamf << (int)type << " UNITS: " << to_string(numberOfSoldiers) << " COORDINATES: " << to_string(coordX) << " " << to_string(coordY) << "\n";
    streamf.close();
}

void squad_manager::changeUnits(int squadIndex, SQUAD unitType, int coordX, int coordY, int currUnits, int value){
    ofstream stream;
    string filename = "soldier_data_" + to_string(squadIndex) + ".txt";
    string str_search = to_string((int)unitType) + " UNITS: " + to_string(currUnits) + " COORDINATES: " + to_string(coordX) + " " + to_string(coordY);

    ifstream infile { "data//squads//soldier data//" + filename };
    string file_contents { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };

    int newUnits = currUnits + value;
    int indF = file_contents.find(str_search);
    file_contents.replace(indF, string(str_search).length(), to_string((int)unitType) + " UNITS: " + to_string(newUnits) + " COORDINATES: " + to_string(coordX) + " " + to_string(coordY));

    stream.open("data//squads//soldier data//" + filename);
    stream << file_contents;
    stream.close();
}

void squad_manager::changeSoldierCoords(int squad, int currCoordX, int currCoordY, int newCoordX, int newCoordY){
    ofstream stream;
    string filename = "soldier_data_" + to_string(squad) + ".txt";
    string str_search = "COORDINATES: " + to_string(currCoordX) + " " + to_string(currCoordY);

    ifstream infile { "data//squads//soldier data//" + filename };
    string file_contents { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };

    int indF = file_contents.find(str_search);
    file_contents.replace(indF, string(str_search).length(), "COORDINATES: " + to_string(newCoordX) + " " + to_string(newCoordY));

    stream.open("data//squads//soldier data//" + filename);
    stream << file_contents;
    stream.close();
}

void squad_manager::killSoldier(int squad, int type, int coordX, int coordY){
    ofstream stream;
    string filename = "soldier_data_" + to_string(squad) + ".txt";
    string str_search = to_string(type) + " UNITS: 0 COORDINATES: " + to_string(coordX) + " " + to_string(coordY) + "\n";

    ifstream infile { "data//squads//soldier data//" + filename };
    string file_contents { istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };

    int indF = file_contents.find(str_search);
    file_contents.replace(indF, string(str_search).length(), "");

    stream.open("data//squads//soldier data//" + filename);
    stream << file_contents;
    stream.close();
}
