#ifndef __OPENGLSYSTEM_HH__
#define __OPENGLSYSTEM_HH__

#include<gl3w.h>
#include<string>
#include<fstream>
#include<exception>
#include<SOIL.h>
#include"fileloader.hh"
#include"graphicssystem.hh"
#include"resourcemanager.hh"

class invalidFileNameException : public std::exception{
private:
  std::string mText;

public:
  invalidFileNameException(const std::string& filename, const std::string& msg):
    mText("Invalid file name: " + filename + ".  " + msg){
  }

  const char* what() const noexcept{
    return mText.data();
  }
};

class shaderCompilationFailedException : public std::exception{
private:
  std::string mText;

public:
  shaderCompilationFailedException(const std::string& path, const std::string& log):
    mText("ERROR:SHADER:" + path + ":COMPILATION_FAILED\n" + log){
  }

  const char* what() const noexcept{
    return mText.data();
  }
};

class shaderLinkingFailedException : public std::exception{
private:
  std::string mText;

public:
  shaderLinkingFailedException(const std::string& log):
    mText("ERROR:SHADER:PROGRAM:LINKING_FAILED\n" + log){
  }

  const char* what() const noexcept{
    return mText.data();
  }
};

typedef unsigned int GLuint;
typedef int GLint;
typedef char GLchar;

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
class GraphicsMesh<OpenGL> : public resource<GraphicsMesh<OpenGL> >{
public:
  GLuint mVbo;
  unsigned int mNVert;
  bool mHasNormal;
  bool mHasColor;
};

template<>
class GraphicsMaterial<OpenGL> : public resource<GraphicsMaterial<OpenGL> >{
public:
  GLuint mDiffuseMap;
  GLuint mSpecularMap;
  float mShininess;
};

template<>
GraphicsMesh<OpenGL> resource<GraphicsMesh<OpenGL> >::acquire(const std::string& fileName){
  GraphicsMesh<OpenGL> mesh;
  std::vector<GLfloat> verts;
  
  size_t dot = fileName.rfind('.');
  std::string type(fileName.substr(dot));
  if(type == "obj"){
    verts = fileloader::objLoader(fileName);
  } else if(type == "flat"){
    verts = fileloader::flatLoader(fileName);
  }
  mesh.mNVert = verts.size();
  
  glGenBuffers(1, &mesh.mVbo);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh.mVbo);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  return mesh;
}

//this function does not belong here.
//  it should be attached to the material manager
GLint acquireTexture(const std::string& file){
  GLuint tex;
  unsigned char* image;
  int width, height;
  
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  
  glBindTexture(GL_TEXTURE_2D, 0);
  
  return tex;
}

template<>
GraphicsMaterial<OpenGL> resource<GraphicsMaterial<OpenGL> >::acquire(const std::string& fileName){
  GraphicsMaterial<OpenGL> material;

  size_t dot = fileName.rfind('.');
  std::string title(fileName.substr(0, dot));
  std::string type(fileName.substr(dot));
  std::string diffName = title + "_diffuse." + type;
  std::string specName = title + "_specular." + type;
  
  if(resourcemanager<GraphicsMaterial<OpenGL> >::checkFile(diffName)){
    material.mDiffuseMap = acquireTexture(diffName);
  } else {
    //require at least diffuse map
    throw invalidFileNameException(diffName, "Valid diffuse map required.");
  }
  //specular map not required
  if(resourcemanager<GraphicsMaterial<OpenGL> >::checkFile(specName)){
    material.mSpecularMap = acquireTexture(specName);
  } else {
    material.mSpecularMap = -1;
  }

  return material;
}

template<>
scenemanager<OpenGLSystem>& OpenGLSystem::getSceneManager(const std::string& name){
  return mScenes[name];
}

template<>
template<>
resourcemanager<GraphicsMesh<OpenGL> >& OpenGLSystem::getResourceManager(){
  return mMeshes;
}

template<>
template<>
resourcemanager<GraphicsMaterial<OpenGL> >& OpenGLSystem::getResourceManager(){
  return mMaterials;
}

template<>
void OpenGLSystem::setMainScene(const std::string& name){
  mMainSceneName = name;
}

