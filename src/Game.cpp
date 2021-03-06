#include"Game.h"

using namespace std;

int main(int argc, char** argv)
{

    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_TIMER ) != 0)
    {
        cout << "SDL Initialization error. " << SDL_GetError() << endl;
        exit(-1);
    }

    SDL_Window* gameWindow = SDL_CreateWindow(
                                 "plotfarmer",
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH,
                                 SCREEN_HEIGHT,
                                 //SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
                                 0
                             );
    if (gameWindow == NULL)
    {
        cout << "SDL Window Initialization error. " << SDL_GetError() << endl;
        exit(-1);
    }

    SDL_Renderer* gameRenderer = SDL_CreateRenderer(
                                     gameWindow,
                                     -1,
                                     SDL_RENDERER_ACCELERATED
                                 );

    if (gameRenderer == NULL)
    {
        cout << "SDL Renderer Initialization error. " << SDL_GetError()
             << endl;
        exit(-1);
    }

    if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG))
    {
        cout << "SDL Renderer Initialization error. " << SDL_GetError()
             << endl;
        exit(-1);
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        cout << "SDL Image Initialization Error. "  << endl;
        exit(-1);
    }

    Game* plotfarmerGame = new Game(
        gameWindow,
        gameRenderer
    );

    plotfarmerGame->gameMainLoop();

    delete plotfarmerGame;

    SDL_Quit();
}


Game::Game(SDL_Window* w, SDL_Renderer* r) :
    gameOver(false),
    gameWindow(w),
    gameRenderer(r),
    level(),
    player(r, level, camera, 1, 1),
    tilemanager(r, level, camera),
    camera({0, 0})
{
}

Game::~Game()
{
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
}

void Game::gameMainLoop()
{
    long next_game_tick = SDL_GetTicks();
    int loops;
    float interpolation;
    while ( !gameOver )
    {
        loops = 0;
        while ( SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP)
        {
            update();
            next_game_tick += SKIP_TICKS;
            loops++;
        }

        interpolation = float( SDL_GetTicks() + SKIP_TICKS - next_game_tick )
                        / float( SKIP_TICKS );

        render(interpolation);
    }
}

void Game::handleKeys()
{
    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT])
        player.setDirection(DIRECTIONLEFT);
    if (state[SDL_SCANCODE_RIGHT])
        player.setDirection(DIRECTIONRIGHT);
    if (!state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT])
        player.stop();
    if (state[SDL_SCANCODE_UP])
        player.jump();
    if (state[SDL_SCANCODE_X] && player.readyToFire())
    {
        vec2di pos = player.getStaffPosition();
        std::shared_ptr<MagicBullet> bullet = make_shared<MagicBullet>
                                              (pos.x, pos.y,
                                               gameRenderer, camera, level, player.getDirection());
        gameObjects.push_back(bullet);
        player.hightenBulletCounter();
    }
}

void Game::update()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            gameOver = true;
        }
    }
    handleKeys();

    player.update();
    //delete dead Objects
    gameObjects.erase(std::remove_if
                      (gameObjects.begin(), gameObjects.end(),
                       [](std::shared_ptr<MovingObject> o)
    {
        return o->isDead();
    }),
    gameObjects.end());
    //update all other game Objects
    for (std::vector<std::shared_ptr<MovingObject>>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        (*it)->update();
    }
}

void Game::render(float interpolation)
{
    player.update_interp(interpolation);

    SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gameRenderer);
    tilemanager.render();
    player.render();

    //render all other game Objects
    for (std::vector<std::shared_ptr<MovingObject>>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
    {
        (*it)->update_interp(interpolation);
        (*it)->render();
    }

    SDL_RenderPresent(gameRenderer);
}

void sdlError(std::string custom)
{
    std::cout << custom << std::endl;
    std::cout << SDL_GetError() << std::endl;
    exit(-1);
}
