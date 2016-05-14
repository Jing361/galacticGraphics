#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"openglsystem.hh"

int main(){
  OpenGLSystem gfx;
  OpenGLSystem::scenemanager& scene = gfx.getManager();
  scenenode& scene.getRootNode();
 
  return 0;
}

