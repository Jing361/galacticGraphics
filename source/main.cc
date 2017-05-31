#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

using namespace std;

void framebuffer_size_callback( GLFWwindow* window, int width, int height ){
  glViewport( 0, 0, width, height );
}

void processInput( GLFWwindow *window ){
  if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ){
    glfwSetWindowShouldClose( window, true );
  }
}

int main(){
  glfwInit();
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  GLFWwindow* window = glfwCreateWindow( 1024, 768, "Test", nullptr, nullptr );

  if( window == nullptr ){
    cout << "glfw failure!" << endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent( window );
  glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );

  if( !gladLoadGLLoader( GLADloadproc( glfwGetProcAddress ) ) ){
    cout << "GLAD failure" << endl;
    return -2;
  }

  glViewport( 0, 0, 1024, 768 );

  while( !glfwWindowShouldClose( window ) ){
    processInput( window );

    glClear( GL_COLOR_BUFFER_BIT );
    glfwSwapBuffers( window );

    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

