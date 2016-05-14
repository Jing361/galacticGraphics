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
};

template<typename ENIGNE>
class GraphicsEntity{
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
};

template<typename ENGINE>
class GraphicsLight{
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
  typedef typename TRAITS::shader shader;
  typedef typename TRAITS::entity entity;
  typedef typename TRAITS::camera camera;
  typedef typename TRAITS::light  light;
  typedef ResourceManager<typename TRAITS::mesh> meshManager;
  typedef ResourceManager<typename TRAITS::material> materialManager;
  typedef TRAITS::scenemanager scenemanager;

private:
  meshManager mMeshes;
  materialManager mMaterials;
  std::map<std::string, std::shared_ptr<entity> > mEntities;
  std::map<std::string, std::shared_ptr<light> > mLights;
  scenemanager scene;
  std::bitset<1024> mKeys;
  unsigned int mFOV;

public:
  scenemanager& getManager();
  void foo();
};

#endif

