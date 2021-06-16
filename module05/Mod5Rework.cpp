// ***** IMPORTS/INCLUDE ******
#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      // Image loading Utility functions

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// imported classes
#include <learnOpengl/camera.h> // Camera class
#include <learnOpengl/shaders.h>

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// ***** NAMESPACE ******
namespace
{

    const char* const WINDOW_TITLE = "Katie's Graphics Project"; // Macro for window title

    // window width and height
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 800;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;         // Handle for the vertex array object
        GLuint vbo;         // Handle for the vertex buffer object
        GLuint nVertices;    // Number of indices of the mesh
        // GLuint nIndices;    // Number of indices of the mesh
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;

    // camera
    Camera gCamera(glm::vec3(0.2f, 5.6f, 9.9f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // frame rate (time between each frame)
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    // Triangle mesh data
    GLMesh gMesh;

    // Texture
    GLuint gTextureId;
    glm::vec2 gUVScale(10.0f, 10.0f);
    GLint gTexWrapMode = GL_REPEAT;

    // Shader programs
    GLuint gProgramId;
    GLuint gCubeProgramId;
    GLuint gLampProgramId;


    /*
    // Subject position and scale
    glm::vec3 gCubePosition(0.0f, 0.0f, 0.0f);
    glm::vec3 gCubeScale(2.0f);

    // Cube and light color
    //m::vec3 gObjectColor(0.6f, 0.5f, 0.75f);
    glm::vec3 gObjectColor(1.f, 0.2f, 0.0f);
    glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);

    // Light position and scale
    glm::vec3 gLightPosition(1.5f, 0.5f, 3.0f);
    glm::vec3 gLightScale(0.3f);

    // Lamp animation
    bool gIsLampOrbiting = true;
    */
}


// ****** USER-DEFINED FUNCTIONS *****
// intialize OpenGL
bool initializeOGL(int, char* [], GLFWwindow** window);


// camera


// projection
void changeProjection();
void toOrtho();
void toPerspective();

// navigation
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyboardNavigation(GLFWwindow* window);
void keyboardControl(GLFWwindow* window);

// textures


//


// shaders
// generic shader
bool createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void destroyShaderProgram(GLuint programId);
// GLuint gCubeProgramId;
// GLuint gLampProgramId;


// draw objects
void draw(floor);
void draw(wall);
void draw(door);
void draw(sideTable);
void draw(sideTable);
void draw(coffeeTable);
void draw(reef);
void draw(lamp);
void draw(couch);


/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif
/*
bool initializeScene(int, char*[], GLFWwindow** window);

// render
void render();

// shaders
bool createShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void destroyShaderProgram(GLuint programId);

// mesh
void createMesh(GLMesh& mesh);
void destroyMesh(GLMesh& mesh);

// navigation input & output
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyboardNavigation(GLFWwindow* window);

// user view
void resizeWindow(GLFWwindow* window, int width, int height);
void enableView(GLFWwindow* window);
void switchMVProjection(GLFWwindow* window, int key, int scancode, int action, int mods);


*/

/*

// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void clearSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void toOrtho();
void toPerspective();
void draw1();
void draw2();
void draw3();
void draw4();
void draw5();


*/







