#ifndef __OPENGLSYSTEM_HH__
#define __OPENGLSYSTEM_HH__

#include<gl3w.h>
#include<string>
#include<fstream>
#include"graphicssystem.hh"

typedef unsigned int GLuint;
typedef int GLint;

struct OpenGL3:public Engine<OpenGL3>{
  static const unsigned int minor;
};
template<>
const std::string Engine<OpenGL3>::name = "OpenGL";
template<>
const unsigned int Engine<OpenGL3>::version = 3;
const unsigned int OpenGL3::minor = 2;

typedef OpenGL3 OpenGL;
typedef GraphicsSystem<OpenGL> OpenGLSystem;

template<>
class GraphicsMesh<OpenGL>{
public:
  GLuint mVbo;
  int mNVert;
};

template<>
class GraphicsMaterial<OpenGL>{
public:
  GLuint mDiffuseMap;
  GLuint mSpecularMap;
  float mShininess;
};

template<>
void OpenGLSystem::foo(){
  std::cout << OpenGLSystem::system::name << std::endl;
}

template<>
class GraphicsShader<OpenGL>{
private:
  GLuint mProgram;

  std::string vLoadShader(const char* fileName);
  void vAttach(GLuint shade, const GLchar* path);

public:
  GraphicsShader(std::string vertexFile, std::string fragmentFile);
  
  void operator()();
};

template<>
std::string GraphicsSystem<OpenGL>::vLoadShader(const char* fileName){
  std::string source;
  std::string line;
  std::ifstream file(fileName);
  
  while(std::getline(file, line)){
    source += line + '\n';
  }
  source += '\0';
  return source;
}

template<>
void GraphicsShader<OpenGL>::vAttach(GLuint shade, const GLchar* path){
  GLint success;
  std::string source = loadShader(path);
  const GLchar* ss = source.c_str();

  glShaderSource(shade, 1, &ss, NULL);
  glCompileShader(shade);
  
  glGetShaderiv(shade, GL_COMPILE_STATUS, &success);
  if(!success){
    GLchar infoLog[512];
    glGetShaderInfoLog(shade, 512, NULL, infoLog);
    std::cerr << "ERROR:SHADER:" << path << ":COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  
  glAttachShader(m_program, shade);
}

template<>
GraphicsShader<OpenGL>::GraphicsShader(std::string vertexFile, std::string fragmentFile):
  mProgram(glCreateProgram()){
  GLint success;
  GLuint vertexSource = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentSource = glCreateShader(GL_FRAGMENT_SHADER);

  attach(vertexSource, vertexPath);
  attach(fragmentSource, fragmentPath);

  glLinkProgram(m_program);
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if(!success){
    GLchar infoLog[512];
    glGetProgramInfoLog(m_program, 512, NULL, infoLog);
    std::cerr << "ERROR:SHADER:PROGRAM:LINKING_FAILED\n" << infoLog << std::endl;
  }
  
  glDeleteShader(vertexSource);
  glDeleteShader(fragmentSource);
}

template<>
void GraphicsShader<OpenGL>::operator()(){
  glUseProgram(mProgram);
}

#endif

