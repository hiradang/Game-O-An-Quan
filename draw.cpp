#include "draw.h"

vObject::vObject()
{
    clip.x = 0;
    clip.y = 0;
}

vObject::~vObject()
{
    if (tTexture != NULL)
    {
        SDL_DestroyTexture(tTexture);
        tTexture = NULL;
    }
}

void vObject::loadTexture(string path, SDL_Renderer* mRenderer)
{
    SDL_Surface* tSurface = IMG_Load(path.c_str());
    if (tSurface == NULL)
    {
        cout << "IMG_Load error:   " << SDL_GetError() << "\n";
        exit(1);
    }
    SDL_SetColorKey(tSurface, SDL_TRUE, SDL_MapRGB(tSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
    tTexture = SDL_CreateTextureFromSurface(mRenderer, tSurface);
    if ( tTexture != NULL)
    {
        clip.h = tSurface->h;
        clip.w = tSurface->w;
    }
    SDL_FreeSurface(tSurface);
    if (tTexture == NULL)
    {
        cout << "SDL_CreateTextureFromSurface error:   " << SDL_GetError() << "\n";
        exit(1);
    }
}

void vObject::render(SDL_Renderer* mRenderer, int x, int y, int w, int h)
{
    SDL_Rect target = {x, y, w, h};
    SDL_Rect quad = {clip.x, clip.y, clip.w, clip.h};
    SDL_RenderCopy(mRenderer, tTexture, &quad, &target);
}

void vText::create(const int &x1, const int &y1, const int &x2, const int &y2, const std::string _content, const SDL_Color &_color)
{
    x = x1;
    y = y1;
    w = x2 - x1;
    h = y2 - y1;
    content = _content;
    color = _color;
}

void vText::render(TTF_Font* mFont, SDL_Renderer *&mRenderer)
{
    SDL_Texture* tTexture = NULL;
    SDL_Surface* tSurface = TTF_RenderText_Solid(mFont, content.c_str(), color);
    if (tSurface == NULL)
    {
        cout << "TTF_RenderText_Solid error:   " << TTF_GetError() << "\n";
        exit(1);
    }
    tTexture = SDL_CreateTextureFromSurface(mRenderer, tSurface);
    if (tTexture == NULL)
    {
        cout << "SDL_CreateTextureFromSurface error:   " << SDL_GetError() << "\n";
        exit(1);
    }
    int aw = tSurface->w, ah = tSurface->h; // actual font print size
    int ax = x, ay = y + h - tSurface->h; // actual font print coordinate: left-down
    SDL_FreeSurface(tSurface);

    SDL_Rect coor = {ax, ay, aw, ah};
    SDL_RenderCopy(mRenderer, tTexture, NULL, &coor);

    SDL_DestroyTexture(tTexture);
}

void vText::update(string newContent)
{
    content = newContent;
}

VisualMaker::VisualMaker()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "SDL_Init error:   " << SDL_GetError() << "\n";
        exit(1);
    }
    mWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == NULL)
    {
        cout << "SDL_CreateWindow error:   " << SDL_GetError() << "\n";
        exit(1);
    }
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (mRenderer == NULL)
    {
        cout << "SDL_Create_Renderer error:   " << SDL_GetError() << "\n";
        exit(1);
    }
    if (TTF_Init() < 0)
    {
        cout << "TFF_Init error:   " << TTF_GetError() << "\n";
        exit(1);
    }
}

VisualMaker::~VisualMaker()
{
    TTF_CloseFont(mFont);
    mFont = NULL;
    TTF_Quit();
    SDL_DestroyRenderer(mRenderer);
    mRenderer = NULL;
    SDL_DestroyWindow(mWindow);
    mWindow = NULL;
    SDL_Quit();
}

void VisualMaker::initAll(const string allPath[], const string fontPath, int fontSize, int fontSize2)
{
    background.loadTexture(allPath[0], mRenderer);
    gameover.loadTexture(allPath[1], mRenderer);
    for (int i = 0; i < NUM_OF_MAP; ++ i)
    {
        string mapPath = pathmaskGen("map", i);
        mapTile[i].loadTexture(mapPath, mRenderer);
    }
    turnMark.loadTexture(allPath[2], mRenderer);
    cursor.loadTexture(allPath[3], mRenderer);
    arrow.loadTexture(allPath[4], mRenderer);
    mFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (mFont == NULL)
    {
        cout << "TTF_OpenFont " << fontPath << " error:   " << TTF_GetError() << "\n";
        exit(1);
    }
    mFont2 = TTF_OpenFont(fontPath.c_str(), fontSize2);
    if (mFont2 == NULL)
    {
        cout << "TTF_OpenFont " << fontPath << " error:   " << TTF_GetError() << "\n";
        exit(1);
    }
    nameText[0].create(60, 40, 220, 80, PLAYER_NAME[0], {0, 0, 0});
    nameText[1].create(500, 400, 680, 440, PLAYER_NAME[1], {0, 0, 0});
    scoreText[0].create(260, 40, 300, 80, "00", {0, 0, 0});
    scoreText[1].create(420, 400, 460, 440, "00", {0, 0, 0});
    popup[0].create(420, 360, 460, 390, "hi", {0, 0, 255});
    popup[1].create(260, 0, 300, 30, "hi", {0, 0, 255});
    yourTurn[0].create(60, 0, 220, 30, "Your turn", {0, 0, 255});
    yourTurn[1].create(500, 360, 680, 390, "Your turn", {0, 0, 255});
}

