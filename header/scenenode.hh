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
  typedef typename SYSTEM::entity entity;
  typedef typename SYSTEM::light  light;
  typedef typename SYSTEM::shader shader;

private:
  std::shared_ptr<scenenode> mParent;
  glm::mat4 mTransform;

public:
  scenenode( scenenode* parent );

  std::shared_ptr<scenenode> createChild();
  void translate( double x, double y, double z );
  void rotate( float r, double x, double y, double z );
  void scale( double x, double y, double z );

  template<class T>
  void attach( std::shared_ptr<T> ptr, shader shade );

  glm::mat4 getTransform();
};

//TODO: if this pointer is on the stack, shared_ptr won't handle this well, probably causing a segfault
template<typename SYSTEM>
scenenode<SYSTEM>::scenenode( scenenode* parent ):
  mParent( parent ){
}

template<typename SYSTEM> 
std::shared_ptr<scenenode<SYSTEM> > scenenode<SYSTEM>::createChild(){
  return std::make_shared<scenenode<SYSTEM> >( this );
}

template<typename SYSTEM>
void scenenode<SYSTEM>::translate( double x, double y, double z ){
  mTransform = glm::translate( mTransform, glm::vec3( x, y, z ) );
}

template<typename SYSTEM>
void scenenode<SYSTEM>::rotate( float r, double x, double y, double z ){
  mTransform = glm::rotate( mTransform, glm::radians( r ), glm::vec3( x, y, z ) );
}

template<typename SYSTEM>
void scenenode<SYSTEM>::scale( double x, double y, double z ){
  mTransform = glm::scale( mTransform, glm::vec3( x, y, z ) );
}

template<typename SYSTEM>
template<class T>
void scenenode<SYSTEM>::attach( std::shared_ptr<T> ptr, shader shade ){
  if( mParent ){
    mParent->attach( ptr, shade );
  }
  if( ptr.get() ){
		ptr->attach( std::make_shared<scenenode<SYSTEM> >( this ) );
  }
}

template<typename SYSTEM>
glm::mat4 scenenode<SYSTEM>::getTransform(){
  glm::mat4 transform;
  if( mParent ){
    transform = mParent->getTransform();
  }
  
  return transform * mTransform;
}

#endif

