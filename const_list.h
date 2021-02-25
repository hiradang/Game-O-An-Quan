#ifndef CONST_LIST__H
#define CONST_LIST__H

#include <iostream>

using namespace std;

const string WINDOW_TITLE = "K64J OANQUAN version 09";
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;
const int COLOR_KEY_B = 255;
const int COLOR_KEY_G = 255;
const int COLOR_KEY_R = 255;
const string OBJECT_PATH[] =
{
    "Visual Files/background.png",
    "Visual Files/gameover.png",
    "Visual Files/turnmark.png",
    "Visual Files/cursor.png",
    "Visual Files/arrow.png"
};
const string MENU_PATH = "Visual Files/menu.png";
const string FONT_PATH = "Visual Files/ComicNeue-Regular.ttf";
const int NUM_OF_MAP = 32;
const int NUM_OF_TUTOR_PAGE = 6;
const string PLAYER_NAME[2] = {"Player 1", "Player 2"};
const int SCATTERNEW_DELAY = 150;
const int SCATTER_DELAY = 350;
const int COLLECT_DELAY = 400;
const int SCORE_DELAY = 500;

#endif // CONST_LIST__H
