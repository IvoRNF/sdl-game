
#include <string>
#include <SDL2/SDL.h>
#include "../../common/VertexArray.h"
#include "./Shader.h"
#include "../../common/Math.h"
#include "../../common/Texture.h"

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
    void setOpenGLAttributes();
    float computeCurrentScale();
    SDL_Window *window;
    Uint32 ticksCount;
    SDL_GLContext context;
    bool running;
    VertexArray *vertexArray;
    float vertexBuffer[166];//[25];
    unsigned int indexBuffer[18];
    Shader *spriteShader;
    Texture *texture;
    float currentScale;
    int scaleDirection;
    float currentRotation;
    VecAxis currentRotationAxis;
    void printV();
};