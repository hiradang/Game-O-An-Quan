#ifndef DRAW__H
#define DRAW__H

#include "const_list.h"
#include "engine.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

class vObject
{
public:
    vObject();
    ~vObject();
    void loadTexture(string path, SDL_Renderer* mRenderer);
    void render(SDL_Renderer* mRenderer, int x, int y, int w, int h);
private:
    SDL_Rect clip;
    SDL_Texture* tTexture = NULL;
};

class vText
{
public:
    void create(const int &x1, const int &y1, const int &x2, const int &y2, const std::string _content, const SDL_Color &_color);
    void render(TTF_Font* mFont, SDL_Renderer *&mRenderer);
    void update(string newContent);
private:
    string content = "NULL";
    int x = 0, y = 0, w = 0, h = 0;
    SDL_Color color = {0, 0, 0};
};

class VisualMaker
{
public:
    VisualMaker();
    ~VisualMaker();
    void initAll(const string allPath[], const string fontPath, int fontSize, int fontSize2);
    void drawAll(Boardgame mBoard);
    void drawEndgame(Boardgame mBoard);
    string pathmaskGen(string mask, int x);
    void drawMenu(string menuPath);
    void initTutor();
    void drawTutor(int i);
private:
    SDL_Window* mWindow = NULL;
    SDL_Renderer* mRenderer = NULL;
    TTF_Font* mFont = NULL;
    TTF_Font* mFont2 = NULL;
    vObject background;
    vObject gameover;
    vObject menu;
    vObject mapTile[NUM_OF_MAP];
    vObject turnMark;
    vObject cursor;
    vObject arrow;
    vObject tutor[NUM_OF_TUTOR_PAGE];
    vText nameText[2];
    vText scoreText[2];
    vText popup[2];
    vText yourTurn[2];
};

#endif // DRAW__H
