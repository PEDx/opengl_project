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
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

  private:
    void checkShaderCompileErrors(unsigned int shader, std::string type);
};
#endif /* shader_hpp */
