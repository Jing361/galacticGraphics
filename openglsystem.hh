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
scenemanager& OpenGLSystem::getManager(){
  return scene;
}

template<>
void OpenGLSystem::foo(){
  std::cout << OpenGLSystem::system::name << std::endl;
}

template<>
class GraphicsShader<OpenGL>{
private:
  GLuint mProgram;

  std::string vLoadShader(const char* fileName);
  void vAttach(GLuint shade, std::basic_string<GLChar> path);

public:
  GraphicsShader(std::string vertexFile, std::string fragmentFile);
  
  void operator()();
};

template<>
class GraphicsEntity<OpenGL>{
public:
  typedef GraphicsMaterial<OpenGL> material;
  typedef GraphicsMesh<OpenGL> mesh;
};

template<>
std::basic_string<GLChar> GraphicsSystem<OpenGL>::vLoadShader(std::string fileName){
  std::basic_string<GLChar> source;
  std::string line;
  std::ifstream file(fileName);
  
  while(std::getline(file, line)){
    source += line + '\n';
  }
  source += '\0';
  return source;
}

template<>
void GraphicsShader<OpenGL>::vAttach(GLuint shade, std::basic_string<GLChar> source){
  GLint success;

  glShaderSource(shade, 1, &source.c_str(), NULL);
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
GraphicsShader<OpenGL>::GraphicsShader(std::string vertexPath,
                                       std::string fragmentPath):
  mProgram(glCreateProgram()){
  GLint success;
  GLuint vertexID =   glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

  attach(vertexID,   vLoadShader(vertexPath));
  attach(fragmentID, vLoadShader(fragmentPath));

  glLinkProgram(m_program);
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if(!success){
    GLchar infoLog[512];
    glGetProgramInfoLog(m_program, 512, NULL, infoLog);
    std::cerr << "ERROR:SHADER:PROGRAM:LINKING_FAILED\n" << infoLog << std::endl;
  }
  
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);
}

template<>
void GraphicsShader<OpenGL>::operator()(){
  glUseProgram(mProgram);
}

#endif

