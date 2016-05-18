#include"entity.hh"
#include"scenenode.hh"

template<typename SYSTEM> 
std::shared_ptr<scenenode> scenenode<SYSTEM>::createChild(){
  return std::shared_ptr<scenenode>(new scenenode<SYSTEM>(this));
}

template<typename SYSTEM>
void scenenode<SYSTEM>::translate(double x, double y, double z){
  mTransform = glm::translate(mTransform, glm::vec3(x, y, z));
}

template<typename SYSTEM>
void scenenode<SYSTEM>::rotate(double x, double y, double z){
  mTransform = glm::rotate(mTransform, glm::vec3(x, y, z));
}

template<typename SYSTEM>
void scenenode<SYSTEM>::scale(double x, double y, double z){
  mTransform = glm::scale(mTransform, glm::vec3(x, y, z));
}

template<class T>
template<typename SYSTEM>
void scenenode<SYSTEM>::attach(std::shared_ptr<T> ptr, shader shade){
  if(mParent){
    mParent->attach(pEnt, shade);
  }
  if(ptr
    ptr->attach(this);
  }
}

template<typename SYSTEM>
glm::mat4 scenenode::getTransform(){
  glm::mat4 transform;
  if(mParent){
    transform = mParent->getTransform();
  }
  
  return transform * mTransform;
}

