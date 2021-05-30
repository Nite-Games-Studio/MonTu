#include "Engine.h"

SDL_Texture* LoadTexture(string file, SDL_Renderer* renderer)
{
    SDL_Texture* objectTexture;
    try
    {
        file = "img\\" + file;

        SDL_Surface* loadingSurface = SDL_LoadBMP(file.c_str());
        objectTexture = SDL_CreateTextureFromSurface(renderer, loadingSurface);
        SDL_FreeSurface(loadingSurface);

        if (objectTexture == NULL)
        {
            throw(1001);
        }
    }catch (int i)
    {
        cout << "NO TEXTURE LOADED " << file << "\n";
        return NULL;
    }
    return objectTexture;
}

SDL_Point* LoadPoint(coordinates coor)
{
    SDL_Point* point = new SDL_Point;
    point->x = coor.x;
    point->y = coor.y;

    return point;
}

void CoordinatesToRect(coordinates coor, SDL_Rect& rect)
{
    rect.x = coor.x;
    rect.y = coor.y;
}

int hexDistance(coordinates a, coordinates b)
{
    return (abs(a.x - b.x) + abs(a.x + a.y - b.x - b.y) + abs(a.y - b.y)) / 2;
}

float triangleArea(SDL_Point* point1, SDL_Point* point2, SDL_Point* point3)
{
   return abs((point1->x * (point2->y - point3->y) + point2->x * (point3 -> y - point1->y) + point3 -> x*(point1->y - point2->y))/2.0);
}

float returnAngleByCoordinates(coordinates direction)
{
    return atan2(direction.x, -1*direction.y) * 180 / PI;
}

bool isInsideATriangle(SDL_Point* triangle1, SDL_Point* triangle2, SDL_Point* triangle3, SDL_Point* mousePoint)
{
   float A = triangleArea (triangle1, triangle2, triangle3);

   float A1 = triangleArea (mousePoint, triangle2, triangle3);

   float A2 = triangleArea (triangle1, mousePoint, triangle3);

   float A3 = triangleArea (triangle1, triangle2, mousePoint);

   return (A == A1 + A2 + A3);
}

bool isInsideAHexagon(vector<SDL_Point*> points, SDL_Point* mousePoint)
{
    for(unsigned int i = 0; i < points.size(); i++)
    {
        for(unsigned int j = i + 1; j < points.size(); j++)
        {
            for(unsigned int m = j + 1; m < points.size(); m++)
            {
                if(isInsideATriangle(points[i], points[j], points[m], mousePoint) == true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool checkForMouseCollision(int mouseX, int mouseY, SDL_Rect object)
{
    if(mouseX > object.x && mouseX < object.x + object.w && mouseY > object.y && mouseY < object.y + object.h)
    {
        return true;
    }
    return false;
}

bool checkForMouseCollision(int mouseX, int mouseY, coordinates objectCoor, int objectSize)
{
    if(mouseX > objectCoor.x && mouseX < objectCoor.x + objectSize && mouseY > objectCoor.y && mouseY < objectCoor.y + objectSize)
    {
        return true;
    }
    return false;
}

bool checkForPossibleMove(int distance, int &movement)
{
    if(distance <= movement)
    {
        return true;
        /// movement -= distance;
    }
    return false;
}

bool checkIfInScreen(SDL_Rect rect, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
    if (rect.x + rect.w > SCREEN_WIDTH)
        return false;
    if (rect.x < 0)
        return false;
    if (rect.y + rect.h > SCREEN_HEIGHT)
        return false;
    if (rect.y < 0)
        return false;
    return true;
}

void write(string text, coordinates coor, SDL_Renderer* renderer, int FONT_SIZE)
{
    SDL_Texture* texture;
    SDL_Surface* surface;
    SDL_Rect rect;
    SDL_Color fcolor;
    TTF_Font* font;

    string str = "ttf/Perpetua-Titling-MT.ttf";
    // Basically translates to pixel size
    font = TTF_OpenFont(str.c_str(), FONT_SIZE);

    if (font == NULL)
    {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }

    fcolor.r = 255;
    fcolor.g = 164;
    fcolor.b = 92;
    const char* t = text.c_str();
    surface = TTF_RenderText_Solid(font, t, fcolor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect.w = surface->w;
    rect.h = surface->h;
    rect.x = coor.x;
    rect.y = coor.y;
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

bool checkForCollisionBetweenRects(SDL_Rect rect1, SDL_Rect rect2)
{
    if (rect1.x < rect2.x + rect2.w &&
    rect1.x + rect1.w > rect2.x &&
    rect1.y < rect2.y + rect2.h &&
    rect1.y + rect1.h > rect2.y) {
        return true;
    }
    return false;
}

unsigned short alignCenter(unsigned short fullSpace, unsigned short objWidth)
{
    return (fullSpace - objWidth) / 2;
}
