#include <windows.h>

#include "TitleScreen.h"
extern World world;

TitleScreen::TitleScreen()
{
    //ctor

    m_objectRect.w = 1920;
    m_objectRect.h = 1080;
    m_currState = 0;

}

TitleScreen::~TitleScreen()
{
    //dtor
}

void TitleScreen::load(string configFile, SDL_Renderer* renderer)
{

    configFile = "config\\" + configFile;

    ifstream stream;
    stream.open(configFile.c_str());

    string tmp;
    stream >> tmp >> m_img;
    stream >> tmp >> m_numberOfStates;

    m_img = "TitleScreens\\" + m_img;

    stream.close();

    //m_objectTexture = LoadTexture(m_img , renderer);


}

void TitleScreen::update()
{

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    SDL_PollEvent(&(world.m_event));

    if(state[SDL_SCANCODE_SPACE] && world.m_buttonDown)
        {
            world.m_quitScene = true;
            world.m_gameState = MENU;
        }

}

void TitleScreen::draw(SDL_Renderer* renderer)
{

//{
    //m_objectRect.x = m_currState*m_objectRect.w;

    /*if(m_currState >= m_numberOfStates/2){

        m_objectRect.x = (m_currState-m_numberOfStates/2)*m_objectRect.w;
        m_objectRect.y = 1080;


    }else{

         m_objectRect.y = 0;

    }
    D(m_objectRect.y);*/
//}

    m_imgCurr = m_img + to_string(m_currState + 1) + ".bmp";

    m_objectTexture = LoadTexture(m_imgCurr, renderer);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, m_objectTexture , /*&m_objectRect*/ NULL, NULL);
    SDL_RenderPresent(renderer);


    if(m_currState == m_numberOfStates-1 ){

        m_currState = 0;
    }else{

        m_currState++;

    }

    SDL_Delay(150);



}
