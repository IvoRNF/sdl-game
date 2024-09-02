#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <SOIL/SOIL.h>

using namespace std;

struct ModelTexture{
  unsigned int id;
  string type;
  const char * path;
};

class Model
{
private:
  string directory;

public:
  void loadModel(string const &path)
  {
    Assimp::Importer importer;
    auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
      std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
      return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    std::cout << "load model success " << path << endl;
    this->processNode(scene->mRootNode, scene);
  }

  void processNode(aiNode *node, const aiScene *scene)
  {
    // cout << "process node" << endl;
    for (int i = 0; i < node->mNumMeshes; i++)
    {
      auto mesh = scene->mMeshes[node->mMeshes[i]];
      this->processMesh(mesh, scene);
    }
    for (int i = 0; i < node->mNumChildren; i++)
    {
      this->processNode(node->mChildren[i], scene);
    }
  }

  void printSample(aiMesh *mesh, int i)
  {
    printf("sample vextex %.2f %.2f %.2f \n", mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    auto texture = mesh->mTextureCoords[0];
    if (texture)
    {
      printf("sample texture %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f ",
             texture[i].x,
             texture[i].y,

             mesh->mTangents[i].x,
             mesh->mTangents[i].y,
             mesh->mTangents[i].z,

             mesh->mBitangents[i].x,
             mesh->mBitangents[i].y,
             mesh->mBitangents[i].z);
    }
  }

  // checks all material textures of a given type and loads the textures if they're not loaded yet.
  // the required info is returned as a Texture struct.
  vector<ModelTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
  {
    vector<ModelTexture> textures;
    for (int i = 0; i < mat->GetTextureCount(type); i++)
    {
      aiString str;
      mat->GetTexture(type, i, &str);
      ModelTexture texture;
      texture.id = TextureFromFile(str.C_Str(), this->directory, false);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
    }
    return textures;
  }

  unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
  {
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    // unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    unsigned char *data = SOIL_load_image(filename.c_str(),
                                          &width, &height, &nrComponents, SOIL_LOAD_AUTO);
    if (data)
    {
      GLenum format;
      if (nrComponents == 1)
        format = GL_RED;
      else if (nrComponents == 3)
        format = GL_RGB;
      else if (nrComponents == 4)
        format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      SOIL_free_image_data(data);
    }
    else
    {
      std::cout << "Texture failed to load at path: " << path << std::endl;
      SOIL_free_image_data(data);
    }

    return textureID;
  }
  void processMesh(aiMesh *mesh, const aiScene *scene)
  {

    // cout << "num vertexis " << mesh->mNumVertices << endl;
    vector<ModelTexture> textures;
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
      // printSample(mesh,i);

      // break;
    }

    // process materials
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    /*vector<ModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<ModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    vector<ModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    vector<ModelTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());*/
  }
};