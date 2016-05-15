#ifndef __FILE_LOADER_HH__
#define __FILE_LOADER_HH__

#include<gl3w.h>
#include<vector>
#include<tuple>

class fileLoader{
  typedef std::tuple<GLfloat, GLfloat, GLfloat> triple;
  
  static triple extractData(std::string line, bool check = true);
  
public:
  static std::vector<GLfloat> objLoader(std::string fileName);
  static std::vector<GLfloat> flatLoader(std::string fileName);
};

#endif
