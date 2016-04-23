#include<iostream>
#include<string>
#include<vector>
#include<map>
#include"openglsystem.hh"

int main(){
  //GraphicsSystem<int> gsi;
  OpenGLSystem ogs;
  //DirectXSystem dxs;
  //VulkanSystem vks;

  OpenGLSystem::shader ogshader("foo", "bar");
  //DirectXSystem::shader dxshader("foo", "bar");
  //VulkanSystem::shader vkshader("foo", "bar");
  
  //gsi.foo();
  ogs.foo();
  //dxs.foo();
  //vks.foo();
  
  ogshader();
  //dxshader();
  //vkshader();
  
  return 0;
}

