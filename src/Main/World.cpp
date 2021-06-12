#include "World.h"

World::World()
{
    m_main_window = NULL;
    m_main_renderer = NULL;
    m_gameState = NOSCENE;
    m_quitScene = false;
    m_cameraOffset.x = 0;
    m_cameraOffset.y = 0;
}

World::~World()
{
    //dtor
}

void World::initSDL(string configFile)
{

    srand(time(NULL));

    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    string cursorImg;

    stream.open(configFile.c_str());
    stream >> tmp >> m_SCREEN_WIDTH >> m_SCREEN_HEIGHT;
    stream >> tmp >> m_CP1.r >> m_CP1.g >> m_CP1.b;
    stream >> tmp >> m_CP2.r >> m_CP2.g >> m_CP2.b;
    stream >> tmp >> cursorImg;

    stream.close();

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    m_main_window = SDL_CreateWindow("Montu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_SCREEN_WIDTH, m_SCREEN_HEIGHT, 0);
    m_main_renderer = SDL_CreateRenderer(m_main_window, -1, SDL_RENDERER_PRESENTVSYNC);

    improveRenderer();

    m_squadManager.init();
    m_configManager.init("config_manager.txt", m_main_renderer);
    m_soundManager.init("SoundManager.txt");
    m_pickAndBan.init("pick_And_Ban.txt", m_main_renderer);
    m_playerStatsManager.init("mainStats.txt");
    m_battle.initBattle("battle_manager.txt");
    m_menu.init("menu.txt");
    m_castleUI.init("castleUI.txt", "Tsarevo", m_main_renderer);
    cursorImg = "img\\" + cursorImg;
    SDL_Surface* loadSurface = SDL_LoadBMP(cursorImg.c_str());
    m_cursor = SDL_CreateColorCursor(loadSurface, 10, 5);
    SDL_SetCursor(m_cursor);

    ///m_soundManager.play_sound("General.mp3");

    // ShowWindow(GetConsoleWindow(), SW_HIDE);
}

void World::improveRenderer()
{
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto desktopWidth = DM.w;
    auto desktopHeight = DM.h;

    m_MOUSE_MULTIPLIER_X = (double)m_SCREEN_WIDTH / (double)desktopWidth;
    m_MOUSE_MULTIPLIER_Y = (double)m_SCREEN_HEIGHT / (double)desktopHeight;

    if (SDL_SetWindowFullscreen(m_main_window, SDL_WINDOW_FULLSCREEN_DESKTOP) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
    }

    SDL_RenderSetLogicalSize(m_main_renderer, 1920, 1080);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

void World::destroySDL()
{
    m_gameState = NOSCENE;
    SDL_DestroyRenderer(m_main_renderer);
    SDL_DestroyWindow(m_main_window);
}

void startDrag(void* input)
{
    World* world = (World*)input;
    world->m_drag = true;
}

void stopDrag(void* input)
{
    World* world = (World*)input;
    world->m_drag = false;
}

int event_filter(void* input, SDL_Event* event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        startDrag(input);
    }
    if (event->type == SDL_MOUSEBUTTONUP)
    {
        stopDrag(input);
    }

    return 1;
}

void World::input()
{
    m_event.type = SDLK_UNKNOWN;
    m_mouseIsPressed = false;
    m_mouseIsReleased = false;
    m_mouseIsDoubleClicked = false;
    m_buttonDown = false;

    SDL_SetEventFilter(&event_filter, (void*)this);

    SDL_PollEvent(&m_event);

    if (m_event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (m_event.button.clicks >= 1)
        {
            m_mouseIsPressed = true;
        }
        if (m_event.button.clicks >= 2)
        {
            m_mouseIsDoubleClicked = true;
        }
    }
    if (m_event.type == SDL_MOUSEBUTTONUP)
    {
        m_mouseIsReleased = true;
    }
    if (m_event.type == SDL_MOUSEMOTION)
    {
        //cout << "RELATIVE" << m_event.motion.xrel << " " << m_event.motion.yrel << endl;
        SDL_GetGlobalMouseState(&(m_mouse.x), &(m_mouse.y));
        m_mouse.x *= m_MOUSE_MULTIPLIER_X;
        m_mouse.y *= m_MOUSE_MULTIPLIER_Y;
    }

    if (m_event.type == SDL_KEYDOWN)
    {
        m_buttonDown = true;
    }
    //cout << m_mouse.x << " " << m_mouse.y << endl;
}

void World::cameraShake()
{
    if (m_startShake + m_cameraShakeDuration > time(NULL))
    {
        // We pick a number random for the magnitude and than we decide if we add, or if we subtract
        // this number from the cameraOffset
        m_cameraOffset.x += (rand() % m_cameraShakeMagnitude) * (rand() % 2 == 0 ? 1 : -1);
        m_cameraOffset.y += (rand() % m_cameraShakeMagnitude) * (rand() % 2 == 0 ? 1 : -1);
        ///cout << m_cameraOffset.x << " " << m_cameraOffset.y << endl;
    }
    else
    {
        m_cameraOffset.x = 0;
        m_cameraOffset.y = 0;
    }
}

void World::initSession(GAME_STATE state)
{
    if (state == PICK_BAN)
    {
        m_available.push_back(ARCHER);
        m_available.push_back(WARRIOR);
    }
}

void World::pickAndBan()
{
    //Check for mouse input and select squad to ban
    if (false)
    {
        ///m_banned.push_back();
    }
}

void World::initMap(string configFile)
{
    configFile = "config\\Maps\\" + configFile;

    fstream in_file;

    in_file.open(configFile.c_str());

    for (int r = 0; r < m_battle.m_rows; r++)
    {
        for (int c = 0; c < m_battle.m_colls; c++)
        {
            in_file >> field[c][r];
        }
    }

    in_file.close();
}
