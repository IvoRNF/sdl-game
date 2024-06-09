#include <iostream>
#include <stdio.h>
#include <string>
#include "./game.h"
#include <GL/glew.h>
#include "../../common/VertexArray.h"
#include "./Shader.h"
#include "../../common/Math.h"

using namespace std;

const int Width = 1024;
const int Height = 700;

const int VertCount = 18;
const int uniqueVertexCount = 18;

Game::Game()
    : window(nullptr), context(nullptr), ticksCount(0), running(true), vertexArray(nullptr),
      texture(nullptr),
      /*vertexBuffer{
          // first triangle LEFT
          -0.5f,0.5f,-1.0f, 0.0f,0.0f, //L
          0.5f,0.5f,-1.0f,  1.0f,0.0f, //R
          0.5f,-0.5f,-1.0f,  0.0f,1.0f, //B


          -0.5f,-0.5f,-1.0f,  0.2f,0.0f,//L

          0.0f,0.0f,0.5f, 0.0f,0.0f,
      },
      indexBuffer{0, 1, 2,    3,2,0,  4,1,2,   0,1,4,  2,3,4,  3,4,0},*/

      /* POS(3) - TEXTCORD(2) - COLOR(4)*/
      vertexBuffer{
          // first triangle LEFT
          -0.5f, 0.5f, -1.0f,    0.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f, // L 0
          0.5f, 0.5f, -1.0f,     1.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,  // R
          0.5f, -0.5f, -1.0f,    0.0f, 1.0f,    1.000f, 0.271f, 0.000f,0.5f, // B

          -0.5f, -0.5f, -1.0f,   0.2f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,
          0.5f, -0.5f, -1.0f,    0.0f, 1.0f,    1.000f, 0.271f, 0.000f,0.5f,
          -0.5f, 0.5f, -1.0f,    0.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,

          0.0f, 0.0f, 0.5f,      0.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,
          0.5f, 0.5f, -1.0f,     1.0f, 1.0f,    1.000f, 0.271f, 0.000f,0.5f,
          0.5f, -0.5f, -1.0f,    0.0f, 1.0f,    1.000f, 0.271f, 0.000f,0.5f,

          -0.5f, 0.5f, -1.0f,    0.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,
          0.5f, 0.5f, -1.0f,     1.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,
          0.0f, 0.0f, 0.5f,      0.0f, 1.0f,    1.000f, 0.271f, 0.000f,0.5f,

          0.5f, -0.5f, -1.0f,    0.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,
          -0.5f, -0.5f, -1.0f,   1.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,
          0.0f, 0.0f, 0.5f,      0.0f, 1.0f,    1.000f, 0.271f, 0.000f,0.5f,

          -0.5f, -0.5f, -1.0f,   0.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f,
          0.0f, 0.0f, 0.5f,      0.0f, 1.0f,    1.000f, 0.271f, 0.000f,0.5f,
          -0.5f, 0.5f, -1.0f,    1.0f, 0.0f,    1.000f, 0.271f, 0.000f,0.5f 

          
          },

      indexBuffer{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
      currentScale(0.0f),
      scaleDirection(1),
      rotations{30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360},
      currentRotationIndex(0),
      currentRotationAxis(None)

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
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
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

void Game::printV()
{
  cout << " " << endl;
  int i = 0;
  for (float n : this->vertexBuffer)
  {
    printf("%.2ff,", n);
    if(i % 9 == 0){
      printf("\n");
    }
    i++;
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

  if (state[SDL_SCANCODE_P])
  {
    this->printV();
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

float Game::computeCurrentScale()
{
  this->currentScale += 0.01 * this->scaleDirection;
  if (this->currentScale <= 0 || this->currentScale >= 0.95)
  {
    this->scaleDirection *= -1;
  }
  return this->currentScale;
}

void Game::DoOutput()
{
  //glClearColor(0, 0, 0, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  this->spriteShader->SetActive();
  
  auto rotatedX = Matrix4::RotationMatrix(X, -90);
  auto rotatedY = Matrix4::RotationMatrix(Y, -40);
  
  Matrix4 transformMatrix = rotatedX * rotatedY;
  Matrix4 tranlationM = Matrix4::Identity();
  Matrix4 scaleM = Matrix4::Identity();
  //float scale =  this->computeCurrentScale();
  //Matrix4::Scale(scale, scaleM);
  //transformMatrix *= scaleM;

  auto degrees = this->rotations[this->currentRotationIndex++] * this->scaleDirection;
  if (this->currentRotationIndex == 12)
  {
    this->currentRotationIndex = 0;
  }
  if(this->currentRotationAxis != None ){
    auto rotatedY2 = Matrix4::RotationMatrix(this->currentRotationAxis, degrees);
    transformMatrix *= rotatedY2;
  }

  this->spriteShader->SetMatrixUniform("transformMatrix", transformMatrix.AsFloatPtr());

  glDrawElements(GL_TRIANGLES, VertCount, GL_UNSIGNED_INT, nullptr);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  SDL_GL_SwapWindow(this->window);
  // delete identity;
}
