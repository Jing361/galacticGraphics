#ifndef __RESOURCEMANAGER_HH__
#define __RESOURCEMANAGER_HH__

#include<string>
#include<map>

template<typename RESOURCE, typename REF = std::string>
class resourcemanager{
public:
  typedef RESOURCE type;

private:
  std::map<REF, RESOURCE> mResources;

public:
  virtual ~resourcemanager();
  //acquire method must be implemented per resource and per engine
  void acquire(const REF& name, const std::string& fileName);
  RESOURCE& getResource(const REF& name);

  static bool checkFile(const std::string& fileName);
};

#include"resourcemanager.cc"

#endif

