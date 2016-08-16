
template<typename SYSTEM> 
std::shared_ptr<scenenode<SYSTEM> > scenenode<SYSTEM>::createChild(){
  return std::make_shared<scenenode<SYSTEM> >(this);
}

template<typename SYSTEM>
void scenenode<SYSTEM>::translate(double x, double y, double z){
  mTransform = glm::translate(mTransform, glm::vec3(x, y, z));
}

template<typename SYSTEM>
void scenenode<SYSTEM>::rotate(float r, double x, double y, double z){
  mTransform = glm::rotate(mTransform, glm::radians(r), glm::vec3(x, y, z));
}

template<typename SYSTEM>
void scenenode<SYSTEM>::scale(double x, double y, double z){
  mTransform = glm::scale(mTransform, glm::vec3(x, y, z));
}

template<typename SYSTEM>
template<class T>
void scenenode<SYSTEM>::attach(std::shared_ptr<T> ptr, shader shade){
  if(mParent){
    mParent->attach(ptr, shade);
  }
  if(ptr.get()){
		ptr->attach(std::make_shared<scenenode<SYSTEM> >(this));
  }
}

template<typename SYSTEM>
glm::mat4 scenenode<SYSTEM>::getTransform(){
  glm::mat4 transform;
  if(mParent){
    transform = mParent->getTransform();
  }
  
  return transform * mTransform;
}

