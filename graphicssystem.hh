#ifndef __GRAPHICSSYSTEM_HH__
#define __GRAPHICSSYSTEM_HH__

#include"resourcemanager.hh"

template<class T>
struct Engine{
  static const std::string name;
  static const unsigned int version;
};

template<typename SYSTEM>
class GraphicsMaterial{
};

template<typename SYSTEM>
class GraphicsMesh{
};

template<typename SYSTEM>
class GraphicsModel{
};

template<typename SYSTEM>
class GraphicsCamera{
};

template<typename SYSTEM>
class GraphicsShader{
public:
  GraphicsShader(std::string vertexFile, std::string fragmentFile){
    (void)vertexFile;
    (void)fragmentFile;
  }
  
  void operator()();
};

template<typename SYSTEM>
class GraphicsTraits{
public:
  typedef GraphicsShader<SYSTEM>   shader;
  typedef GraphicsModel<SYSTEM>    model;
  typedef GraphicsCamera<SYSTEM>   camera;
  typedef GraphicsMesh<SYSTEM>     mesh;
  typedef GraphicsMaterial<SYSTEM> material;
};

template<typename SYSTEM, class TRAITS = GraphicsTraits<SYSTEM> >
class GraphicsSystem{
public:
  typedef SYSTEM system;
  typedef typename TRAITS::shader shader;
  typedef typename TRAITS::model  model;
  typedef typename TRAITS::camera camera;
  typedef ResourceManager<typename TRAITS::mesh> meshManager;
  typedef ResourceManager<typename TRAITS::material> materialManager;

private:
  meshManager mMeshes;
  materialManager mMaterials;

public:
  void foo();
};

#endif

