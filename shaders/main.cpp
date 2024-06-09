#include "./game/game.h"

int main(int argc, char** argv)
{
  Game game;
  bool inited = game.Init();
  if (inited)
  {
    game.Loop();
  }
  game.ShutDown();
  return 0;
}
