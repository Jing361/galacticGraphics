#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<memory>
#include"openglsystem.hh"

int main(){
  typedef OpenGLSystem engine;
  typedef typename OpenGLSystem::shader   shader;
  typedef typename OpenGLSystem::entity   entity;
  typedef typename OpenGLSystem::camera   camera;
  typedef typename OpenGLSystem::light    light;
  typedef typename OpenGLSystem::mesh     mesh;
  typedef typename OpenGLSystem::material material;
  typedef OpenGLSystem::meshManager meshManager;
  typedef OpenGLSystem::materialManager materialManager;
  typedef OpenGLSystem::scenemanager scenemanager;

  OpenGLSystem gfx;
  scenemanager& scene = gfx.getManager(mainScene);
  meshManager& meshes = gfx.getResourceManager<mesh>();
  materialManager& mats = gfx.getResourceManager<material>();

  std::string mainScene = "main";
  shader shade("vertex.glsl", "fragment.glsl");
  std::shared_ptr<scenenode> entRoot(scene.getRootNode().createChild());
  camera cam;

  gfx.setMainScene(mainScene);
  scene.addCamera("main", cam);
  scene.setMainCamera("main");
  meshes.acquire("cube", "data/cubePTN.flat");
  mats.acquire("box", "data/container.jpg");
  entRoot.translate(0, 3, 0);

  auto ent = std::make_shared<entity>(meshes.getResource("cube"), mats.getResource("box"));
  entRoot.attach(ent, shade);

  while(gfx.getRunning()){
    gfx.renderMainScene();
  }
 
  return 0;
}

