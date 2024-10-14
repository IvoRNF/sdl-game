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



//#define STB_IMAGE_IMPLEMENTATION
//#include "stb/stb_image.h"
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
    SDL_Window *window;
    Uint32 ticksCount;
    SDL_GLContext context;
    bool running;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    float rotationAngle;
    float yaw; 
    float pitch;
    Model * ourModel;
    Shader * ourShader;
    void yawAndPitchMouse();

    float mouseChangeX;
    float mouseChangeY;

    int lastMouseX;
    int lastMouseY;

    bool firstMouse;
};
#endif