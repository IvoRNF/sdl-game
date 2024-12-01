#include <iostream>
#include <stdio.h>
#include <string>
#include "./game.h"
#include <GL/glew.h>
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "memory"
#include "rapidjson/document.h"
#include <fstream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace std;
using namespace rapidjson;
const int Width = 1024;
const int Height = 700;
/**/
/* POS(3) - TEXTCORD(2) - COLOR(4)*/

Game::Game()
    : window(nullptr), context(nullptr), ticksCount(0), running(true),
      rotationYaw(1.6f),
      rotationPitch(4.8f),
      FPSCamera()
{
}

bool Game::Init()
{

  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if (sdlResult != 0)
  {
    SDL_Log("SDL not intited %s", SDL_GetError());
    return false;
  }
  this->setOpenGLAttributes();

  this->window = SDL_CreateWindow("open gl 3 ", 1900, 0, Width, Height, SDL_WINDOW_OPENGL);
  if (!this->window)
  {
    SDL_Log("Failed to create window %s", SDL_GetError());
    return false;
  }
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  this->context = SDL_GL_CreateContext(this->window);
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    SDL_Log("failed to init glew");
    return false;
  }
  glGetError();

  // stbi_set_flip_vertically_on_load(true);

  // build and compile shaders
  // -------------------------
  ourShader = new Shader("./shaders/1.model_loading.vs", "./shaders/1.model_loading.fs");
  // load models
  // -----------

  string model_name = "./assets/airplane/11665_Airplane_v1_l3.obj";
  ourModel = new Model();
   ourModel->loadModel(FileSystem::getPath(model_name));
  auto cubeFname = "/home/ivo/Documents/c++/load-model/assets/cube/pyramid.json";
  auto cubeTextureFname = "/home/ivo/Documents/c++/load-model/assets/cube/wall.png";
  cubeModel = new Model();
  cubeModel->loadModelFromJSON(cubeFname, cubeTextureFname);

  return true;
}

void Game::setOpenGLAttributes()
{
  // Set OpenGL attributes
  // Use the core OpenGL profile
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // Specify version 3.3
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  // Request a color buffer with 8-bits per RGBA channel
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  // Enable double buffering
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  // Force OpenGL to use hardware acceleration
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
}

void Game::Loop()
{
  while (this->running)
  {
    this->ProcessInput();
    this->Update();
    this->DoOutput();
  }
}
void Game::ShutDown()
{
  SDL_GL_DeleteContext(this->context);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

void Game::ProcessInput()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      this->running = false;
      break;
    }
  }
  auto state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_ESCAPE])
  {
    this->running = false;
  }

  this->processInput(state);

  auto step = 0.1;
  if (state[SDL_SCANCODE_UP])
  {
    this->rotationPitch += step;
  }
  if (state[SDL_SCANCODE_DOWN])
  {
    this->rotationPitch -= step;
  }

  if (state[SDL_SCANCODE_LEFT])
  {
    this->rotationYaw += step;
  }
  if (state[SDL_SCANCODE_RIGHT])
  {
    this->rotationYaw -= step;
  }
}

void Game::Update()
{
  // Wait until 16ms has elapsed since last frame
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->ticksCount + 60))
    ;
  // Delta time is the difference in ticks from last frame
  // (converted to seconds)
  float deltaTime = (SDL_GetTicks() - this->ticksCount) / 1000.0f;
  // Clamp maximum delta time value
  if (deltaTime > 0.05f)
  {
    deltaTime = 0.05f;
  }
  // Update tick counts (for next frame)
  this->ticksCount = SDL_GetTicks();
}

void Game::DoOutput()
{
  glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ourShader->use();

  // view/projection transformations
  auto Zoom = 45.0f;
  glm::mat4 projection = glm::perspective(glm::radians(Zoom), (float)Width / (float)Height, 0.1f, 100.0f);

  glm::mat4 view = this->getViewMatrix();
  ourShader->setMat4("projection", projection);
  ourShader->setMat4("view", view);

  // render the loaded model
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
  // model = glm::rotate(model, 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, this->rotationYaw, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, this->rotationPitch, glm::vec3(1.0f, 0.0f, 0.0f));
  // model = glm::rotate(model, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
  // model = glm::rotate(model, -360.0f, glm::vec3(0.0f, 0.0f, 1.0f));
  auto scale = 0.4f;
  model = glm::scale(model, glm::vec3(scale)); // it's a bit too big for our scene, so scale it down
  ourShader->setMat4("model", model);
  ourModel->Draw(*ourShader);
  this->cubeModel->Draw(*ourShader);

  glEnable(GL_DEPTH_TEST);
  SDL_GL_SwapWindow(this->window);
}
