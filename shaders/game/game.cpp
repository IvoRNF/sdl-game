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
#include <glm/gtx/quaternion.hpp>
#include "memory"
#include "rapidjson/document.h"
#include <fstream>
#include <vector>


using namespace std;
using namespace rapidjson;
const int Width = 1024;
const int Height = 700;
/**/
/* POS(3) - TEXTCORD(2) - COLOR(4)*/
const float initY = 10.0f;


Game::Game()
    : window(nullptr), context(nullptr), ticksCount(0), running(true), vertexArray(nullptr),
      texture(nullptr),
      vertexBuffer{ std::vector<float>()},
      indexBuffer{ std::vector<int>() },
      currentRotation(0.10f),
      cameraPos{glm::vec3(0.0f, 0.0f, -initY)},
      cameraFront{glm::vec3(0.0f, 0.0f, initY)},
      cameraUp{glm::vec3(0.0f, 1.0f, 0.0f)},
      rotationAngle(35.0f),
      yaw{90.0f},
      pitch{0.0f}
{
  cameraRight = glm::cross(cameraFront, cameraUp);
 // this->loadPyramidAsset();
  this->loadAirplane();
}

void Game::loadAirplane(){
  string path = "./assets/container/12281_Container_v2_L2.obj";
  this->model.loadModel(path);
  cout << "vertex buffer size" << this->model.vertex.size() << endl;
  cout << " texture cord. size " << this->model.textCord.size() << endl;
  cout << "textures size" << this->model.textures.size() << endl;
  cout << "index buffer size" <<this->model.indexs.size() << endl;
  this->vertexCount = this->model.vertex.size();
  this->uniqueVertexCount = this->vertexCount;
  this->columnsCount = 9;
  auto indexSz = this->model.indexs.size();
  this->indexBuffer.reserve(indexSz);
  for(size_t i = 0; i < indexSz ; i++){
    this->indexBuffer[i] = this->model.indexs[i];
  }
  this->vertexBuffer.reserve(this->vertexCount * this->columnsCount);
  int z = 0;
  for(int i = 0; i < this->vertexCount ; i++){
    auto textCord = this->model.textCord[i];
    auto vert = this->model.vertex[i];
     //cout << "x-cord:" << textCord.x << "y-cord" << textCord.y << endl;
    vector<float> values{ vert.x , vert.y, vert.z ,textCord.x,textCord.y ,0.0f,200.0f,0.0f,0.0f};  
    for(auto value : values){
      this->vertexBuffer[z] = value;
      //cout << value << " ";
      z++;
    }
    //cout << endl;
  }    
}

void Game::Draw() 
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    auto textures = this->model.textures;
    cout << this->model.textures.size() << endl;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        auto shader = *this->spriteShader;
        //cout << "binded texture..." <<("material." + name + number) <<endl;
         shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
    // draw mesh
    /*glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);*/
}  

void Game::loadPyramidAsset(){
  string fname = "./assets/pyramid.json";
  std::ifstream fileLoaded(fname, std::ios::in | std::ios::binary | std::ios::ate);
  if(!fileLoaded.is_open()){
     cerr << printf("file %s not found ",fname.c_str()) << endl;
     return;
  }
  std::ifstream::pos_type fileSz = fileLoaded.tellg();
  fileLoaded.seekg(0,std::ios::beg);
  std::vector<char> bytes(static_cast<size_t>(fileSz) + 1);
  fileLoaded.read(bytes.data(),static_cast<size_t>(fileSz));
  Document doc;
  doc.Parse(bytes.data());
  if(!doc.IsArray()){
    cerr << printf("%s is not an array",fname.c_str()) << endl;
  }
  auto arr = doc.GetArray();
  if(arr.Size() == 0){
    return;
  }
  
  this->vertexCount = arr.Size();
  this->uniqueVertexCount = this->vertexCount;
  this->columnsCount =  arr[0].GetArray().Size();
 
  this->indexBuffer.reserve(this->vertexCount);
  for(size_t i = 0; i < this->vertexCount ; i++){
    this->indexBuffer[i] = i;
  }
  this->vertexBuffer.reserve(this->vertexCount * this->columnsCount);

  for(size_t i = 0 ; i < arr.Size(); i++){
    auto value  = arr[i].GetArray();
    for(size_t j = 0 ; j < value.Size() ; j++){
      auto num = value[j].GetFloat();
      this->vertexBuffer[i * value.Size() + j] = num;
    }
  }
  cout<< "loaded sucess json"  << endl;
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

  this->window = SDL_CreateWindow("OPENGL Game programming C++ ", 1900, 0, Width, Height, SDL_WINDOW_OPENGL);
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
  this->vertexArray =  std::unique_ptr<VertexArray>(new VertexArray( this->vertexBuffer.data(), uniqueVertexCount, indexBuffer.data(), this->vertexCount, this->columnsCount));

  this->spriteShader = std::unique_ptr<Shader>(new Shader());
  if (!this->spriteShader->Load("./shaders/Transform.vert", "./shaders/Basic.frag"))
  {
    SDL_Log("Cannot load shaders files.");
    return false;
  }

 //this->texture = std::unique_ptr<Texture>(new Texture());

 //this->texture->Load("./assets/container/12281_Container_diffuse.png");//"./assets/wall.png");

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
}

