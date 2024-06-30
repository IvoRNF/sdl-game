
#include <string>
#include <SDL2/SDL.h>
#include "../../common/VertexArray.h"
#include "./Shader.h"
#include "../../common/Math.h"
#include "../../common/Texture.h"
#include <glm/glm.hpp>
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
    VertexArray *vertexArray;
    float vertexBuffer[166];
    unsigned int indexBuffer[18];
    Shader *spriteShader;
    Texture *texture;
    float currentRotation;
    VecAxis currentRotationAxis;
    glm::vec3 cameraDirection;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    float rotationAngle;
   
};