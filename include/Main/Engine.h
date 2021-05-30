#ifndef ENGINE_H
#define ENGINE_H

#define PI 3.14

#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include <SDL.h>
#include "SDL_TTF.h"

#include "defines.h"

using namespace std;

SDL_Texture* LoadTexture(string file, SDL_Renderer* renderer);
SDL_Point* LoadPoint(coordinates coor);
int hexDistance(coordinates a, coordinates b);
float triangleArea (int x1, int y1, int x2, int y2, int x3, int y3);
float returnAngleByCoordinates(coordinates direction);
bool isInsideATriangle (SDL_Point* triangle1, SDL_Point* triangle2, SDL_Point* triangle3, SDL_Point* mousePoint);
bool isInsideAHexagon (vector<SDL_Point*> points, SDL_Point* mousePoint);
bool checkForMouseCollision(int mouseX, int mouseY, SDL_Rect object);
bool checkForMouseCollision(int mouseX, int mouseY, coordinates objectCoor, int objectSize);
bool checkForPossibleMove(int distance, int &movement);
bool checkIfInScreen(SDL_Rect rect, int SCREEN_WIDTH, int SCREEN_HEIGHT);
bool checkForCollisionBetweenRects(SDL_Rect rect1, SDL_Rect rect2);
unsigned short alignCenter(unsigned short fullSpace, unsigned short objWidth);

void write(string text, coordinates coor, SDL_Renderer* renderer, int FONT_SIZE);
void CoordinatesToRect(coordinates coor, SDL_Rect& rect);
#endif // ENGINE_H
