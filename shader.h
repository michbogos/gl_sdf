#pragma once
#include<string>

class Shader
{
private:
public:
    unsigned int vShader;
    unsigned int fShader;
    unsigned int program;
    Shader(const char* vShaderPath, const char* fShaderPath);
    void use();
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    ~Shader();
};
