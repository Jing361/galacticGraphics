#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<memory>
#include"openglsystem.hh"

int main(){
  std::string mainScene = "main";
  OpenGLSystem gfx;
  OpenGLSystem::scenemanager& scene = gfx.getManager(mainScene);
  gfx.setMainScene(mainScene);
  std::shared_ptr<scenenode> entRoot(scene.getRootNode().createChild());
  OpenGLSystem::entity ent;

  while(gfx.getRunning()){
    gfx.renderMainScene();
  }
 
  return 0;
}

