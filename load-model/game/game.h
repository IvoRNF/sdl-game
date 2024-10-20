#ifndef GAME_H
#define GAME_H
#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "./model.h"
#include "./shader.h"
#include "./shader_m.h"
#include "./filesystem.h"
#include "./camera.h"


class Game : public FPSCamera
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
    SDL_Window *window;
    Uint32 ticksCount;
    SDL_GLContext context;
    bool running;
    
    Model * ourModel;
    Shader * ourShader;

    
    float rotationYaw;
    float rotationPitch;
};
#endif