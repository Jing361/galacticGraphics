#ifndef __SCENE_NODE_HH__
#define __SCENE_NODE_HH__

#include<memory>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"graphicssystem.hh"

template<typename SYSTEM>
class scenenode{
public:
  typedef typename SYSTEM::entity     entity;
  typedef typename SYSTEM::light      light;
  typedef typename SYSTEM::shader     shader;

private:
  std::shared_ptr<scenenode> mParent;
  glm::mat4 mTransform;

public:
  std::shared_ptr<scenenode> createChild();
  void translate(double x, double y, double z);
  void rotate(double x, double y, double z);
  void scale(double x, double y, double z);
  
  template<class T>
  virtual void attach(std::shared_ptr<T> ptr, shader shade);

  glm::mat4 getTransform();
};

#endif

