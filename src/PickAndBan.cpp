#include "PickAndBan.h"

PickAndBan::PickAndBan()
{
    //ctor
}

PickAndBan::~PickAndBan()
{
    //dtor
}

void PickAndBan::init(string configFile, SDL_Renderer* renderer)
{
    // Passing the main renderer from the world
    m_renderer = renderer;

    int numberOfSquads;
    int iconHight, iconWidth;
    // Reading from the configuration file
    configFile = "config\\" + configFile;
    fstream stream;

    // Creating this buff so we can read the name of the img for SDL_Texture
    string buff;
    // Creating tmp
    string tmp;

    m_Map1Button.x = 188;
    m_Map1Button.y = 167;
    m_Map1Button.w = 361;
    m_Map1Button.h = 194;

    m_Map2Button.x = 815;
    m_Map2Button.y = 167;
    m_Map2Button.w = 361;
    m_Map2Button.h = 194;

    m_Map3Button.x = 188;
    m_Map3Button.y = 467;
    m_Map3Button.w = 361;
    m_Map3Button.h = 194;

    m_Map4Button.x = 815;
    m_Map4Button.y = 467;
    m_Map4Button.w = 361;
    m_Map4Button.h = 194;
/*
    stream.open(configFile.c_str());

    // Reading the number of squads
    stream >> tmp >> numberOfSquads;
    stream >> tmp >> iconWidth >> iconHight;
    stream >> tmp >> buff;
    m_backgroundTexture = LoadTexture(buff, m_renderer);

    for(int i = 0; i < numberOfSquads; i ++)
    {
        // creating model, that we will push-back to the vector
        icon* model = new icon;

        // implementing all SQUAD types
        model->m_type = (SQUAD)i;

        // all icons should start form AVAILABLE and than transition to either picked or banned
        model->m_iconState = AVAILABLE;

        // taking the coordinates for the rectangle, where we will draw the object
        stream >> tmp >> model->m_rect.x >> model->m_rect.y;
        model->m_rect.w = iconWidth;
        model->m_rect.h = iconHight;

        // using the already created buff for the SDL_Texture of each icon
        stream >> tmp >> buff;
        model->m_texture = LoadTexture(buff, m_renderer);

        //pushing back the model to the vector
        m_icons.push_back(model);
    }
    // Closing the input stream
    stream.close();
*/
}

// Updates per frame
void PickAndBan::update()
{
    for (int i = 0; i < m_icons.size(); i ++)
    {
        if(checkForMouseCollision(0, 0 , m_icons[i]->m_rect))
        {
            cout << "clicked " << i << endl;
        }
    }
}

// Draws per frame
void PickAndBan::draw()
{
    // Drawing the background
    SDL_RenderCopy(m_renderer, m_backgroundTexture, NULL, NULL);

    // Drawing all icons
    for (int i = 0; i < m_icons.size(); i ++)
    {
        SDL_RenderCopy(m_renderer, m_icons[i]->m_texture, NULL, &(m_icons[i]->m_rect));
    }

    // Presenting the renderer
    SDL_RenderPresent(m_renderer);
}

// Use this function for picking a certain squad type
void PickAndBan::pick(SQUAD picked)
{
    // Go through the whole vector
    // When you find the searched type -> make it's state PICKED
    for(int i = 0; i < m_icons.size(); i ++)
    {
        if(m_icons[i]->m_type == picked)
        {
            m_icons[i]->m_iconState = PICKED;
        }
    }
}

// Use this function for banning a certain squad type
void PickAndBan::ban(SQUAD banned)
{
    // Go through the whole vector
    // When you find the searched type -> make it's state BANNED
    for(int i = 0; i < m_icons.size(); i ++)
    {
        if(m_icons[i]->m_type == banned)
        {
            m_icons[i]->m_iconState = BANNED;
        }
    }
}
