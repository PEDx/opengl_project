//
//  shader.hpp
//  opengl-project
//
//  Created by ped on 2018/6/7.
//  Copyright © 2018年 ped. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include "include/glad.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
  unsigned int ID;
  Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
  void userShader();
  // uniform工具函数
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec3(const std::string &name, const glm::vec3 &vec) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w);
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
  void checkShaderCompileErrors(unsigned int shader, std::string type);
};
#endif /* shader_hpp */
