#version 330 core

struct Material{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light{
  vec3 position;

//color factors
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
//attenuation factors
  float constant;
  float linear;
  float quadratic;
};

struct spotLight{
  vec3 position;
  vec3 direction;
  float cutOff;

//color factors
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
//attenuation factors
  float constant;
  float linear;
  float quadratic;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform Light lights[5];
uniform spotLight spots[5];
uniform int nLights;
uniform int nSpots;

uniform Material material;
uniform vec3 viewPos;

void main(){
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  
  vec3 result = vec3(0.0);
  
  for(int i = 0; i < nLights; ++i){
    Light lite = lights[i];
    
    vec3 lightDir = normalize(lite.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    vec3 ambient = lite.ambient * vec3(texture(material.diffuse, TexCoord));
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * (diff * lite.diffuse);
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * lite.specular;
    
    float distance = length(lite.position - FragPos);
    //1 / constant + linear * distance + quadratic * distance^2
    float attenuation = 1.0f / (lite.constant + (lite.linear * distance) + (lite.quadratic * (distance * distance)));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    result += (ambient + diffuse + specular);
  }
  
  for(int i = 0; i < nSpots; ++i){
    spotLight lite = spots[i];
    vec3 lightDir = normalize(lite.position - FragPos);
    float theta = dot(lightDir, normalize(-lite.direction));
    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    
    ambient = lite.ambient * vec3(texture(material.diffuse, TexCoord));
    
    //if angle check fails, use only ambient
    if(theta < lite.cutOff){
      vec3 reflectDir = reflect(-lightDir, norm);
      
      float diff = max(dot(norm, lightDir), 0.0);
      diffuse = vec3(texture(material.diffuse, TexCoord)) * (diff * lite.diffuse);
      
      float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      specular = vec3(texture(material.specular, TexCoord)) * spec * lite.specular;
    }
    
    float distance = length(lite.position - FragPos);
    //1 / constant + linear * distance + quadratic * distance^2
    float attenuation = 1.0f / (lite.constant + (lite.linear * distance) + (lite.quadratic * (distance * distance)));
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    result += (ambient + diffuse + specular);
  }
  
  color = vec4(result, 1.0f);
}

