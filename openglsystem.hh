#ifndef __OPENGLSYSTEM_HH__
#define __OPENGLSYSTEM_HH__

#include<gl3w.h>
#include<string>
#include<fstream>
#include<exception>
#include"graphicssystem.hh"
#include"resourcemanager.hh"

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
  unsigned int mNVert;
  bool mHasNormal;
  bool mHasColor;
};

template<>
class GraphicsMaterial<OpenGL>{
public:
  GLuint mDiffuseMap;
  GLuint mSpecularMap;
  float mShininess;
};

template<typename REF = std::string>
void resourcemanager<GraphicsMesh<OpenGL> >::acquire(const REF& name, const std::string& fileName){
  type mesh;
  std::vector<GLfloat> verts;
  
  size_t dot = fileName.rfind('.');
  std::string type(fileName.substr(dot));
  if(type == "obj"){
    verts = fileLoader::objLoader(fileName);
  } else if(type == "flat"){
    verts = fileLoader::flatLoader(fileName);
  }
  mesh.mNVert = verts.size();
  
  glGenBuffers(1, &mesh.mVbo);
  
  glBindBuffer(GL_ARRAY_BUFFER, mesh.mVbo);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  mResources[name] = mesh;
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

template<typename REF = std::string>
void resourcemanager<GraphicsMaterial<OpenGL> >::acquire(const REF& name, const std::string& fileName){
  type material;

  size_t dot = fileName.rfind('.');
  std::string title(fileName.substr(0, dot));
  std::string type(fileName.substr(dot));
  std::string diffName = title + "_diffuse." + type;
  std::string specName = title + "_specular." + type;
  
  if(checkFile(diffName)){
    material.m_diffMap = acquireTexture(diffName);
  } else {
    //require at least diffuse map
    throw invalideFileNameException("Invalid file name.  Valid diffuse map required.");
  }
  //specular map not required
  if(checkFile(specName)){
    material.m_specMap = acquireTexture(specName);
  } else {
    material.m_specMap = -1;
  }

  mResources[name] = material;
}

template<>
scenemanager& OpenGLSystem::getSceneManager(const std::string& name){
  return mScenes[name];
}

template<>
ResourceManager<mesh>& OpenGLSystem::getResourceManager(){
  return mMeshes;
}

template<>
ResourceManager<material>& OpenGLSystem::getResourceManager(){
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
  typedef GraphicsMesh<OpenGL>     mesh;

private:
  GLuint mVao;
  material mMaterial;
  mesh mMesh;
  std::weak_ptr<scenenode> mParent;

public:
  GraphicsEntity(mesh mes, material mat);

  void attach(std::shared_ptr<scenenode> parent);
  void render(GraphicsShader<OpenGL> shader);
};

template<>
GraphicsEntity<OpenGL>::GraphicsEntity(mesh mes, material mat):
  mMaterial(mat),
  mMesh(mes){
}

template<>
void GraphicsEntity<OpenGL>::attach(std::shared_ptr<scenenode> parent){
  mParent = parent;
}

template<>
void GraphicsEntity<OpenGL>::render(GraphicsShader<OpenGL> shader){
}

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
  
  glAttachShader(mProgram, shade);
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

  glLinkProgram(mProgram);
  glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
  if(!success){
    GLchar infoLog[512];
    glGetProgramInfoLog(mProgram, 512, NULL, infoLog);
    std::cerr << "ERROR:SHADER:PROGRAM:LINKING_FAILED\n" << infoLog << std::endl;
  }
  
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);
}

template<>
void GraphicsShader<OpenGL>::operator()(){
  glUseProgram(mProgram);
}

template<>
class GraphicsLight<OpenGL>{
private:
  std::weak_ptr<scenenode> mParent;
  glm::vec3 mDiffuseColor;
  glm::vec3 mSpecularColor;
  float mConstant;
  float mLinear;
  float mQuadratic;

public:
  void attach(std::shared_ptr<scenenode> parent);
};

#endif

