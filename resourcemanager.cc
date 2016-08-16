#include<fstream>
#include<SOIL.h>
#include<exception>
#include"resourcemanager.hh"

template<typename RESOURCE, typename REF = std::string>
bool resourcemanager<RESOURCE, REF>::checkFile(const std::string& fileName){
  return (bool) std::ifstream(fileName);
}

template<typename RESOURCE, typename REF = std::string>
resourcemanager::~resourcemanager(){
}

template<typename RESOURCE, typename REF = std::string>
void resourcemanager<RESOURCE, REF>::acquire(const REF& name, const std::string& fileName){
  mResources[name] = RESOURCE::acquire(fileName);
}

template<typename RESOURCE, typename REF = std::string>
RESOURCE& resourcemanager<RESOURCE, REF>::getResource(const REF& name){
  try{
    return mResources.at(name);
  } catch(std::exception& e) {
    throw invalidResourceException("Invalid resource" + name);
  }
}