template<>
bool OpenGLSystem::renderScene(const std::string& name){
  bool ret;
  try{
    mScenes.at(name).render();
    ret = true;
  } catch(std::exception& e){
    ret = false;
  }
  return ret;
}

template<>
bool OpenGLSystem::renderMainScene(){
  return renderScene(mMainSceneName);
}

template<>
bool OpenGLSystem::getRunning(){
  return mRunning;
}

template<>
void OpenGLSystem::foo(){
  std::cout << OpenGLSystem::engine::name << std::endl;
}

template<>
class GraphicsShader<OpenGL>{
private:
  GLuint mProgram;

  std::string vLoadShader(const std::string& fileName);
  void vAttach(GLuint shade, const std::basic_string<GLchar>& source, const std::string& path = "");

public:
  GraphicsShader(const std::string& vertexFile, const std::string& fragmentFile);
  virtual ~GraphicsShader();
  
  void operator()();
};

template<>
class GraphicsEntity<OpenGL>{
public:
  typedef GraphicsMaterial<OpenGL> material;
  typedef GraphicsMesh<OpenGL>     mesh;
  typedef GraphicsSystem<OpenGL>   system;
  typedef scenenode<system>        node;

private:
  GLuint mVao;
  material mMaterial;
  mesh mMesh;
  std::weak_ptr<scenenode<system> > mParent;

public:
  GraphicsEntity(mesh mes, material mat);

  void attach(std::shared_ptr<node> parent);
  void render(GraphicsShader<OpenGL> shader);
};

GraphicsEntity<OpenGL>::GraphicsEntity(mesh mes, material mat):
  mMaterial(mat),
  mMesh(mes){
}

void GraphicsEntity<OpenGL>::attach(std::shared_ptr<node> parent){
  mParent = parent;
}

void GraphicsEntity<OpenGL>::render(GraphicsShader<OpenGL> shader){
}

std::basic_string<GLchar> GraphicsShader<OpenGL>::vLoadShader(const std::string& fileName){
  std::basic_string<GLchar> source;
  std::string line;
  std::ifstream file(fileName);
  
  while(std::getline(file, line)){
    source += line + '\n';
  }
  source += '\0';
  return source;
}

void GraphicsShader<OpenGL>::vAttach(GLuint shade, const std::basic_string<GLchar>& source, const std::string& path){
  GLint success;
  const char* srcStr = source.c_str();

  glShaderSource(shade, 1, &srcStr, NULL);
  glCompileShader(shade);
  
  glGetShaderiv(shade, GL_COMPILE_STATUS, &success);
  if(!success){
    GLchar infoLog[512];
    glGetShaderInfoLog(shade, 512, NULL, infoLog);
    throw shaderCompilationFailedException(path, infoLog);
  }
  
  glAttachShader(mProgram, shade);
}

GraphicsShader<OpenGL>::GraphicsShader(const std::string& vertexPath,
                                       const std::string& fragmentPath):
  mProgram(glCreateProgram()){
  GLint success;
  GLuint vertexID =   glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

  vAttach(vertexID,   vLoadShader(vertexPath), vertexPath);
  vAttach(fragmentID, vLoadShader(fragmentPath), fragmentPath);

  glLinkProgram(mProgram);
  glGetProgramiv(mProgram, GL_LINK_STATUS, &success);

  // delete shaders early for exception safety.
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);

  if(!success){
    GLchar infoLog[512];
    glGetProgramInfoLog(mProgram, 512, NULL, infoLog);
    //object creation failed, delete program
    glDeleteProgram(mProgram);
    throw shaderLinkingFailedException(infoLog);
  }
}

GraphicsShader<OpenGL>::~GraphicsShader(){
	//deleting here would allow copied objects to delete the shader...
  //glDeleteProgram(mProgram);
}

void GraphicsShader<OpenGL>::operator()(){
  glUseProgram(mProgram);
}

template<>
class GraphicsLight<OpenGL>{
public:
  typedef GraphicsSystem<OpenGL> system;

private:
  std::weak_ptr<scenenode<system> > mParent;
  glm::vec3 mDiffuseColor;
  glm::vec3 mSpecularColor;
  float mConstant;
  float mLinear;
  float mQuadratic;

public:
  void attach(std::shared_ptr<scenenode<system> > parent);
};

#endif

