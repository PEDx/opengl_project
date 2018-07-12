//
//  model.cpp
//  opengl-project
//
//  Created by fb_001 on 2018/7/4.
//  Copyright © 2018年 ped. All rights reserved.
//

#include "model.hpp"

void Model::Draw(Shader shader)
{
  for (unsigned int i = 0; i < meshes.size(); i++)
    meshes[i].Draw(shader);
}

void Model::loadModel(string path)
{
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
    return;
  }

  directory = path.substr(0, path.find_last_of("./"));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
  vector<Vertex> vertices;
  vector<unsigned int> indices;
  vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;
    glm::vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;
    vertex.Position = vector;

    if (mesh->mNormals)
    {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;
    }
    else
      vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);

    if (mesh->mTextureCoords[0])
    {
      glm::vec2 vector;
      vector.x = mesh->mTextureCoords[0][i].x;
      vector.y = mesh->mTextureCoords[0][i].y;
      vertex.TexCoords = vector;
    }
    else
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);

    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

  vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                     aiTextureType_DIFFUSE, "texture_diffuse");

  textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

  vector<Texture> specularMaps = loadMaterialTextures(material,
                                                      aiTextureType_SPECULAR, "texture_specular");

  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
  vector<Texture> textures;
  int count = mat->GetTextureCount(type);
  for (unsigned int i = 0; i < count; i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    {
      if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
      {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip)
    { // 如果纹理还没有被加载，则加载它
      Texture texture;
      texture.id = TextureFromFile(str.C_Str(), directory, false);
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      textures_loaded.push_back(texture); // 添加到已加载的纹理中
    }
  }
  return textures;
}

unsigned int Model::TextureFromFile(const char *path, const string &directory, bool gamma)
{
  string filename = string(path);
  filename = directory + '/' + filename;

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format = 0;
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

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
