#include<fstream>
#include<SOIL.h>
#include<exception>
#include"resourcemanager.hh"

bool resourceManager::checkFile(const std::string& fileName){
  std::ifstream file(fileName);
  bool ret = file;
  
  file.close();
  
  return ret;
}

template<typename RESOURCE, typename REF = std::string>
RESOURCE& resourcemanager<RESOURCE, REF>::getResource(const REF& name){
  try{
    return mResources.at(name);
  } catch(std::exception& e) {
    throw invalidResourceException("Invalid resource" + name);
  }
}

