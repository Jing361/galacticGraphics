#include<sstream>
#include<string>
#include<fstream>
#include"fileLoader.hh"

fileLoader::triple fileLoader::extractData(std::string line, bool check){
  triple ret;
  GLfloat value;
  std::string token;
  std::stringstream ss(line);
  //dump first value
  ss >> token;
  
  ss >> value;
  std::get<0>(ret) = value;
  ss >> value;
  std::get<1>(ret) = value;
  if(check){
    ss >> value;
    std::get<2>(ret) = value;
  }
  
  return ret;
}

// file format described here
// http://paulbourke.net/dataformats/obj/
std::vector<GLfloat> fileLoader::objLoader(std::string fileName){
  std::ifstream file(fileName);
  std::string line;
  std::vector<GLfloat> object;
  std::vector<triple> vertices;
  std::vector<std::tuple<GLfloat, GLfloat>> texCoords;
  std::vector<triple> normals;
  std::vector<triple> color;
  std::vector<std::string> faces;
  
  while(std::getline(file, line)){
    if(line[0] != '#'){
      if(line[0] == 'v'){
        if(line[1] == 'n'){
          normals.push_back(extractData(line));
        } else if(line[1] == 't'){
          triple vals = extractData(line, false);
          std::tuple<GLfloat, GLfloat> actual = std::make_tuple(std::get<0>(vals), std::get<1>(vals));
          texCoords.push_back(actual);
        } else {
          vertices.push_back(extractData(line));
        }
      } else if(line[0] == 'f'){
        faces.push_back(line);
      }
    }
  }
  
  for(auto it = faces.begin(); it != faces.end(); ++it){
    std::stringstream ss(*it);
    std::string token;
    //dump first value; 'f'
    ss >> token;
    
    while(ss >> token){
      std::string value;
      std::stringstream iss(token);
      std::getline(iss, value, '/');
      std::stringstream jss(value);
      int idx;
      jss >> idx;
      --idx;
      
      //position
      {
        auto it = vertices[idx];
        object.push_back(std::get<0>(it));
        object.push_back(std::get<1>(it));
        object.push_back(std::get<2>(it));
      }
      //color
      if(color.size() > 0){
        auto it = color[idx];
        object.push_back(std::get<0>(it));
        object.push_back(std::get<1>(it));
        object.push_back(std::get<2>(it));
      }
      //texture
      if(texCoords.size() > 0){
        auto it = texCoords[idx];
        object.push_back(std::get<0>(it));
        object.push_back(std::get<1>(it));
      }
      //normal
      if(normals.size() > 0){
        auto it = normals[idx];
        object.push_back(std::get<0>(it));
        object.push_back(std::get<1>(it));
        object.push_back(std::get<2>(it));
      }
    }
  }
  return object;
}

// should augment file format with attributes
// attributes could use 'attr' identifier
// use special attributes to identify whether
// normals are included or whether color is included
// or infer this information from line length
std::vector<GLfloat> fileLoader::flatLoader(std::string fileName){
  std::vector<GLfloat> object;
  std::ifstream file(fileName);
  std::string line;
  
  while(file >> line){
    std::string token;
    std::stringstream ss(line);
    
    while(std::getline(ss, token, ',')){
      GLfloat value;
      
      std::stringstream iss(token);
      iss >> value;
      object.push_back(value);
    }
  }
  
  return object;
}
