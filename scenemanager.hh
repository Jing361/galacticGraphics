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
public:
  typedef typename SYSTEM::shader shader;

private:
  class rootnode:public scenenode<SYSTEM>{
  private:
    scenemanager& mManager;
  
  public:
    rootnode(scenemanager* pManager):
      mManager(pManager){
    }
    
    void attach(std::shared_ptr<entity> pEnt, shader shade){
      mManager.attachObject(pEnt, shade);
    }
    
    void attach(std::shared_ptr<light> pLight, shader shade){
      mManager.attachLight(pLight, shade);
    }
  };
  
  std::map<shader, std::shared_ptr<entity> > mEntities;
  std::map<shader, std::shared_ptr<light> > mLights;
  rootnode mRoot;
  
  void attachObject(std::shared_ptr<entity> pEnt, shader shade);
  void attachLight(std::shared_ptr<light> pLight, shader shade);
  
  friend rootnode;

public:
  scenemanager();
  
  void render();
  scenenode& getRootNode();
};

#endif

