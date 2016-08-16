#ifndef __VULKANSYSTEM_HH__
#define __VULKANSYSTEM_HH__

struct Vulkan:public Engine<Vulkan>{
};
template<>
const std::string Engine<Vulkan>::name = "Vulkan";
template<>
const unsigned int Engine<Vulkan>::version = 1;

typedef GraphicsSystem<Vulkan> VulkanSystem;

template<>
void GraphicsShader<Vulkan>::operator()(){
  std::cout << Vulkan::name << " shader!" << std::endl;
}

template<>
void VulkanSystem::foo(){
  std::cout << VulkanSystem::system::name << std::endl;
}

#endif

