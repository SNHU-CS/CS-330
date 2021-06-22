// !! Shaders will be replaced with calls from shader.h !!
// !! However, DO NOT replace shaders until shader.h is compatible with the program !!





// as skills advance, consider changing into content pipeline
// add missing debug error callbacks

// ********** IMPORTS/INCLUDE ***********
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


// ********** NAMESPACE ***********
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
        GLuint nIndices;    // Number of indices of the mesh
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;

    // camera
    Camera gCamera(glm::vec3(0.2f, 5.6f, 9.9f)); // do not change. precalcated to fit scene
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // frame rate (time between each frame)
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    // Triangle mesh data
    GLMesh gMesh;

    // Texture
    glm::vec2 gUVScale(10.0f, 10.0f);
    GLint gTexWrapMode = GL_REPEAT;
    GLint gTexFilterMode = GL_LINEAR;
    const char* texFilename;
    GLuint textureId;
    GLuint textureIdAll[] = {
        texDoorClassic, // 0
        texDoorRustic, // 1
        texSunsetPic, // 2
        texRug, // 3
        texWallpaper, // 4
        texGrassAlphine, // 5
        texMarbleCream, // 6
        texCottonCream, // 7
        texFabricRed, // 8
        texMetalBlack, // 9
        texWoodHerring, // 10
        texWoodSolidDark, // 11
        texWoodRustic // 12
    };
    int texCount = size(textureIdAll);


    // Shader programs
    GLuint shaderProgramID; // formally gProgramID
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


// *********** USER-DEFINED FUNCTIONS **********
// intialize OpenGL
bool initializeOGL(int argc, char* argv[], GLFWwindow** window);
//bool initializeWindow?Inout?Callback?(int, char* [], GLFWwindow** window);

// window resize and projection
void resizeWindow(GLFWwindow* window, int width, int height);
void toOrtho();
void toPerspective();
void changeProjectionCallback(); // use keyboard to change projection (event)

// user Inputs
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyboardNavigation(GLFWwindow* window);
void keyboardControl(GLFWwindow* window);


// textures
void flipImageVertical(unsigned char* image, int width, int height, int channels);
void createAllTextures(GLuint& textureIdAll, int texCount);
bool createEachTexture(const char* filename, GLuint& textureId, GLint gTexWrapMode, GLint gTexFilterMode);
void deleteTextures(GLuint& textureIdAll, int texCount);


