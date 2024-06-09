#include <iostream>
#include <stdio.h>
#include <string>
#include "./game.h"

using namespace std;

const int thickness = 15;
const float paddleHeight = 100.0f;
const int Width = 1024;
const int Height = 700;
Game::Game()
    : window(nullptr), renderer(nullptr), ticksCount(0), running(true), paddleDirection(0)
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
  this->window = SDL_CreateWindow("SDL Game programming C++ 2024", 1900, 0, Width, Height, 0);
  if (!this->window)
  {
    SDL_Log("Failed to create window %s", SDL_GetError());
    return false;
  }

  this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!this->renderer)
  {
    SDL_Log("Failed to create renderer %s", SDL_GetError());
    return false;
  }
  this->paddlePosition.x = 0;
  this->paddlePosition.y = Height / 2 - (paddleHeight / 2);

  this->ballPosition.x = static_cast<float>(Width - thickness);
  this->ballPosition.y = Height / 2 - (thickness / 2);

  this->ballVelocity.x = -260.0f;
  this->ballVelocity.y = 285.0f;
  return true;
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
  SDL_DestroyRenderer(this->renderer);
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
  else if (state[SDL_SCANCODE_UP])
  {
    this->paddleDirection = -1;
  }
  else if (state[SDL_SCANCODE_DOWN])
  {
    this->paddleDirection = 1;
  }
}

void Game::Update()
{
  // Wait until 16ms has elapsed since last frame
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->ticksCount + 16));
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
  if (this->paddleDirection != 0)
  {
    auto y = deltaTime * this->paddleDirection * 300.0f;
    auto newY = this->paddlePosition.y + y;
    if (newY < 0)
    {
      newY = 0;
      this->paddleDirection = 0;
    }
    if ((newY + paddleHeight) >= Height)
    {
      newY = static_cast<float>(Height - (paddleHeight));
      this->paddleDirection = 0;
    }
    this->paddlePosition.y = newY;
  }
  if(this->ballIntersectsPaddle())
  {
    this->ballVelocity.x *= -1;
  }
   this->ballPosition.x += this->ballVelocity.x * deltaTime;
  this->ballPosition.y += this->ballVelocity.y * deltaTime;
  if (this->ballPosition.x < 0) //ball outside screen on left , start at right
  {
    this->ballPosition.x = static_cast<float>(Width - thickness);
  }
  if (this->ballPosition.y < 0) //ball outside screen on top , invert y direction
  {
    this->ballVelocity.y *= -1;
  }
  if (this->ballPosition.y >= Height) //ball outside screen on bottom , invert y direction
  {
    this->ballVelocity.y *= -1;
  }
  if (this->ballPosition.x >= Width) //ball outside screen on right , invert x direction
  {
    this->ballVelocity.x *= -1;
  }
  
}
bool Game::ballIntersectsPaddle()
{
  return this->ballPosition.x <= (this->paddlePosition.x + thickness) &&
         this->ballPosition.x >= this->paddlePosition.x && 
         (this->ballPosition.y <= (this->paddlePosition.y + paddleHeight) 
         && this->ballPosition.y >= this->paddlePosition.y);
}

void Game::DoOutput()
{

  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
  SDL_RenderClear(this->renderer);
  SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);

  SDL_Rect paddleRect{
      static_cast<int>(this->paddlePosition.x),
      static_cast<int>(this->paddlePosition.y),
      thickness,
      static_cast<int>(paddleHeight),
  };

  SDL_RenderFillRect(this->renderer, &paddleRect);
  SDL_SetRenderDrawColor(this->renderer, 0, 255, 255, 255);
  SDL_Rect ballRect{
      static_cast<int>(this->ballPosition.x),
      static_cast<int>(this->ballPosition.y),
      thickness,
      thickness,
  };

  SDL_RenderFillRect(this->renderer, &ballRect);
  SDL_RenderPresent(this->renderer);
}

Vect2 Game::rotate(Vect2 point, float degree)
{
  auto pi = std::acos(-1);
  const int theta = degree * 180 / pi;
  float c = std::cos(theta);
  float s = std::sin(theta);

  Vect2 result =
      {
          point.x * c - point.y * s,
          point.x * s + point.y * c};

  return result;
}