void VisualMaker::drawAll(Boardgame mBoard)
{
    SDL_RenderClear(mRenderer);
    background.render(mRenderer, 0, 0, 720, 480);
    for (int i = 0; i < 12; ++ i) //render pawn map
    {
        int x = 80;
        if (i < 7) x += i * 80;
        else x += (12 - i) * 80;
        int y = 160 + i / 7 * 80;

        int v = mBoard.getPawnTile(i);
        mapTile[v].render(mRenderer, x, y, 80, 80);
    }
    for (int i = 0; i < 2; ++ i) //render king tile map
    {
        int x = 80 + i * 480;
        int y = 240;
        if (mBoard.getKingTile(i) > 0)
        {
            mapTile[31].render(mRenderer, x, y, 80, 80);
        }
        else
        {
            mapTile[0].render(mRenderer, x, y, 80, 80);
        }
    }
    { //render hand
        int x = mBoard.getHand();
        if (mBoard.getTurn() == 0)
        {
            mapTile[x].render(mRenderer, 420, 40, 80, 80);
            mapTile[0].render(mRenderer, 260, 360, 80, 80);
        }
        else
        {
            mapTile[0].render(mRenderer, 420, 40, 80, 80);
            mapTile[x].render(mRenderer, 260, 360, 80, 80);
        }
    }
    { //rendre cursor
        int x, y, pos = mBoard.getCursor();
        if (pos < 6)
        {
            x = 150 + 80 * (pos - 1);
            y = 150;
        }
        else
        {
            x = 150 + (11 - pos) * 80;
            y = 230;
        }
        if (mBoard.getLocker()) arrow.render(mRenderer, x, y, 100, 100);
        else cursor.render(mRenderer, x, y, 100, 100);
    }
    for (int i = 0; i < 2; ++ i) //render text
    {
        scoreText[i].update(to_string(mBoard.getScore(i)));
        nameText[i].render(mFont, mRenderer);
        scoreText[i].render(mFont, mRenderer);
    }
    //render turn mark & pop up
    int turn = mBoard.getTurn();

    if (turn == 0) turnMark.render(mRenderer, 30, 20, 300, 80);
    else turnMark.render(mRenderer, 390, 380, 300, 80);
    yourTurn[turn].render(mFont2, mRenderer);

    int lastScored = mBoard.getLastScored();
    if (lastScored != -10)
    {
        popup[turn].update("+" + to_string(lastScored));
        popup[turn].render(mFont2, mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

void VisualMaker::drawEndgame(Boardgame mBoard)
{
    int winner = -1;
    if (mBoard.getScore(0) > mBoard.getScore(1)) winner = 0;
    if (mBoard.getScore(0) < mBoard.getScore(1)) winner = 1;
    nameText[0].create(210, 340, 460, 380, PLAYER_NAME[0], {0, 0, 0});
    nameText[1].create(210, 380, 460, 420, PLAYER_NAME[1], {0, 0, 0});
    scoreText[0].create(460, 340, 560, 380, to_string(mBoard.getScore(0)), {0, 0, 0});
    scoreText[1].create(460, 380, 560, 420, to_string(mBoard.getScore(1)), {0, 0, 0});
    if (winner > -1) popup[0].create(260, 210, 460, 250, PLAYER_NAME[winner] + " wins", {0, 0, 255});
    else popup[0].create(240, 210, 480, 250, "It's a draw. GGWP", {0, 0, 255});

    SDL_RenderClear(mRenderer);
    gameover.render(mRenderer, 0, 0, 720, 480);
    for (int i = 0; i < 2; ++ i)
    {
        nameText[i].render(mFont, mRenderer);
        scoreText[i].render(mFont, mRenderer);
    }
    popup[0].render(mFont, mRenderer);

    SDL_RenderPresent(mRenderer);
}

string VisualMaker::pathmaskGen(string mask, int x)
{
    return "Visual Files/" + mask + "/" + mask + to_string(x) + ".png";
}

void VisualMaker::drawMenu(string menuPath)
{
    menu.loadTexture(menuPath.c_str(), mRenderer);
    SDL_RenderClear(mRenderer);
    menu.render(mRenderer, 0, 0, 720, 480);
    SDL_RenderPresent(mRenderer);
}

void VisualMaker::initTutor()
{
    for (int i = 0; i < NUM_OF_TUTOR_PAGE; ++ i)
    {
        string tutorPath = pathmaskGen("tutorial", i);
        tutor[i].loadTexture(tutorPath, mRenderer);
    }
}

void VisualMaker::drawTutor(int i)
{
    SDL_RenderClear(mRenderer);
    tutor[i].render(mRenderer, 0, 0, 720, 480);
    SDL_RenderPresent(mRenderer);
}
