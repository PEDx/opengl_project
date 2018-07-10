//
//  model.hpp
//  opengl-project
//
//  Created by fb_001 on 2018/7/4.
//  Copyright © 2018年 ped. All rights reserved.
//

#ifndef MODEL_H
#define MODEL_H

#include <stdio.h>
#include <vector>
#include "shader.hpp"
#include "mesh.hpp"
#include "includes/assimp/Importer.hpp"
#include "includes/assimp/scene.h"
#include "includes/assimp/postprocess.h"
#include "includes/stb_image.h"

using namespace std;

class Model
{
public:
  vector<Mesh> meshes;
  string directory;
  vector<Texture> textures_loaded;
  Model(char *path)
  {
    loadModel(string(path));
  }
  void Draw(Shader shader);

private:
  void loadModel(string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  unsigned int TextureFromFile(char const *path, const string &directory, bool gamma);
  vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       string typeName);
};

#endif /* MODEL_H */
