#ifndef __SCENE_MANAGER_HH__
#define __SCENE_MANAGER_HH__

#include<map>
#include<vector>
#include<memory>
#include"graphicssystem.hh"
#include"sceneNode.hh"
#include"entity.hh"
#include"light.hh"
#include"renderer.hh"

template<typename SYSTEM>
class scenemanager{
private:
  class rootnode:public scenenode{
  private:
    scenemanager& mManager;
  
  public:
    rootnode(scenemanager* pManager):
      mManager(pManager){
    }
    
    void attachObject(entity* pEnt, GLuint shader){
      mManager.attachObject(pEnt, shader);
    }
    
    void attachLight(light* pLight, GLuint shader){
      mManager.attachLight(pLight, shader);
    }
  };
  
  std::map<typename SYSTEM::shader, std::shared_ptr<entity> > mEntities;
  std::map<typename SYSTEM::shader, std::shared_ptr<light> > mLights;
  rootnode mRoot;
  
  void attachObject(std::shared_ptr<entity> pEnt, typename SYSTEM::shader shader);
  void attachLight(std::shared_ptr<light> pLight, typename SYSTEM::shader shader);
  
  friend rootnode;

public:
  scenemanager();
  
  void render();
  scenenode& getRootNode();
};

#endif

