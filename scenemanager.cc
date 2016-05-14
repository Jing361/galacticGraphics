#include"scenemanager.hh"

scenemanager::scenemanager(){
}

void scenemanager::attachObject(std::shared_ptr<entity> pEnt, GLuint shader){
  mEntities.insert(std::pair<GLuint, entity*>(shader, pEnt));
}

void scenemanager::attachLight(std::shared_ptr<light> pLight, GLuint shader){
  mLights.insert(std::pair<GLuint, light*>(shader, pLight));
}

void scenemanager::render(){
  GLint viewLoc = -1;
  GLint projLoc = -1;
  GLint viewPosLoc = -1;
  GLint nlightsLoc = -1;
  GLint nspotsLoc = -1;
  
  GLint prog = -1;
  for(auto it = mEntities.begin(); it != mEntities.end(); ++it){
    if((*it).first != prog){
      prog = (*it).first;
      glUseProgram(prog);
      viewLoc = glGetUniformLocation(prog, "view");
      projLoc = glGetUniformLocation(prog, "projection");
      viewPosLoc = glGetUniformLocation(prog, "viewPos");
      nlightsLoc = glGetUniformLocation(prog, "nLights");
      nspotsLoc = glGetUniformLocation(prog, "nSpots");
      
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
      glUniform3f(viewPosLoc, cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
      glUniform1i(nlightsLoc, mLights.size());
      
      int i = 0;
      for(auto jt = mLights.lower_bound(prog); jt != mLights.upper_bound(prog); ++jt){
        (*jt).second.getUniforms(prog, i++);
      }
      i = 0;
    }
    (*it).second.render(prog);
  }
}

sceneNode& scenemanager::getRootNode(){
  return mRoot;
}

