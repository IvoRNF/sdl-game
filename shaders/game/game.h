
#include <string>
#include <SDL2/SDL.h>
#include "../../common/VertexArray.h"
#include "./Shader.h"
#include "../../common/Math.h"
#include "../../common/Texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Game
{

public:
    Game();
    bool Init();
    void Loop();
    void ShutDown();
    void loadPyramidAsset();

private:
    void ProcessInput();
    void Update();
    void DoOutput();
    void setOpenGLAttributes();
    SDL_Window *window;
    Uint32 ticksCount;
    SDL_GLContext context;
    bool running;
    std::unique_ptr<VertexArray> vertexArray;
    std::unique_ptr<float> vertexBuffer;
    std::vector< float > *verts;
    unsigned int indexBuffer[15];
    std::unique_ptr<Shader> spriteShader;
    std::unique_ptr<Texture> texture;
    float currentRotation;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    float rotationAngle;
    float yaw; 
    float pitch;

    void yawAndPitchMouse();

    float mouseChangeX;
    float mouseChangeY;

    int lastMouseX;
    int lastMouseY;

    bool firstMouse;
};