// shaders
// generic shader
bool createshaderProgramID(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void destroyshaderProgramID(GLuint programIdAll);
// unique shaders for lighting.
// GLuint gCubeProgramId;
// GLuint gLampProgramId;

//void UCreateMesh(GLMesh& mesh);
//void UDestroyMesh(GLMesh& mesh);

// rendering, draw objects
// void rendering(); // <--generic for 1 object

// mesh

void meshFloor();
void meshWall();
void meshDoor();
void meshSideTableA();
void meshSideTableB();
void meshCoffeeTable();
void meshReef();
void meshLamp();
void meshCouch();



/*
// REMAINING UNDECLARED FUNCTIONS
// mesh
void createMesh(GLMesh& mesh);
void destroyMesh(GLMesh& mesh);

// user view + projection
void enableView(GLFWwindow* window);
*/

/*
// VERTEX ARRAY PROJECT
// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);

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


// ********** MAIN CLASS ***********
int main(int argc, char* argv[])
{

    if (initializeOGL(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
    UCreateMesh(gMesh); // Calls the function to create the Vertex Buffer Object

    // Create the shader program
    if (!createshaderProgramID((vertexShaderSource, fragmentShaderSource, shaderProgramID))
        return EXIT_FAILURE;

    glUseProgram(shaderProgramID);

    createAllTextures(textureIdAll, texCount);
    //  return EXIT_FAILURE;

    for (int t = 0, t < texCount, t++) {
        glUniform1i(glGetUniformLocation(shaderProgramID, textureIdAll[t]), t);      
    }


}


    // Sets the background color of the window to black (it will be implicitely used by glClear)
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    /* TODO: REVAMP ALL OF RENDER LOOP TO FIX NEW LAYOUT
    // render loop
    while (!glfwWindowShouldClose(gWindow))
    {
        // in its own function or here??
        // per-frame timing
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        processInput(gWindow);

        // Render this frame
        rendering();

        glfwPollEvents();
    }

    TODO: CHECK ON DESTROYING- WHERE AND IF NEEDED - NEW AREAS INCLUDING TEXTURES
    // Release mesh data... unsure if needed here anymore
    destroyMesh(gMesh);

    // Release shader program... unsure if needed here anymore
    destroyshaderProgramID(shaderProgramID);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}



// ********** INTIALIZE OPENGL AND EVENTS **********
// Initialize GLFW, GLEW, and user window
bool initializeOGL(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (*window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, resizeWindow);
    glfwSetCursorPosCallback(*window, mousePositionCallback);
    glfwSetScrollCallback(*window,  mouseScrollCallback);
    glfwSetMouseButtonCallback(*window, mouseButtonCallback);
    glfwSetProjectionCallback(*window, changeProjectionCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // Note: if using GLEW version 1.13 or earlier
    glewExperimental = GL_TRUE;
    GLenum GlewInitResult = glewInit();

    if (GLEW_OK != GlewInitResult)
    {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}

for (int t = 0, t < texCount, t++) {

    //glUniform1i(glGetUniformLocation(shaderProgramID, textureIdAll[t]), t);
}

// ********** RENDERING **********
/*
void URender()
{

    // Sends transform information to the Vertex shader
    GLuint transformLocation = glGetUniformLocation(gProgramId, "shaderTransform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformation));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.vao);

    // bind textures on corresponding texture units (activate and bind)
    GLuint textureIdAll[] = {
        texDoorClassic, // 0
        texDoorRustic, // 1
        texSunsetPic, // 2
        texRug, // 3
        texWallpaper, // 4
        texGrassAlphine, // 5
        texMarbleCream, // 6
        texCottonCream, // 7
        texFabricRed, // 8
        texMetalBlack, // 9
        texWoodHerring, // 10
        texWoodSolidDark, // 11
        texWoodRustic // 12
    };

    int texCount = size(textureIdAll)
    glBindTextures(0, texCount, textureIdAll);

    // Draws the triangle
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}

*/

// ********** WINDOW RESIZE AND PROJECTION SETTINGS **********
// adjusts window when screen size changed is changed (by OS or user)
void resizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void toOrtho()
{
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}


void toPerspective()
{
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void changeProjectionCallback()
{
    // ADD: change to ortho
    // ADD: reset to original
}


// ********** INPUT DEVICES **********
// input: mouse movement (inversed)
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (gFirstMouse)
    {
        gLastX = xpos;
        gLastY = ypos;
        gFirstMouse = false;
    }

    float xoffset = xpos - gLastX;
    float yoffset = gLastY - ypos; // reversed since y-coordinates go from bottom to top

    gLastX = xpos;
    gLastY = ypos;

    gCamera.ProcessMouseMovement(xoffset, yoffset);
}


//  input: mouse scroll wheel
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    // adjusts screen movement speed
    gCamera.ProcessMouseScroll(yoffset);
}


//  input: mouse button
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    // no action assigned yet. only log response
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    // no action assigned yet. only log response
    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    // no action assigned yet. only log response
    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    // no action assigned yet. only log response
    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}


