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
    glm::mat4 createModelFromCamera();
    void Update();
    void DoOutput();
    void setOpenGLAttributes();
    SDL_Window *window;
    Uint32 ticksCount;
    SDL_GLContext context;
    bool running;
    glm::mat4 modelCubeMatrix;
    Model * ourModel;
    Model * cubeModel;
    Shader * ourShader;
    Shader * cubeShader;

    
    float rotationYaw;
    float rotationPitch;
};
#endif