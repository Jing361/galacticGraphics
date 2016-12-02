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
  invalidFileNameException( const std::string& filename, const std::string& msg ):
    mText( "Invalid file name: " + filename + ".  " + msg ){
  }

  const char* what() const noexcept{
    return mText.data();
  }
};

class shaderCompilationFailedException : public std::exception{
private:
  std::string mText;

public:
  shaderCompilationFailedException( const std::string& path, const std::string& log ):
    mText( "ERROR:SHADER:" + path + ":COMPILATION_FAILED\n" + log ){
  }

  const char* what() const noexcept{
    return mText.data();
  }
};

class shaderLinkingFailedException : public std::exception{
private:
  std::string mText;

public:
  shaderLinkingFailedException( const std::string& log ):
    mText( "ERROR:SHADER:PROGRAM:LINKING_FAILED\n" + log ){
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
GraphicsMesh<OpenGL> resource<GraphicsMesh<OpenGL> >::acquire( const std::string& fileName ){
  GraphicsMesh<OpenGL> mesh;
  std::vector<GLfloat> verts;
  
  size_t dot = fileName.rfind( '.' );
  std::string type( fileName.substr( dot ) );
  if( type == "obj" ){
    verts = fileloader::objLoader( fileName );
  } else if( type == "flat" ){
    verts = fileloader::flatLoader( fileName );
  }
  mesh.mNVert = verts.size();
  
  glGenBuffers( 1, &mesh.mVbo );
  
  glBindBuffer( GL_ARRAY_BUFFER, mesh.mVbo );
  glBufferData( GL_ARRAY_BUFFER, verts.size() * sizeof( GLfloat ), verts.data(), GL_STATIC_DRAW );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  
  return mesh;
}

//this function does not belong here.
//  it should be attached to the material manager
GLint acquireTexture( const std::string& file ){
  GLuint tex;
  unsigned char* image;
  int width, height;
  
  glGenTextures( 1, &tex );
  glBindTexture( GL_TEXTURE_2D, tex );
  
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  
  image = SOIL_load_image( file.c_str(), &width, &height, 0, SOIL_LOAD_RGB );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
  glGenerateMipmap( GL_TEXTURE_2D );
  SOIL_free_image_data( image );
  
  glBindTexture( GL_TEXTURE_2D, 0 );
  
  return tex;
}

template<>
GraphicsMaterial<OpenGL> resource<GraphicsMaterial<OpenGL> >::acquire( const std::string& fileName ){
  GraphicsMaterial<OpenGL> material;

  size_t dot = fileName.rfind( '.' );
  std::string title( fileName.substr( 0, dot ) );
  std::string type( fileName.substr( dot ) );
  std::string diffName = title + "_diffuse." + type;
  std::string specName = title + "_specular." + type;
  
  if( resourcemanager<GraphicsMaterial<OpenGL> >::checkFile( diffName ) ){
    material.mDiffuseMap = acquireTexture( diffName );
  } else {
    //require at least diffuse map
    throw invalidFileNameException( diffName, "Valid diffuse map required." );
  }
  //specular map not required
  if( resourcemanager<GraphicsMaterial<OpenGL> >::checkFile( specName ) ){
    material.mSpecularMap = acquireTexture( specName );
  } else {
    material.mSpecularMap = -1;
  }

  return material;
}

template<>
scenemanager<OpenGLSystem>& OpenGLSystem::getSceneManager( const std::string& name ){
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
void OpenGLSystem::setMainScene( const std::string& name ){
  mMainSceneName = name;
}

template<>
bool OpenGLSystem::renderScene( const std::string& name ){
  bool ret;
  try{
    mScenes.at( name ).render();
    ret = true;
  } catch( std::exception& e ){
    ret = false;
  }
  return ret;
}

template<>
bool OpenGLSystem::renderMainScene(){
  return renderScene( mMainSceneName );
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

  std::string vLoadShader( const std::string& fileName );
  void vAttach( GLuint shade, const std::basic_string<GLchar>& source, const std::string& path = "" );

public:
  GraphicsShader( const std::string& vertexFile, const std::string& fragmentFile );
  virtual ~GraphicsShader();
  
  void operator()();

  bool operator==( const GraphicsShader& other ) const;
  bool operator!=( const GraphicsShader& other ) const;
  operator int( ) const;
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
  GraphicsEntity( mesh mes, material mat );

  void attach( std::shared_ptr<node> parent );
  void render( GraphicsShader<OpenGL> shader );
};

GraphicsEntity<OpenGL>::GraphicsEntity( mesh mes, material mat ):
  mMaterial( mat ),
  mMesh( mes ){
}

void GraphicsEntity<OpenGL>::attach( std::shared_ptr<node> parent ){
  mParent = parent;
}

void GraphicsEntity<OpenGL>::render( GraphicsShader<OpenGL> shader ){
}

template<>
class GraphicsCamera<OpenGL>{
public:
  typedef GraphicsSystem<OpenGL> system;
  typedef scenenode<system>      node;

private:
  std::weak_ptr<node> mParent;
  glm::vec3 mFront;
  glm::vec3 mWorldup;
  glm::vec3 mRight;
  glm::vec3 mUp;

public:
  glm::mat4 getViewMatrix();
  glm::vec3 getPosition();
};

glm::mat4 GraphicsCamera<OpenGL>::getViewMatrix(){
  return mParent.lock()->getTransform();
}

glm::vec3 GraphicsCamera<OpenGL>::getPosition(){
  auto mat = getViewMatrix();
  return { mat[3][0], mat[3][1], mat[3][2] };
}

std::basic_string<GLchar> GraphicsShader<OpenGL>::vLoadShader( const std::string& fileName ){
  std::basic_string<GLchar> source;
  std::string line;
  std::ifstream file( fileName );
  
  while( std::getline( file, line ) ){
    source += line + '\n';
  }
  source += '\0';
  return source;
}

void GraphicsShader<OpenGL>::vAttach( GLuint shade, const std::basic_string<GLchar>& source, const std::string& path ){
  GLint success;
  const char* srcStr = source.c_str();

  glShaderSource( shade, 1, &srcStr, NULL );
  glCompileShader( shade );
  
  glGetShaderiv( shade, GL_COMPILE_STATUS, &success );
  if( !success ){
    GLchar infoLog[512];
    glGetShaderInfoLog( shade, 512, NULL, infoLog );
    throw shaderCompilationFailedException( path, infoLog );
  }
  
  glAttachShader( mProgram, shade );
}

GraphicsShader<OpenGL>::GraphicsShader( const std::string& vertexPath,
                                        const std::string& fragmentPath ):
  mProgram( glCreateProgram() ){
  GLint success;
  GLuint vertexID =   glCreateShader( GL_VERTEX_SHADER );
  GLuint fragmentID = glCreateShader( GL_FRAGMENT_SHADER );

  vAttach( vertexID,   vLoadShader( vertexPath ), vertexPath );
  vAttach( fragmentID, vLoadShader( fragmentPath ), fragmentPath );

  glLinkProgram( mProgram );
  glGetProgramiv( mProgram, GL_LINK_STATUS, &success );

  // delete shaders early for exception safety.
  glDeleteShader( vertexID );
  glDeleteShader( fragmentID );

  if( !success ){
    GLchar infoLog[512];
    glGetProgramInfoLog( mProgram, 512, NULL, infoLog );
    //object creation failed, delete program
    glDeleteProgram( mProgram );
    throw shaderLinkingFailedException( infoLog );
  }
}

GraphicsShader<OpenGL>::~GraphicsShader(){
	//deleting here would allow copied objects to delete the shader...
  //glDeleteProgram( mProgram );
}

void GraphicsShader<OpenGL>::operator()(){
  glUseProgram( mProgram );
}

bool GraphicsShader<OpenGL>::operator==( const GraphicsShader& other ) const{
  return mProgram == other.mProgram;
}

bool GraphicsShader<OpenGL>::operator!=( const GraphicsShader& other ) const{
  return !( (*this) == other );
}

GraphicsShader<OpenGL>::operator int( ) const{
  return mProgram;
}

template<>
class GraphicsLight<OpenGL>{
public:
  typedef glm::vec3               color;
  typedef GraphicsSystem<OpenGL>  system;
  typedef typename system::shader shader;
  typedef scenenode<system>       node;

private:
  std::weak_ptr<scenenode<system> > mParent;
  color mDiffuseColor;
  color mSpecularColor;
  float mConstant = 1.0;
  float mLinear = 0.0014;
  float mQuadratic = 0.000007;

public:
  GraphicsLight( color diff, color spec );
  void attach( std::shared_ptr<scenenode<system> > parent );
  void getUniforms( shader prog, const std::string& var );
  glm::vec3 getPosition();
};

GraphicsLight<OpenGL>::GraphicsLight( color diff, color spec ):
  mDiffuseColor( diff ),
  mSpecularColor( spec ){
}

void GraphicsLight<OpenGL>::attach( std::shared_ptr<scenenode<system> > parent ){
  mParent = parent;
}

void GraphicsLight<OpenGL>::getUniforms( shader prog, const std::string& var ){
  auto pos = getPosition();

  GLint lightPosLoc       = glGetUniformLocation(prog, (var + ".position").c_str());
  
  GLint lightDiffuseLoc   = glGetUniformLocation(prog, (var + ".diffuse").c_str());
  GLint lightSpecularLoc  = glGetUniformLocation(prog, (var + ".specular").c_str());
  
  GLint lightConstantLoc  = glGetUniformLocation(prog, (var + ".constant").c_str());
  GLint lightLinearLoc    = glGetUniformLocation(prog, (var + ".linear").c_str());
  GLint lightQuadraticLoc = glGetUniformLocation(prog, (var + ".quadratic").c_str());
  
  glUniform3f(lightPosLoc, pos.x, pos.y, pos.z);
  
  glUniform3f(lightDiffuseLoc,  mDiffuseColor.r, mDiffuseColor.g, mDiffuseColor.b);
  glUniform3f(lightSpecularLoc, mSpecularColor.r, mSpecularColor.g, mSpecularColor.b);
  
  glUniform1f(lightConstantLoc,  mConstant);
  glUniform1f(lightLinearLoc,    mLinear);
  glUniform1f(lightQuadraticLoc, mQuadratic);
}

glm::vec3 GraphicsLight<OpenGL>::getPosition(){
  auto mat = mParent.lock()->getTransform();
  return { mat[3][0], mat[3][1], mat[3][2] };
}

#endif