//  input: keyboard navigation
void keyboardNavigation(GLFWwindow* window)
{
    static const float cameraSpeed = 2.5f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // move scene backward, forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        gCamera.ProcessKeyboard(FORWARD, gDeltaTime);

    // move scene left, right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        gCamera.ProcessKeyboard(LEFT, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        gCamera.ProcessKeyboard(RIGHT, gDeltaTime);

    //move scene down, up
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        gCamera.ProcessKeyboard(UP, gDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        gCamera.ProcessKeyboard(DOWN, gDeltaTime);
}


//  input: keyboard controls (non-navigation)
void keyboardControl(GLFWwindow* window) {
    // add projection here. call on ortho
    // use callback, not getkey
        // source: https://www.glfw.org/docs/3.3/input_guide.html#input_key
    // Creates a ortho projection
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
}


// ********** SHADERS **********
// !! Shaders will be replaced with calls from shader.h !!

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif


/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
    layout(location = 1) in vec4 color;  // Color data from Vertex Attrib Pointer 1
    layout(location = 2) in vec2 textureCoordinate; // texture data

    out vec4 vertexColor;
    out vec2 vertexTexCoord;

    void main()
    {
        gl_Position = vec4(position.x, position.y, position.z, 1.0);
        vertexColor = color;
        vertexTexCoord = textureCoordinate;
    }
);



// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main() {

    // Output color = color of the texture at the specified UV
    color = texture(myTextureSampler, UV).rgb;
}


// Applying multiple textures http://www.ozone3d.net/tutorials/glsl_texturing_p03.php
// Fragment Shader Source Code
const GLchar* fragmentShaderSource = GLSL(440,
    in vec4 vertexColor; // Variable to hold incoming color data from vertex shader
    in vec2 vertexTexCoord; // UV, texture coordinates

    out vec4 fragmentColor; // output

    // Values that stay constant for the whole mesh.
    uniform sampler2D texSample;

    void main()
    {
        // Output color = color of the texture at the specified UV
        //fragmentColor = vec4(vertexColor);
        fragmentColor = texture(texSample, vertexTexCoord); // Sends texture to the GPU for rendering
    }
);


// Implements the default Shader function
bool createshaderProgramID(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
    // Compilation and linkage error reporting
    int success = 0;
    char infoLog[512];

    // Create a Shader program object.
    programId = glCreateProgram();

    // Create the vertex and fragment shader objects
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    // Retrive the shader source
    glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
    glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

    // Compile the vertex shader, and print compilation errors (if any)
    glCompileShader(vertexShaderId); // compile the vertex shader
    // check for shader compile errors
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glCompileShader(fragmentShaderId); // compile the fragment shader
    // check for shader compile errors
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

        return false;
    }

    // Attached compiled shaders to the shader program
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    glLinkProgram(programId);   // links the shader program
    // check for linking errors
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

        return false;
    }

    glUseProgram(programId);    // Uses the shader program

    return true;
}


// destroys generic shader program
void destroyshaderProgramID(GLuint programId)
{
    glDeleteProgram(programId);
}



