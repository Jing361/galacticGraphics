#ifndef __GRAPHICSSYSTEM_HH__
#define __GRAPHICSSYSTEM_HH__

#include<string>
#include<map>
#include<memory>
#include<bitset>
#include"resourcemanager.hh"
#include"scenemanager.hh"

template<class T>
struct Engine{
  static const std::string name;
  static const unsigned int version;
};

template<typename ENIGNE>
class GraphicsMaterial{
};

template<typename ENIGNE>
class GraphicsMesh{
private:
  unsigned int mNVert;
  bool mHasNormal;
  bool mHasColor;
};

template<typename ENGINE>
class GraphicsEntity{
public:
  typedef GraphicsMaterial<ENGINE> material;
  typedef GraphicsMesh<ENGINE>     mesh;

private:
  material mMaterial;
  mesh mMesh;
  std::weak_ptr<scenenode> mParent;

public:
  GraphicsEntity(mesh mes, material mat);

  void attach(std::shared_ptr<scenenode> parent);
  void render(GraphicsShader<ENGINE> shader);
};

template<typename ENIGNE>
class GraphicsCamera{
private:
  std::weak_ptr<scenenode> mParent;
  glm::vec3 mFront;
  glm::vec3 mWorldup;
  glm::vec3 mRight;
  glm::vec3 mUp;

public:
  glm::mat4 getViewMatrix();
};

template<typename ENIGNE>
class GraphicsShader{
public:
  GraphicsShader(std::string vertexFile, std::string fragmentFile);
  
  void operator()();
};

template<typename ENGINE>
class GraphicsLight{
public:
  typedef glm::vec3 color;
  typedef /*something*/ scenenode;

private:
  std::weak_ptr<scenenode> mParent;
  color mDiffuseColor;
  color mSpecularColor;
  float mConstant;
  float mLinear;
  float mQuadratic;

public:
  GraphicsLight(color diff, color spec);
  void attach(std::shared_ptr<scenenode> parent);
};

template<typename ENGINE>
GraphicsLight<ENIGNE>::GraphicsLight(color diff, color spec):
  mDiffuseColor(diff),
  mSpecularColor(spec){
}

template<typename ENGINE>
void GraphicsLight::attach(std::shared_ptr<scenenode> parent){
  mParent = parent;
}

template<typename ENIGNE>
class GraphicsTraits{
public:
  typedef GraphicsShader<ENIGNE>   shader;
  typedef GraphicsEntity<ENIGNE>   entity;
  typedef GraphicsCamera<ENIGNE>   camera;
  typedef GraphicsLight<ENGINE>    light;
  typedef GraphicsMesh<ENIGNE>     mesh;
  typedef GraphicsMaterial<ENIGNE> material;
};

template<typename ENIGNE, class TRAITS = GraphicsTraits<ENIGNE> >
class GraphicsSystem{
public:
  typedef ENIGNE engine;
  typedef typename TRAITS::shader   shader;
  typedef typename TRAITS::entity   entity;
  typedef typename TRAITS::camera   camera;
  typedef typename TRAITS::light    light;
  typedef typename TRAITS::mesh     mesh;
  typedef typename TRAITS::material material;
  typedef ResourceManager<mesh>     meshManager;
  typedef ResourceManager<material> materialManager;
  typedef scenemanager<GraphicsSystem<ENGINE> > scenemanager;

private:
  meshManager mMeshes;
  materialManager mMaterials;
  std::map<std::string, std::shared_ptr<entity> > mEntities;
  std::map<std::string, std::shared_ptr<light> > mLights;
  std::map<std::string, scenemanager> mScenes;
  std::string mMainSceneName;
  std::bitset<1024> mKeys;
  unsigned int mFOV;
  bool mRunning;

public:
  scenemanager& getSceneManager(const std::string& name);
  template<class RESOURCE>
  ResourceManager<RESOURCE>& getResourceManager();
  void setMainScene(const std::string& name);
  bool renderScene(const std::string& name);
  bool renderMainScene();
  bool getRunning();
  void foo();
};

#endif

