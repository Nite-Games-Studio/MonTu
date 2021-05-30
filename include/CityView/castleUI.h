#ifndef CASTLEUI_H
#define CASTLEUI_H

#include <SDL.h>

#include "Engine.h"
#include "defines.h"

struct squadSlot : public Button
{
    soldier* data;
};

struct createSquadElement
{
    UI_object m_pic;
    string m_typeName;
    unsigned short m_numberOfSoldiers;
    SQUAD m_type;
    SDL_Rect m_upBtnRect;
    SDL_Rect m_downBtnRect;
};

struct newSquadData
{
    SDL_Rect m_pic;
    SDL_Rect m_upBtnRect;
    SDL_Rect m_downBtnRect;
    unsigned short m_numberOfSoldiers;
};

class castleUI
{
public:
    castleUI();
    virtual ~castleUI();

    /// main
    string m_cityName;

    SDL_Rect m_tabButtons[3];
    SDL_Renderer* m_renderer;

    coordinates m_startOfCreateSquad;
    unsigned short m_createSquadMargin;

    /// city squad
    Button m_hoveredSlot;
    UI_object m_mainWindow[3];
    UI_object m_draggedSlot;
    SDL_Rect m_colliders[3][8];
    vector<squadSlot*> m_squads;
    short m_draggedSquad;
    coordinates m_lastHoveredSlot;
    SDL_Texture* m_slotTexture;

    /// create squad
    SDL_Texture* m_arrowTexture;
    Button m_createSquadButton;
    UI_object m_createSquadWindow;
    unsigned short m_arrowSize;
    unsigned short m_squadElWidth;
    unsigned short m_newSquadWidth;
    unsigned short m_newSquadMargin;
    unsigned short m_startOfArrows;
    unsigned short m_newSquadArrowWidth;
    coordinates m_startOfNewSquadWindow;
    vector<soldier*> m_data;
    newSquadData* m_newSquadData[5];
    createSquadElement* m_createSquadEl[5];

    /// main
    void init(string configFile, string cityName, SDL_Renderer* renderer);
    void loadData(string configFile);
    void saveData(string configFile);
    void hover();
    void moveSquad();

    void update();
    void draw();

    /// city squad
    void updateCitySquad();
    void drawCitySquad();

    /// create squad
    void updateCreateSquad();
    void drawCreateSquad();
    void updateCreateSquadElements();
    string loadSquadName(SQUAD type);

    void createSquad();
    void createSquadHover();
    void removeUnits();

    SDL_Texture* loadSquadTexture(SQUAD type);
protected:

private:
    unsigned short m_tab;
};


#endif // CASTLEUI_H
