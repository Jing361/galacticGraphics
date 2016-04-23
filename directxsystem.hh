#ifndef __DIRECTXSYSTEM_HH__
#define __DIRECTXSYSTEM_HH__

struct DirectX12:public Engine<DirectX12>{
};
template<>
const std::string Engine<DirectX12>::name = "DirectX";
template<>
const unsigned int Engine<DirectX12>::version = 12;

typedef DirectX12 DirectX;
typedef GraphicsSystem<DirectX> DirectXSystem;

template<>
void GraphicsShader<DirectX>::operator()(){
  std::cout << DirectX::name << " shader!" << std::endl;
}

template<>
void DirectXSystem::foo(){
  std::cout << DirectXSystem::system::name << std::endl;
}

#endif

