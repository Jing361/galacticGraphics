#ifndef __RESOURCEMANAGER_HH__
#define __RESOURCEMANAGER_HH__

#include<string>
#include<map>
#include<fstream>
#include<SOIL.h>
#include<exception>

class invalidResourceException : public std::exception{
private:
  std::string mMessage;
  
public:
  invalidResourceException(  const std::string& name  );
  virtual const char* what() const noexcept;
};

invalidResourceException::invalidResourceException(  const std::string& name  ):
  mMessage( "Invalid resource:\t" + name ){
}

const char* invalidResourceException::what() const noexcept{
  return mMessage.c_str();
}

template<typename RESOURCE, typename REF = std::string>
class resourcemanager{
public:
  typedef RESOURCE type;

private:
  std::map<REF, RESOURCE> mResources;

public:
  virtual ~resourcemanager();
  //acquire method must be implemented per resource and per engine
  void acquire( const REF& name, const std::string& fileName );
  RESOURCE& getResource( const REF& name );

  static bool checkFile( const std::string& fileName );
};

template<typename RESOURCE, typename REF>
bool resourcemanager<RESOURCE, REF>::checkFile( const std::string& fileName ){
  return bool( std::ifstream( fileName ) );
}

template<typename RESOURCE, typename REF>
resourcemanager<RESOURCE, REF>::~resourcemanager(){
}

template<typename RESOURCE, typename REF>
void resourcemanager<RESOURCE, REF>::acquire( const REF& name, const std::string& fileName ){
  mResources[name] = RESOURCE::acquire( fileName );
}

template<typename RESOURCE, typename REF>
RESOURCE& resourcemanager<RESOURCE, REF>::getResource( const REF& name ){
  try{
    return mResources.at( name );
  } catch( std::out_of_range& ) {
    throw invalidResourceException( "Invalid resource" + name );
  }
}

#endif

