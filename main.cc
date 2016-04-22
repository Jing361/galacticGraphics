#include<iostream>
#include<string>
#include<vector>
#include<map>

typedef unsigned int GLuint;

//CRTP class.
template<class T>
struct Engine{
    static const std::string name;
    static const unsigned int version;
};

struct OpenGL3:public Engine<OpenGL3>{
    static const unsigned int minor;
};
template<>
const std::string Engine<OpenGL3>::name = "OpenGL";
template<>
const unsigned int Engine<OpenGL3>::version = 3;
const unsigned int OpenGL3::minor = 2;

typedef OpenGL3 OpenGL;

struct DirectX12:public Engine<DirectX12>{
};
template<>
const std::string Engine<DirectX12>::name = "DirectX";
template<>
const unsigned int Engine<DirectX12>::version = 12;

typedef DirectX12 DirectX;

struct Vulkan:public Engine<Vulkan>{
};
template<>
const std::string Engine<Vulkan>::name = "Vulkan";
template<>
const unsigned int Engine<Vulkan>::version = 1;

template<typename SYSTEM>
class GraphicsShader{
public:
    GraphicsShader(std::string vertexFile, std::string fragmentFile){
        (void)vertexFile;
        (void)fragmentFile;
    }
    
    void operator()();
};

template<>
void GraphicsShader<OpenGL>::operator()(){
    std::cout << OpenGL::name << " shader!" << std::endl;
}

template<>
void GraphicsShader<DirectX>::operator()(){
    std::cout << DirectX::name << " shader!" << std::endl;
}

template<>
void GraphicsShader<Vulkan>::operator()(){
    std::cout << Vulkan::name << " shader!" << std::endl;
}

template<typename SYSTEM>
class GraphicsModel{
};

template<typename SYSTEM>
class GraphicsCamera{
};

template<typename SYSTEM>
class GraphicsMesh{
};

template<>
class GraphicsMesh<OpenGL>{
public:
    GLuint mVbo;
    int mNVert;
};

template<typename SYSTEM>
class GraphicsMaterial{
};

template<>
class GraphicsMaterial<OpenGL>{
public:
    GLuint mDiffuseMap;
    GLuint mSpecularMap;
    float mShininess;
};

template<typename SYSTEM>
class GraphicsTraits{
public:
    typedef GraphicsShader<SYSTEM>     shader;
    typedef GraphicsModel<SYSTEM>      model;
    typedef GraphicsCamera<SYSTEM>     camera;
    typedef GraphicsMesh<SYSTEM>       mesh;
    typedef GraphicsMaterial<SYSTEM>   material;
};

template<typename RESOURCE, typename REF = std::string>
class resourceManager{
private:
    std::map<REF, RESOURCE> mResources;

public:
    void acquire(REF name, std::string fileName);
    RESOURCE getResource(REF name){ return mResources[name]; }
};

template<typename SYSTEM, class TRAITS = GraphicsTraits<SYSTEM> >
class GraphicsSystem{
public:
    typedef SYSTEM system;
    typedef typename TRAITS::shader shader;
    typedef typename TRAITS::model  model;
    typedef typename TRAITS::camera camera;
    typedef resourceManager<typename TRAITS::mesh> meshManager;
    typedef resourceManager<typename TRAITS::material> materialManager;

private:
    meshManager mMeshes;
    materialManager mMaterials;

public:
    void foo();
};
typedef GraphicsSystem<OpenGL> OpenGLSystem;
typedef GraphicsSystem<DirectX> DirectXSystem;
typedef GraphicsSystem<Vulkan> VulkanSystem;

template<>
void OpenGLSystem::foo(){
    std::cout << OpenGLSystem::system::name << std::endl;
}

template<>
void DirectXSystem::foo(){
    std::cout << DirectXSystem::system::name << std::endl;
}

template<>
void VulkanSystem::foo(){
    std::cout << VulkanSystem::system::name << std::endl;
}

int main(){
    //GraphicsSystem<int> gsi;
    OpenGLSystem ogs;
    DirectXSystem dxs;
    VulkanSystem vks;

    OpenGLSystem::shader ogshader("foo", "bar");
    DirectXSystem::shader dxshader("foo", "bar");
    VulkanSystem::shader vkshader("foo", "bar");
    
    //gsi.foo();
    ogs.foo();
    dxs.foo();
    vks.foo();
    
    ogshader();
    dxshader();
    vkshader();
    
    return 0;
}

