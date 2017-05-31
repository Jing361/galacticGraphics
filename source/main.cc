#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<exception>
#include<string>

using namespace std;

void framebuffer_size_callback( GLFWwindow* window, int width, int height ){
  glViewport( 0, 0, width, height );
}

void processInput( GLFWwindow *window ){
  if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ){
    glfwSetWindowShouldClose( window, true );
  }
}

class glfw_exception : public exception{
private:
  string mMsg;

public:
  glfw_exception( string message ):
    mMsg( string( "GLFW error: " ) + message ){
  }
};

class glfw_handler{
private:
  GLFWwindow* mWindow;

public:
  glfw_handler( bool isResizable = true ){
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

    mWindow = glfwCreateWindow( 1024, 768, "Test", nullptr, nullptr );

    if( mWindow == nullptr ){
      glfwTerminate();
      throw glfw_exception( "Failed to initialize." );
    }

    glfwMakeContextCurrent( mWindow );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    if( isResizable ){
      glfwSetFramebufferSizeCallback( mWindow, framebuffer_size_callback );
    }
  }

  void tick(){
    processInput( mWindow );

    glfwSwapBuffers( mWindow );

    glfwPollEvents();
  }

  bool shouldClose(){
    return glfwWindowShouldClose( mWindow );
  }

  ~glfw_handler(){
    glfwTerminate();
  }
};

int main(){
  glfw_handler gh;

  if( !gladLoadGLLoader( GLADloadproc( glfwGetProcAddress ) ) ){
    cout << "GLAD failure" << endl;
    return -2;
  }

  glViewport( 0, 0, 1024, 768 );

  while( !gh.shouldClose() ){
    glClear( GL_COLOR_BUFFER_BIT );

    gh.tick();
  }

  return 0;
}