void Game::yawAndPitchMouse()
{
  int mouseX = 0;
  int mouseY = 0;
  SDL_GetRelativeMouseState(&mouseX, &mouseY);
  if (this->firstMouse)
  {
    this->lastMouseX = mouseX;
    this->lastMouseY = mouseY;
    this->firstMouse = false;
  }
  float sensitivity = 0.03f;
  this->mouseChangeX = (mouseX - this->lastMouseX) * sensitivity;
  this->mouseChangeY = (mouseY - this->lastMouseY) * sensitivity;

  this->pitch += -this->mouseChangeY;
  this->yaw += this->mouseChangeX;

  if (this->pitch > 89.0f)
  {
    this->pitch = 89.0f;
  }
  if (this->pitch < -89.0f)
  {
    this->pitch = -89.0f;
  }
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

  yawAndPitchMouse();
  if (state[SDL_SCANCODE_ESCAPE])
  {
    this->running = false;
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
    cameraPos -= cameraRight * cameraSpeed;
  }
  if (state[SDL_SCANCODE_D])
  {
    cameraPos += cameraRight * cameraSpeed;
  }
  if (state[SDL_SCANCODE_R])
  {
    cameraPos += cameraSpeed * cameraUp;
  }
  if (state[SDL_SCANCODE_F])
  {
    cameraPos -= cameraSpeed * cameraUp;
  }

  if (state[SDL_SCANCODE_T])
  {
    this->yaw += 1.0f;
  }
  if (state[SDL_SCANCODE_G])
  {
    this->yaw -= 1.0f;
  }
  if (state[SDL_SCANCODE_Y])
  {
    this->pitch += 1.0f;
  }
  if (state[SDL_SCANCODE_H])
  {
    this->pitch -= 1.0f;
  }
  if (this->pitch > 89.0f)
  {
    this->pitch = 89.0f;
  }
  if (this->pitch < -89.0f)
  {
    this->pitch = -89.0f;
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
  glm::quat qX = glm::quat(glm::vec3(glm::radians(92.0f), 0.0f, 0.0f));
 // model = glm::toMat4(qX);
  glm::quat qY = glm::quat(glm::vec3(0.0f, glm::radians(this->rotationAngle), 0.0f));
  model = model * glm::toMat4(qY);
  model = glm::scale(model, glm::vec3(0.01f));

  this->rotationAngle += 1.0f;
  if (this->rotationAngle > 360.0f)
  {
    this->rotationAngle = 0;
  }
  glm::vec3 direction;
  direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  direction.y = sin(glm::radians(this->pitch));
  direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(direction);
  glm::vec3 target = cameraPos + cameraFront;

  auto view = glm::lookAt(cameraPos, target, cameraUp);
   auto t = glm::mat4(1.0f);
  this->spriteShader->SetMatrixUniform("projectionMatrix", glm::value_ptr(projection));
  this->spriteShader->SetMatrixUniform("viewMatrix", glm::value_ptr(view));
 
  this->spriteShader->SetMatrixUniform("transformMatrix", glm::value_ptr( model ));
  this->Draw();
 // glDrawElements(GL_TRIANGLES, this->vertexCount, GL_UNSIGNED_INT, nullptr);
  glEnable(GL_DEPTH_TEST);
  SDL_GL_SwapWindow(this->window);
}
