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

template<typename RESOURCE>
class resource{
public:
  static RESOURCE acquire(const std::string& fileName);
};

template<typename ENGINE>
class GraphicsMaterial : public resource<GraphicsMaterial<ENGINE> >{
};

template<typename ENGINE>
class GraphicsMesh : public resource<GraphicsMesh<ENGINE> >{
private:
  unsigned int mNVert;
  bool mHasNormal;
  bool mHasColor;
};

template<typename ENGINE>
class GraphicsShader{
public:
  GraphicsShader(const std::string& vertexFile, const std::string& fragmentFile);
  
  void operator()();
};

template<typename ENGINE>
class GraphicsEntity;

template<typename ENGINE>
class GraphicsCamera;

template<typename ENGINE>
class GraphicsLight;

template<typename ENGINE>
class GraphicsTraits{
public:
  typedef GraphicsShader<ENGINE>   shader;
  typedef GraphicsEntity<ENGINE>   entity;
  typedef GraphicsCamera<ENGINE>   camera;
  typedef GraphicsLight<ENGINE>    light;
  typedef GraphicsMesh<ENGINE>     mesh;
  typedef GraphicsMaterial<ENGINE> material;
};

template<typename ENGINE, class TRAITS = GraphicsTraits<ENGINE> >
class GraphicsSystem{
public:
  typedef ENGINE engine;
  typedef typename TRAITS::shader   shader;
  typedef typename TRAITS::entity   entity;
  typedef typename TRAITS::camera   camera;
  typedef typename TRAITS::light    light;
  typedef typename TRAITS::mesh     mesh;
  typedef typename TRAITS::material material;
  typedef resourcemanager<mesh>     meshManager;
  typedef resourcemanager<material> materialManager;
  typedef scenemanager<GraphicsSystem<ENGINE> > scene;

private:
  meshManager mMeshes;
  materialManager mMaterials;
  std::map<std::string, std::shared_ptr<entity> > mEntities;
  std::map<std::string, std::shared_ptr<light> > mLights;
  std::map<std::string, scene> mScenes;
  std::string mMainSceneName;
  std::bitset<1024> mKeys;
  unsigned int mFOV;
  bool mRunning;

public:
  scene& getSceneManager(const std::string& name);
  template<class RESOURCE>
  resourcemanager<RESOURCE>& getResourceManager();
  void setMainScene(const std::string& name);
  bool renderScene(const std::string& name);
  bool renderMainScene();
  bool getRunning();
  void foo();
};

template<typename ENGINE>
class GraphicsEntity{
public:
  typedef GraphicsMaterial<ENGINE> material;
  typedef GraphicsMesh<ENGINE>     mesh;
	typedef GraphicsShader<ENGINE>   shader;
  typedef GraphicsSystem<ENGINE>   system;
  typedef scenenode<system>        node;

private:
  material mMaterial;
  mesh mMesh;
  std::weak_ptr<node> mParent;

public:
  GraphicsEntity(mesh mes, material mat);

  void attach(std::shared_ptr<node> parent);
  void render(shader shade);
};

template<typename ENGINE>
class GraphicsCamera{
public:
  typedef GraphicsSystem<ENGINE>  system;
  typedef scenenode<system>       node;

private:
  std::weak_ptr<node> mParent;
  glm::vec3 mFront;
  glm::vec3 mWorldup;
  glm::vec3 mRight;
  glm::vec3 mUp;

public:
  glm::mat4 getViewMatrix();
};

template<typename ENGINE>
class GraphicsLight{
public:
  typedef glm::vec3 color;
  typedef GraphicsSystem<ENGINE> system;
  typedef scenenode<system>      node;

private:
  std::weak_ptr<node> mParent;
  color mDiffuseColor;
  color mSpecularColor;
  float mConstant;
  float mLinear;
  float mQuadratic;

public:
  GraphicsLight(color diff, color spec);
  void attach(std::shared_ptr<node> parent);
};

template<typename ENGINE>
GraphicsLight<ENGINE>::GraphicsLight(color diff, color spec):
  mDiffuseColor(diff),
  mSpecularColor(spec){
}

template<typename ENGINE>
void GraphicsLight<ENGINE>::attach(std::shared_ptr<node> parent){
  mParent = parent;
}

#endif

