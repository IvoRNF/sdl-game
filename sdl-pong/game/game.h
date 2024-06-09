
#include <string>
#include <SDL2/SDL.h>

struct Vect2
{
    float x;
    float y;
};

class Game
{

public:
    Game();
    bool Init();
    void Loop();
    void ShutDown();

private:
    void ProcessInput();
    void Update();
    void DoOutput();
    Vect2 rotate(Vect2, float);
    bool ballIntersectsPaddle();

    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 ticksCount;
    bool running;
    int paddleDirection;
    Vect2 paddlePosition;
    Vect2 ballPosition;
    Vect2 ballVelocity;
    
};