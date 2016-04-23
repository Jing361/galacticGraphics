#ifndef __OPENGLSYSTEM_HH__
#define __OPENGLSYSTEM_HH__

#include"graphicssystem.hh"

typedef unsigned int GLuint;

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
void GraphicsShader<OpenGL>::operator()(){
  std::cout << OpenGL::name << " shader!" << std::endl;
}

#endif

