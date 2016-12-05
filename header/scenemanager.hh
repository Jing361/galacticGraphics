#ifndef __SCENE_MANAGER_HH__
#define __SCENE_MANAGER_HH__

#include<map>
#include<vector>
#include<memory>
#include<exception>
#include"graphicssystem.hh"
#include"scenenode.hh"

template<typename SYSTEM>
class scenemanager{
public:
  typedef typename SYSTEM::shader   shader;
  typedef typename SYSTEM::camera   camera;
  typedef typename SYSTEM::entity   entity;
  typedef typename SYSTEM::light    light;
  typedef typename SYSTEM::location location;

private:
  class rootnode : public scenenode<SYSTEM>{
  private:
    scenemanager& mManager;
  
  public:
    rootnode( scenemanager* pManager ):
      scenenode<SYSTEM>( nullptr ),
      mManager( *pManager ){
    }
    
    void attach( std::shared_ptr<entity> pEnt, shader shade ){
      mManager.attachObject( pEnt, shade );
    }
    
    void attach( std::shared_ptr<light> pLight, shader shade ){
      mManager.attachLight( pLight, shade );
    }
  };
  
  std::multimap<shader, std::shared_ptr<entity> > mEntities;
  std::multimap<shader, std::shared_ptr<light> > mLights;
  std::map<std::string, camera> mCameras;
  std::string mMainCamera;
  rootnode mRoot;
  
  void attachObject( std::shared_ptr<entity> pEnt, shader shade );
  void attachLight( std::shared_ptr<light> pLight, shader shade );
  
  friend rootnode;

public:
  scenemanager();
  
  void render();
  scenenode<SYSTEM>& getRootNode();
  void addCamera( const std::string& name, camera cam );
  void setMainCamera( const std::string& name );
  camera& getCamera( const std::string& name );
};

template<typename SYSTEM>
scenemanager<SYSTEM>::scenemanager():
  mRoot( this ){
}

template<typename SYSTEM>
void scenemanager<SYSTEM>::attachObject( std::shared_ptr<entity> pEnt, shader shade ){
  mEntities.emplace( shade, pEnt );
}

template<typename SYSTEM>
void scenemanager<SYSTEM>::attachLight( std::shared_ptr<light> pLight, shader shade ){
  mLights.emplace( shade, pLight );
}

template<typename SYSTEM>
void scenemanager<SYSTEM>::render(){
  location viewLoc = -1;
  location projLoc = -1;
  location viewPosLoc = -1;
  location nlightsLoc = -1;
  location nspotsLoc = -1;

  shader prog = shader( "", "" );
  for( auto it = mEntities.begin(); it != mEntities.end(); ++it ){
    if( (*it).first != prog ){
      prog = (*it).first;
      prog();
      viewLoc = glGetUniformLocation( prog, "view" );
      projLoc = glGetUniformLocation( prog, "projection" );
      viewPosLoc = glGetUniformLocation( prog, "viewPos" );
      nlightsLoc = glGetUniformLocation( prog, "nLights" );
      nspotsLoc = glGetUniformLocation( prog, "nSpots" );

      auto& cam = mCameras[mMainCamera];
      auto view = cam.getViewMatrix();
      //TODO:set-up zoom, width and height
      auto projection = glm::perspective( 90.0f, 800.0f/600.0f, 0.1f, 100.0f );

      glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
      glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
      glUniform3f( viewPosLoc, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z );
      glUniform1i( nlightsLoc, mLights.size() );

      int i = 0;
      for( auto jt = mLights.lower_bound( prog ); jt != mLights.upper_bound( prog ); ++jt ){
        (*jt).second->getUniforms( prog, std::string( "lights[" ) + std::to_string( i++ ) + "]" );
      }
      i = 0;
    }
    (*it).second->render( prog );
  }
}

template<typename SYSTEM>
scenenode<SYSTEM>& scenemanager<SYSTEM>::getRootNode(){
  return mRoot;
}

template<typename SYSTEM>
void scenemanager<SYSTEM>::addCamera( const std::string& name, camera cam ){
  mCameras[name] = cam;
}

template<typename SYSTEM>
void scenemanager<SYSTEM>::setMainCamera( const std::string& name ){
  mMainCamera = name;
}

template<typename SYSTEM>
typename scenemanager<SYSTEM>::camera& scenemanager<SYSTEM>::getCamera( const std::string& name ){
  // when exception is thrown, let-it-go
  return mCameras.at( name );
}

#endif

