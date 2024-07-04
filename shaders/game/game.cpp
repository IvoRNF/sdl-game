#include <iostream>
#include <stdio.h>
#include <string>
#include "./game.h"
#include <GL/glew.h>
#include "../../common/VertexArray.h"
#include "./Shader.h"
#include "../../common/Math.h"
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/mat4x4.hpp>                // glm::mat4
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

const int Width = 1024;
const int Height = 700;

const int VertCount = 18;
const int uniqueVertexCount = 18;
const float zz1 = -2.5f;
const float zz2 = -1.0f;

Game::Game()
    : window(nullptr), context(nullptr), ticksCount(0), running(true), vertexArray(nullptr),
      texture(nullptr),
      /*vertexBuffer{
          // first triangle LEFT
          -0.5f,0.5f,-1.0f, 0.0f,0.0f,   1.000f, 0.271f, 0.000f,0.5f,//L
          0.5f,0.5f,-1.0f,  1.0f,0.0f,   1.000f, 0.271f, 0.000f,0.5f,//R
          0.5f,-0.5f,-1.0f,  0.0f,1.0f,  1.000f, 0.271f, 0.000f,0.5f,//B


          -0.5f,-0.5f,-1.0f,  0.2f,0.0f,  1.000f, 0.271f, 0.000f,0.5f,//L

          0.0f,0.0f,0.5f, 0.0f,0.0f,1.000f, 0.271f, 0.000f,0.5f
      },
      indexBuffer{0, 1, 2,    3,2,0,  4,1,2,   0,1,4,  2,3,4,  3,4,0},*/

      /* POS(3) - TEXTCORD(2) - COLOR(4)*/
      //-1.0f = -0.33f
      // 0.5 = 0.15

      vertexBuffer{
          // first triangle LEFT

          -0.5f, -0.5f, zz1, 0.2f, 0.0f, 1.000f, 0.0f, 0.0f, 0.5f,
          0.5f, -0.5f, zz1, 0.0f, 1.0f, 1.000f, 0.0f, 0.0f, 0.5f,
          -0.5f, 0.5f, zz1, 0.0f, 0.0f, 1.000f, 0.0f, 0.0f, 0.5f,

          0.0f, 0.0f, zz2, 0.0f, 0.0f, 0.000f, 1.0f, 0.000f, 0.5f,
          0.5f, 0.5f, zz1, 1.0f, 1.0f, 0.000f, 1.0f, 0.000f, 0.5f,
          0.5f, -0.5f, zz1, 0.0f, 1.0f, 0.000f, 1.0f, 0.000f, 0.5f,

          -0.5f, 0.5f, zz1, 0.0f, 0.0f, 0.000f, 1.0f, 1.0f, 0.5f,
          0.5f, 0.5f, zz1, 1.0f, 0.0f, 0.000f, 1.0f, 1.0f, 0.5f,
          0.0f, 0.0f, zz2, 0.0f, 1.0f, 0.000f, 1.0f, 1.0f, 0.5f,

          0.5f, -0.5f, zz1, 0.0f, 0.0f, 1.00f, 0.0f, 1.00f, 0.5f,
          -0.5f, -0.5f, zz1, 1.0f, 0.0f, 1.00f, 0.0f, 1.00f, 0.5f,
          0.0f, 0.0f, zz2, 0.0f, 1.0f, 1.00f, 0.0f, 1.00f, 0.5f,

          -0.5f, -0.5f, zz1, 0.0f, 0.0f, 1.000f, 0.271f, 0.000f, 0.5f,
          0.0f, 0.0f, zz2, 0.0f, 1.0f, 1.000f, 0.271f, 0.000f, 0.5f,
          -0.5f, 0.5f, zz1, 1.0f, 0.0f, 1.000f, 0.271f, 0.000f, 0.5f

      },

      indexBuffer{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
      currentRotationAxis(None),
      currentRotation(0.10f),
      cameraPos{glm::vec3(0.0f, -3.0f, -4.0f)},
      cameraFront{glm::vec3(0.0f, 0.0f, 4.0f)},
      cameraUp{glm::vec3(0.0f, 1.0f, 0.0f)},
      rotationAngle(35.0f)

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

  this->window = SDL_CreateWindow("OPENGL Game programming C++ 2024", 1900, 0, Width, Height, SDL_WINDOW_OPENGL);
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

  this->vertexArray = new VertexArray(vertexBuffer, uniqueVertexCount, indexBuffer, VertCount);

  this->spriteShader = new Shader();
  if (!this->spriteShader->Load("./shaders/Transform.vert", "./shaders/Basic.frag"))
  {
    SDL_Log("Cannot load shaders files.");
    return false;
  }

  this->texture = new Texture();

  this->texture->Load("./assets/wall.png");

  this->spriteShader->SetActive();

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
  delete this->vertexArray;
  // delete this->spriteShader;
  delete this->texture;
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
  if (state[SDL_SCANCODE_Z])
  {
    this->currentRotationAxis = Z;
  }

  if (state[SDL_SCANCODE_X])
  {
    this->currentRotationAxis = X;
  }

  if (state[SDL_SCANCODE_Y])
  {
    this->currentRotationAxis = Y;
  }

  if (state[SDL_SCANCODE_N])
  {
    this->currentRotationAxis = None;
  }

  const float cameraSpeed = 0.05f; // adjust accordingly
  if (state[SDL_SCANCODE_W])
  {
    cameraPos += cameraSpeed * cameraFront;
  }
  if (state[SDL_SCANCODE_S])
  {
    cameraPos -= cameraSpeed * cameraFront;
  }
  if (state[SDL_SCANCODE_A])
  {
    cameraPos -= glm::cross(cameraFront, cameraUp) * cameraSpeed;
  }
  if (state[SDL_SCANCODE_D])
  {
    cameraPos += glm::cross(cameraFront, cameraUp) * cameraSpeed;
  }
  if (state[SDL_SCANCODE_R])
  {
    cameraPos += cameraSpeed * cameraUp;
    // cout << cameraPos[0] << "=X " << cameraPos[1] << "=Y " << cameraPos[2]<< "=Z" << endl;
  }
  if (state[SDL_SCANCODE_F])
  {
    cameraPos -= cameraSpeed * cameraUp;
    // cout << cameraPos[0] << "=X " << cameraPos[1] << "=Y " << cameraPos[2]<< "=Z" << endl;
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

  this->spriteShader->SetActive();

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);

  auto model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(92.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(this->rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::scale(model, glm::vec3(1.55f));
  this->rotationAngle += 1.0f;
  if (this->rotationAngle > 360.0f)
  {
    this->rotationAngle = 0;
  }

  auto view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  // projection = projection * view;
  this->spriteShader->SetMatrixUniform("projectionMatrix", glm::value_ptr(projection));
  this->spriteShader->SetMatrixUniform("viewMatrix", glm::value_ptr(view));
  this->spriteShader->SetMatrixUniform("transformMatrix", glm::value_ptr(model));

  glDrawElements(GL_TRIANGLES, VertCount, GL_UNSIGNED_INT, nullptr);
  glEnable(GL_DEPTH_TEST);
  SDL_GL_SwapWindow(this->window);
}
