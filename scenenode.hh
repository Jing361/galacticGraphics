#ifndef __SCENE_NODE_HH__
#define __SCENE_NODE_HH__

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"light.hh"
class entity;

class scenenode{
private:
  scenenode* mParent;
  glm::vec3 mPosition;
  glm::mat4 mTransform;

public:
  scenenode(scenenode* pNode = nullptr);
  
  //can't clean up child? :o
  scenenode* createChild();
  void setPosition(glm::vec3 pPosition);
  void translate(double x, double y, double z);
  
  virtual void attachObject(entity* pEnt, GLuint shader);
  virtual void attachLight(light* pLight, GLuint shader);
  glm::mat4 getTransform();
};

#endif

