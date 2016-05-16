#ifndef __SCENE_NODE_HH__
#define __SCENE_NODE_HH__

#include<memory>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"light.hh"
#include"graphicssystem.hh"

template<typename SYSTEM>
class scenenode{
public:
  typedef typename SYSTEM::scenenode scenenode;
  typedef typename SYSTEM::entity    entity;
  typedef typename SYSTEM::light     light;

private:
  std::shared_ptr<scenenode> mParent;
  glm::mat4 mTransform;

public:
  std::shared_ptr<scenenode> createChild();
  void translate(double x, double y, double z);
  void rotate(double x, double y, double z);
  void scale(double x, double y, double z);
  
  virtual void attachEntity(entity* pEnt, GLuint shader);
  virtual void attachLight(light* pLight, GLuint shader);
  glm::mat4 getTransform();
};

#endif

