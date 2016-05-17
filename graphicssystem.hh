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

template<typename ENIGNE>
class GraphicsEntity{
public:
  typedef GraphicsMaterial<ENGINE> material;
  typedef GraphicsMesh<ENGINE>     mesh;

private:
  material mMaterial
  mesh mMesh;
  std::weak_ptr<scenenode> mParent;
};

template<typename ENIGNE>
class GraphicsCamera{
};

template<typename ENIGNE>
class GraphicsShader{
public:
  GraphicsShader(std::string vertexFile, std::string fragmentFile);
  
  void operator()();
};

template<typename ENIGNE>
class GraphicsEntity{
public:
  typedef GraphicsMaterial<ENGINE> material;
  typedef GraphicsMesh<ENGINE> mesh;

private:
  std::shared_ptr<scenenode> mParent;

public:
  void attach(std::shared_ptr<scenenode> parent);
};

template<typename ENGINE>
class GraphicsLight{
private:
  std::shared_ptr<scenenode> mParent;
  glm::vec3 mDiffuseColor;
  glm::vec3 mSpecularColor;
  float mConstant;
  float mLinear;
  float mQuadratic;

public:
  void attach(std::shared_ptr<scenenode> parent);
};

template<typename ENIGNE>
class GraphicsTraits{
public:
  typedef GraphicsShader<ENIGNE>   shader;
  typedef GraphicsEntity<ENIGNE>   entity;
  typedef GraphicsCamera<ENIGNE>   camera;
  typedef GraphicsLight<ENGINE>    light;
  typedef GraphicsMesh<ENIGNE>     mesh;
  typedef GraphicsMaterial<ENIGNE> material;
  typedef scenemanager<ENGINE>     scenemanager;
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
  typedef ResourceManager<mesh> meshManager;
  typedef ResourceManager<material> materialManager;
  typedef TRAITS::scenemanager scenemanager;

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

