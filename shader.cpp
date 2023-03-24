#include<fstream>
#include<sstream>
#include<string>
#include<iostream>
#include<glad/glad.h>


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

Shader::Shader(const char* vShaderPath, const char* fShaderPath){
    std::ifstream vShaderFile(vShaderPath);
    std::ifstream fShaderFile(fShaderPath);
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    vShaderFile.close();
    fShaderFile.close();
    std::string vShaderCode = vShaderStream.str();
    std::string fShaderCode = fShaderStream.str();
    const char* vShaderSource = vShaderCode.c_str();
    const char* fShaderSource = fShaderCode.c_str();

    int success;
    char infoLog[512];

    this->vShader = glCreateShader(GL_VERTEX_SHADER);
    this->fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(this->vShader, 1, &vShaderSource, NULL);
    glShaderSource(this->fShader, 1, &fShaderSource, NULL);

    glCompileShader(this->vShader);
    glGetShaderiv(this->vShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(this->vShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    glCompileShader(this->fShader);
    glGetShaderiv(this->fShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(this->fShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    this->program = glCreateProgram();
    glAttachShader(this->program, this->vShader);
    glAttachShader(this->program, this->fShader);
    glLinkProgram(this->program);
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(this->vShader);
    glDeleteShader(this->fShader);
}

Shader::~Shader(){

}

void Shader::use(){
    glUseProgram(this->program);
}

void Shader::setBool(const std::string &name, bool value){         
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value)
{ 
    glUniform1i(glGetUniformLocation(this->program, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(this->program, name.c_str()), value); 
} 
