#ifndef __RESOURCEMANAGER_HH__
#define __RESOURCEMANAGER_HH__

#include<string>
#include<map>

template<typename RESOURCE, typename REF = std::string>
class ResourceManager{
private:
  std::map<REF, RESOURCE> mResources;

public:
  void acquire(REF name, std::string fileName);
  RESOURCE getResource(REF name){ return mResources[name]; }
};

#endif