// ********** TEXTURES AND IMAGES **********
// flips images vertically to match OpenGl's y-axis
// (the y-axis of OpenGL and standard images are inverted when compared).
void flipImageVertical(unsigned char* image, int width, int height, int channels)
{
    for (int j = 0; j < height / 2; ++j)
    {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i)
        {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}


// generates settings for individual texture 
// assumes all images are declared and processed as GL_TEXTURE_2D
// FOLLOWUP: is & needed in front of GLint gTExtWrapMode or FilterMode?
bool createEachTexture(const char* filename, GLuint &textureId, GLint gTexWrapMode, GLint gTexFilterMode)
{
    int width, height, channels;

    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

    if (image)
    {
        flipImageVertical(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gTexWrapMode;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gTexWrapMode);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gTexFilterMode;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gTexFilterMode);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
        {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}


// load all textures
/* IMPORTANT: By default, assumes all images are declared and processed as GL_TEXTURE_2D.
 *    Specify location, textureID, Wrap Parameter and Filter Parameter
 *    To make S or T different from each other, a custom loop needs to be made, based on the createEachTexture
 */
void createAllTexture(GLuint& textureIdAll, int texCount)
{
    // specify location, textureID, Wrap Parameter and Filter Parameter

    // image: classic door
    texFilename = "../../resources/images/door-classic.png";
    if (!createTexture(texFilename, texDoorClassic, GL_CLAMP_TO_BORDER, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // image: rustic door (dark wood by default)
    // texFilename = "../../resources/images/door-light-wood.png";
    texFilename = "../../resources/images/door-dark-wood.png";
    if (!createTexture(texFilename, texDoorRustic, GL_CLAMP_TO_BORDER, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // image: frame sunset picture (large by default. small available)
    texFilename = "../../resources/images/framed-sunset-lrg.png";
    //texFilename = "../../resources/images/frame-sunset-sml.png";
    if (!createTexture(texFilename, texSunsetPic, GL_CLAMP_TO_BORDER, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // image: vintage door rug
    texFilename = "../../resources/images/rug-vintage.png";
    if (!createTexture(texFilename, texRug, GL_CLAMP_TO_BORDER, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // texture: textured wallpaper/creamy brown-gray, wall
    // dark, mid, light shades
    // texFilename = "../../resources/textures/wallpaper-uni-dark.jpg";
    // texFilename = "../../resources/textures/wallpaper-uni-light.jpg";
    texFilename = "../../resources/textures/wallpaper-uni-med.jpg";
    if (!createTexture(texFilename, texWallpaper, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // texture: grass/alphine, wreath
    texFilename = "../../resources/textures/grass-alphine.jpg";
    if (!createTexture(texFilename, texGrassAlphine, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // texture: marble ceramic/cream, lamp base
    texFilename = "../../resources/textures/marble-cream.jpg";
    if (!createTexture(texFilename, texMarbleCream, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // texture: cotton/cream, lamp cover
    texFilename = "../../resources/textures/cotton-seamless.jpg";
    if (!createTexture(texFilename, texCottonCream, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // texture: cotton/red, couch fabric
    texFilename = "../../resources/textures/fabric-red-seamless.png";
    if (!createTexture(texFilename, texFabricRed, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }


    // texture: metal brused/black, long table, couch legs
    texFilename = "../../resources/textures/metal-black-brush.jpg";
    if (!createTexture(texFilename, texMetalBlack, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // texture: wood (herring pattern)/dark, floor
    texFilename = "../../resources/textures/wood-floor-herringdark.jpg";
    if (!createTexture(texFilename, texWoodHerring, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // texture: wood (solid natural)/dark, table/alternative
    texFilename = "../../resources/textures/solid-dark-wood.jpg";
    if (!createTexture(texFilename, texWoodSolidDark, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // texture: wood (straigtened rustic), side tables
    texFilename = "../../resources/textures/wood-floor-scratched.jpg";
    if (!createTexture(texFilename, texWoodRustic, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    glUseProgram(shaderProgramID);
    // We set the texture as texture unit 0
    glUniform1i(glGetUniformLocation(shaderProgramID, "uTextureBase"), 0);
    // We set the texture as texture unit 1
    glUniform1i(glGetUniformLocation(shaderProgramID, "uTextureExtra"), 1);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
// -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);
}

// delete textures
void deleteTexture(GLuint &textureIdAll, int texCount)
{
    // uses glDeleteTextures
    // source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteTextures.xhtml
    int n = size(textureIdAll)
    glDeleteTextures(texCount, textureIdAll);
}

void meshFloor()
{
    // plane
    // texture - wood floor
}

void meshWalls()
{
    // plane
    // light gray-blue texture
    // add sunset pic image
}

void meshDoor()
{
    // apply as texture to wall instead??
    // plane
    // door image
}

void meshSideTableA()
{


    //consider combined into 1 class for BOTH side tables by using offsets
    // small cuboid
    // wood texture with special front texture for drawers
    // 4 legs
    // wood texture
}

void meshSideTableB()
{
    //consider combined into 1 class for BOTH side tables by using offsets
    // small cuboid
    // wood texture with special front texture for drawers
    // 4 legs
    // wood texture
}

void meshCoffeeTable()
{
    // elongated cuboid
    // 4 legs
    // metal texture
}

void meshReef()
{
    // torus
    // alphine/green tree texture
    // ??add ornaments??
}

void meshLamp()
{
    // cylinder
    // cylinder
    // cone
    // marble texture, cloth texture

}

void meshCouch()
{
    // red fabric texture
    // metal legs texture
}



