#include"entity.hh"
#include"scenenode.hh"

scenenode::scenenode(scenenode* pNode = nullptr):
  mParent(pNode){
}

std::shared_ptr<scenenode> scenenode::createChild(){
  return std::shared_ptr<scenenode>(new scenenode(this));
}

void scenenode::setPosition(glm::vec3 pPosition){
  mTransform = glm::translate(mTransform, pPosition - mPosition);
}

void scenenode::translate(double x, double y, double z){
  mTransform = glm::translate(mTransform, glm::vec3(x, y, z));
}

void scenenode::rotate(double x, double y, double z){
  mTransform = glm::rotate(mTransform, glm::vec3(x, y, z));
}

void scenenode::scale(double x, double y, double z){
  mTransform = glm::scale(mTransform, glm::vec3(x, y, z));
}

void scenenode::attachObject(entity* pEnt, GLuint shader){
  if(mParent){
    mParent->attachObject(pEnt, shader);
  }
  if(pEnt){
    pEnt->attach(this);
  }
}

void scenenode::attachPointLight(light* pLight, GLuint shader){
  if(mParent){
    mParent->attachPointLight(pLight, shader);
  }
  if(pLight){
    pLight->attach(this);
  }
}

void scenenode::attachSpotLight(spotLight* pLight, GLuint shader){
  if(mParent){
    mParent->attachSpotLight(pLight, shader);
  }
  if(pLight){
    pLight->attach(this);
  }
}

void scenenode::attachAmbientLight(glm::vec3 ambientColor, GLuint shader){
  if(mParent){
    mParent->attachAmbientLight(ambientColor, shader);
  }
}

glm::mat4 scenenode::getTransform(){
  glm::mat4 transform;
  if(mParent){
    transform = mParent->getTransform();
  }
  
  return transform * mTransform;
}

