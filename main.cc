#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<memory>
#include"openglsystem.hh"

int main(){
  typedef OpenGLSystem system;
  typedef typename system::shader   shader;
  typedef typename system::entity   entity;
  typedef typename system::camera   camera;
  typedef typename system::light    light;
  typedef typename system::mesh     mesh;
  typedef typename system::material material;
  typedef system::meshManager       meshManager;
  typedef system::materialManager   materialManager;
  typedef system::scene             scenemanager;

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

