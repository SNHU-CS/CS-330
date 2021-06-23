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

#include <learnOpengl/camera.h> // Camera class
// #include <learnOpengl/shaders.h>

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// ********** NAMESPACE ***********
// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Katie's Graphics Project"; // Macro for window title

    // Variables for window width and height
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

    // Shader programs
    GLuint gCubeProgramId;
    GLuint gLampProgramId;

    // Triangle mesh data
    GLMesh gMesh;
    GLMesh gMeshTable;
    GLMesh gMeshDresserDrawer;

    // Texture
    GLint gTexWrapMode = GL_REPEAT;
    GLint gTexFilterMode = GL_LINEAR;
    GLuint gTextureId; // original

    GLuint texDoorClassic; // 0
    GLuint texDoorRustic; // 1
    GLuint texSunsetPic; // 2
    GLuint texRug; // 3
    GLuint texWallpaper; // 4
    GLuint texGrassAlphine; // 5
    GLuint texMarbleCream; // 6
    GLuint texCottonCream; // 7
    GLuint texFabricRed; // 8
    GLuint texMetalBlack; // 9
    GLuint texWoodHerring; // 10
    GLuint texWoodSolidDark; // 11
    GLuint texRusticWood; // 12
    GLuint texWoodOakFine; // 13
    GLuint texWalnutMed; // 14
    GLuint texDresserDrawer; //15

    // also check into bindless textures later
    // remember to update array size as textures and images are added
    // GLuint textureIdAll[] = {
    GLuint textureIdAll[16] = {
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
        texRusticWood, // 12
        texWoodOakFine, // 13
        texWalnutMed, // 14
        texDresserDrawer, //15
    };
    int texCount = size(textureIdAll);
    //int texCount = textureIdAll.length;
    //int texCount = 16;

    // texture images: uv
    // check if can define values within shader, draw or mesh later
    glm::vec2 gUVScale(0.5f, 10.0f);
    glm::vec2 gUVScaleSideDresser(0.25f, 0.25f);
    glm::vec2 gUVScaleDresserLegs(5.25f, 5.25f);
    glm::vec2 gUVScaleDresserDrawer(5.25f, 5.25f);


    // camera
    Camera gCamera(glm::vec3(0.2f, 2.6f, 9.9f)); // do not change. precalcated to fit scene 0.2f, 5.6f, 9.9f
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    // CUBE
    // Subject position and scale
    glm::vec3 gCubePosition(0.0f, 0.0f, 0.0f);
    glm::vec3 gCubeScale(2.0f);

    // Cube and light color
    //m::vec3 gObjectColor(0.6f, 0.5f, 0.75f);
    glm::vec3 gObjectColor(1.f, 0.2f, 0.0f);
    glm::vec3 gLightColor(1.0f, 1.0f, 1.0f);

    // PROJECTING CUBE LIGHT
    // Light position and scale
    glm::vec3 gLightPosition(1.5f, 0.5f, 3.0f);
    glm::vec3 gLightScale(0.3f);

}

// *********** USER-DEFINED FUNCTIONS **********
bool initializeOGL(int, char* [], GLFWwindow** window);
void rendering();

// window resize and projection
void resizeWindow(GLFWwindow* window, int width, int height);
void toOrtho();
void toPerspective();
void changeProjectionCallback(); // use keyboard to change projection (event)

// shaders
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

// user Inputs
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyboardNavigation(GLFWwindow* window);
// event
void keyboardControl(GLFWwindow* window);

// textures
void flipImageVertical(unsigned char* image, int width, int height, int channels);
bool createEachTexture(const char* filename, GLuint& textureId, GLint gTexWrapMode, GLint gTexFilterMode);
//check into implementing instead of createEachTexture crowding main
bool createAllTexture();
void deleteTexture(GLuint& textureIdAll, int texCount);

void UDestroyTexture(GLuint textureId);

// mesh
void meshFloor(GLMesh& mesh);
void meshWall(GLMesh& mesh);
void meshSideTables(GLMesh& mesh);
void meshDresserDrawer(GLMesh& mesh);
void meshCoffeeTable(GLMesh& mesh);
void meshBalloons(GLMesh& mesh);
void meshWreath(GLMesh& mesh);
void meshLamp(GLMesh& mesh);
void meshDoor(GLMesh& mesh);
void meshCouch(GLMesh& mesh);

void UCreateMesh(GLMesh& mesh);
void UDestroyMesh(GLMesh& mesh);

// draw
void drawFloor();
void drawWall();
void drawSideTables();
void drawDresserDrawer();
void drawCoffeeTable();
void drawBalloons();
void drawWreath();
void drawLamp();
void drawDoor();
void drawCouch();

void DrawLight();
void DrawCube();


// *********** SHADERS: DEFINE **********
/* Cube Vertex Shader Source Code*/
const GLchar* cubeVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
    layout(location = 1) in vec3 normal; // VAP position 1 for normals
    layout(location = 2) in vec2 textureCoordinate;

    out vec3 vertexNormal; // For outgoing normals to fragment shader
    out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
    out vec2 vertexTextureCoordinate;

    //Uniform / Global variables for the  transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

    vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

    vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
    vertexTextureCoordinate = textureCoordinate;
}
);


/* Cube Fragment Shader Source Code*/
const GLchar* cubeFragmentShaderSource = GLSL(440,

    in vec3 vertexNormal; // For incoming normals
    in vec3 vertexFragmentPos; // For incoming fragment position
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor; // For outgoing cube color to the GPU

    // Uniform / Global variables for object color, light color, light position, and camera/view position
    uniform vec3 objectColor;
    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D uTexture; // Useful when working with multiple textures
    uniform vec2 uvScale;


void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

    //Calculate Ambient lighting*/
    float ambientStrength = 0.1f; // Set ambient or global lighting strength
    vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

    //Calculate Diffuse lighting*/
    vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
    vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
    float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
    vec3 diffuse = impact * lightColor; // Generate diffuse light color

    //Calculate Specular lighting*/
    float specularIntensity = 0.8f; // Set specular light strength
    float highlightSize = 16.0f; // Set specular highlight size
    vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
    vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
    //Calculate specular component
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
    vec3 specular = specularIntensity * specularComponent * lightColor;

    // Texture holds the color to be used for all three components
    vec4 textureColor = texture(uTexture, vertexTextureCoordinate * uvScale);

    // Calculate phong result
    vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

    fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
}
);


/* Lamp Vertex Shader Source Code*/
const GLchar* lampVertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position;
    layout(location = 2) in vec2 textureCoordinate;

    out vec2 vertexTextureCoordinate;

    //Uniform / Global variables for the  transform matrices
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
        vertexTextureCoordinate = textureCoordinate;
    }
);


/* Fragment Shader Source Code*/
const GLchar* lampFragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

    uniform sampler2D uTexRusticWood;
    uniform sampler2D uTexDresserDrawer;
    uniform vec2 uvScaleSideDresser;
    uniform vec2 uvScaleDresserLegs;
    uniform vec2 uvScaleDresserDrawer;


    void main()
    {   
        //unsure of correct method. ALTERNATIVE IDEAS TO TEST OUT
        // https://open.gl/textures
        //vec4 fragTexRusticWood = texture(uTexRusticWood, vertexTextureCoordinate);
        vec4 fragTexRusticWood = texture(uTexRusticWood, vertexTextureCoordinate * uvScaleSideDresser);
        //vec4 fragTexRusticWoodLegs = texture(uTexRusticWood, vertexTextureCoordinate);
        vec4 fragTexRusticWoodLegs = texture(uTexRusticWood, vertexTextureCoordinate * uvScaleDresserLegs);
        //vec4 fragImgDresserDrawer = texture(uTexDresserDrawer, vertexTextureCoordinate);
        vec4 fragImgDresserDrawer = texture(uTexDresserDrawer, vertexTextureCoordinate * uvScaleDresserDrawer);

        // ideas to test for fragment color/output
        // fragmentColor = texture(uTexRusticWood, vertexTextureCoordinate * uvScaleSideDresser); // original
        fragmentColor = fragTexRusticWood;
        // fragmentColor = fragTexRusticWoodLegs;
        // fragmentColor = fragImgDresserDrawer;
        // fragmentColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
        // fragmentColor = fragTexRusticWood * fragTexRusticWoodLegs;
        // fragmentColor = fragTexRusticWood + fragTexRusticWoodLegs;
        // fragmentColor = fragTexRusticWood * fragTexRusticWoodLegs * fragImgDresserDrawer;
        // fragmentColor = fragTexRusticWood * fragTexRusticWoodLegs + fragImgDresserDrawer;
        // fragmentColor = fragTexRusticWood + fragTexRusticWoodLegs + fragImgDresserDrawer;
        // fragmentColor = mix(texture(uTexRusticWood, vertexTextureCoordinate), texture(texture2, TexCoord), 0.2);


    }
);


// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it
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

// ********** MAIN CLASS ***********
int main(int argc, char* argv[])
{
    if (!initializeOGL(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
    UCreateMesh(gMesh); // Calls the function to create the Vertex Buffer Object
    meshSideTables(gMeshTable);
    meshDresserDrawer(gMeshDresserDrawer);


    // Create the shader programs
    if (!UCreateShaderProgram(cubeVertexShaderSource, cubeFragmentShaderSource, gCubeProgramId))
        return EXIT_FAILURE;

    if (!UCreateShaderProgram(lampVertexShaderSource, lampFragmentShaderSource, gLampProgramId))
        return EXIT_FAILURE;


    // TODO: MINIMIZE INTO FUNCTION(S) LATER
    // type (image/texture): texture, object that uses textur
    // createAllTexture();
    
    // texture: wood (solid natural)/dark, table/alternative
    const char* texFilename = "../../resources/textures/solid-dark-wood.jpg";
    if (!createEachTexture(texFilename, texWoodSolidDark, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    glUseProgram(gCubeProgramId);
    glUniform1i(glGetUniformLocation(gCubeProgramId, "uTexture"), 0);

    // texture: wood (straigtened rustic), side tables
    texFilename = "../../resources/textures/wood-scratched.jpg";
    //texFilename = "../../resources/textures/dresserDrawer.png";
    if (!createEachTexture(texFilename, texRusticWood, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    glUseProgram(gLampProgramId);
    glUniform1i(glGetUniformLocation(gLampProgramId, "uTexRusticWood"), 1);
    
    // image: dresser drawer, side tables
    texFilename = "../../resources/images/dresserDrawer.png";
    if (!createEachTexture(texFilename, texDresserDrawer, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    glUseProgram(gLampProgramId);
    glUniform1i(glGetUniformLocation(gLampProgramId, "uTexDresserDrawer"), 2);
    


    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // render loop
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        keyboardNavigation(gWindow);

        // Render this frame
        rendering();

        glfwPollEvents();
    }

    // Release mesh data
    
    UDestroyMesh(gMesh);
    UDestroyMesh(gMeshTable);
    //UDestroyMesh(gMeshDresserDrawer);

    // Release texture
    // use destroy textures function later instead
    UDestroyTexture(gTextureId);
    UDestroyTexture(texRusticWood);
    //UDestroyTexture(texDresserDrawer);

    // Release shader programs
    UDestroyShaderProgram(gCubeProgramId);
    UDestroyShaderProgram(gLampProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}

// ********** INTIALIZE OPENGL AND EVENTS **********
// Initialize GLFW, GLEW, and create a window
bool initializeOGL(int argc, char* argv[], GLFWwindow** window)
{
    // GLFW: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW: window creation
    // ---------------------
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
    glfwSetScrollCallback(*window, mouseScrollCallback);
    glfwSetMouseButtonCallback(*window, mouseButtonCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLEW: initialize
    // ----------------
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


// ********** WINDOW RESIZE AND PROJECTION SETTINGS **********
// adjusts window when screen size changed is changed (by OS or user)
void resizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
void toOrtho()
{
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
*/

/*
void toPerspective()
{
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
*/

void changeProjectionCallback()
{
    // ADD: change to ortho
    // ADD: reset to original
}


// ********** KEYBOARD-BASED NAVIGATION **********
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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


// ********** MOUSE-BASED NAVIGATION **********
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}


// glfw: handle mouse button events
// --------------------------------
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        if (action == GLFW_PRESS)
            cout << "Left mouse button pressed" << endl;
        else
            cout << "Left mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
        if (action == GLFW_PRESS)
            cout << "Middle mouse button pressed" << endl;
        else
            cout << "Middle mouse button released" << endl;
    }
    break;

    case GLFW_MOUSE_BUTTON_RIGHT:
    {
        if (action == GLFW_PRESS)
            cout << "Right mouse button pressed" << endl;
        else
            cout << "Right mouse button released" << endl;
    }
    break;

    default:
        cout << "Unhandled mouse button event" << endl;
        break;
    }
}


// ********** RENDERING **********
// Functioned called to render a frame
void rendering()
{
    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 scale;
    glm::mat4 rotation;
    glm::mat4 translation;
    glm::mat4 model = translation * rotation * scale;

    // move to cube (or remove if cube is deleted)
    // Activate the cube VAO (used by cube)
    glBindVertexArray(gMesh.vao);


    DrawCube();
    // DrawLight();
    drawFloor();
    drawWall();
    drawSideTables();
    drawDresserDrawer();
    drawCoffeeTable();
    drawBalloons();
    drawWreath();
    drawLamp();
    drawDoor();
    drawCouch();

    // Deactivate the Vertex Array Object and shader program
    glBindVertexArray(0);
    glUseProgram(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}




// ********** TEXTURES AND IMAGES **********

// generates settings for individual texture 
// assumes all images are declared and processed as GL_TEXTURE_2D
bool createEachTexture(const char* filename, GLuint& textureId, GLint gTexWrapMode, GLint gTexFilterMode)
{
    int width, height, channels;

    unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

    if (image)
    {
        flipImageVertical(image, width, height, channels);

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gTexWrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gTexWrapMode);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gTexFilterMode);
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

 /*
 bool createAllTexture()
 {
     // specify location, textureID, Wrap Parameter and Filter Parameter

     // image: classic door
     const char* texFilename = "../../resources/images/door-classic.jpg";
     if (!createEachTexture(texFilename, texDoorClassic, GL_CLAMP_TO_BORDER, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }
     glUseProgram(gCubeProgramId);
     glUniform1i(glGetUniformLocation(gCubeProgramId, "uTexture"), 0);



     // image: rustic door (dark wood by default)
     // texFilename = "../../resources/images/door-light-wood.png";
     texFilename = "../../resources/images/door-dark-wood.png";
     if (!createEachTexture(texFilename, texDoorRustic, GL_CLAMP_TO_BORDER, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // image: frame sunset picture (large by default. small available)
     texFilename = "../../resources/images/framed-sunset-lrg.png";
     //texFilename = "../../resources/images/frame-sunset-sml.png";
     if (!createEachTexture(texFilename, texSunsetPic, GL_CLAMP_TO_BORDER, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // image: vintage door rug
     texFilename = "../../resources/images/rug-vintage.png";
     if (!createEachTexture(texFilename, texRug, GL_CLAMP_TO_BORDER, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // texture: textured wallpaper/creamy brown-gray, wall
     // dark, mid, light shades
     // texFilename = "../../resources/textures/wallpaper-uni-dark.jpg";
     // texFilename = "../../resources/textures/wallpaper-uni-light.jpg";
     texFilename = "../../resources/textures/wallpaper-uni-med.jpg";
     if (!createEachTexture(texFilename, texWallpaper, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // texture: grass/alphine, wreath
     texFilename = "../../resources/textures/grass-alphine.jpg";
     if (!createEachTexture(texFilename, texGrassAlphine, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // texture: marble ceramic/cream, lamp base
     texFilename = "../../resources/textures/marble-cream.jpg";
     if (!createEachTexture(texFilename, texMarbleCream, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // texture: cotton/cream, lamp cover
     texFilename = "../../resources/textures/cotton-seamless.jpg";
     if (!createEachTexture(texFilename, texCottonCream, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // texture: cotton/red, couch fabric
     texFilename = "../../resources/textures/fabric-red-seamless.png";
     if (!createEachTexture(texFilename, texFabricRed, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }


     // texture: metal brused/black, long table, couch legs
     texFilename = "../../resources/textures/metal-black-brush.jpg";
     if (!createEachTexture(texFilename, texMetalBlack, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // texture: wood (herring pattern)/dark, floor
     texFilename = "../../resources/textures/wood-floor-herringdark.jpg";
     if (!createEachTexture(texFilename, texWoodHerring, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // texture: wood (solid natural)/dark, table/alternative
     texFilename = "../../resources/textures/solid-dark-wood.jpg";
     if (!createEachTexture(texFilename, texWoodSolidDark, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // texture: wood (straigtened rustic), side tables
     texFilename = "../../resources/textures/wood-scratched.jpg";
     if (!createEachTexture(texFilename, texRusticWood, GL_REPEAT, GL_LINEAR))
     {
         cout << "Failed to load texture " << texFilename << endl;
         return EXIT_FAILURE;
     }

     // CONTINUE ADDED IMAGES FROM 14+

     return true;
 }
 */

// delete textures. change this to default later
void deleteTexture(GLuint& textureIdAll, int texCount)
{
    // uses glDeleteTextures
    // source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteTextures.xhtml
    glDeleteTextures(texCount, &textureIdAll);
}

// remove once deleteTexture properly functions/enabled
void UDestroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}


// ********** SHADERS: CREATE AND DESTROY **********
// Implements the UCreateShaders function
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
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


void UDestroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

// ********** OBJECT MESH **********
// Implements the UCreateMesh function

void UCreateMesh(GLMesh& mesh)
{
    // Position and Color data
    GLfloat verts[] = {
        //Positions          //Normals
        // ------------------------------------------------------
    //Back Face          //Negative Z Normal  Texture Coords.
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    //Front Face         //Positive Z Normal
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

      //Left Face          //Negative X Normal
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     //Right Face         //Positive X Normal
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     //Bottom Face        //Negative Y Normal
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    //Top Face           //Positive Y Normal
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}


// mesh
void meshFloor(GLMesh& mesh)
{
    // texture - wood floor

    GLfloat verts[] = {
        // Vertex Positions    // normals        // textures
        // bottom
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -0.5f,  0.0, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.5f, 1.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, -0.2f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    mesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));


    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

void meshWall(GLMesh& mesh)
{
    // plane
    // light gray-blue texture
    // add sunset pic image
}

void meshDoor(GLMesh& mesh)
{
    // apply as texture to wall instead??
    // plane
    // door image

}

void meshSideTables(GLMesh& gMeshTable)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // front
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        // back
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        // left side
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        // right side
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         // bottom
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        // top
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMeshTable.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMeshTable.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMeshTable.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMeshTable.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMeshTable.vbo); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}


void meshDresserDrawer(GLMesh& gMeshDresserDrawer)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // place at front of side table dresser cube
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMeshDresserDrawer.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMeshDresserDrawer.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMeshDresserDrawer.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMeshDresserDrawer.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMeshDresserDrawer.vbo); // Activates the buffer
    glBindBuffer(GL_ARRAY_BUFFER, gMeshDresserDrawer.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}


void meshCoffeeTable(GLMesh& mesh)
{
    // elongated cuboid
    // 4 legs
    // metal texture
}


void meshWreath(GLMesh& mesh)
{
    // torus
    // alphine/green tree texture
    // ??add ornaments??
}


void meshBalloons(GLMesh& mesh)
{
    // balloon
    // solid color texture wit light shine
}

void meshLamp(GLMesh& mesh)
{
    // cylinder
    // cylinder
    // cone
    // marble texture, cloth texture

}


void meshCouch(GLMesh& mesh)
{
    // red fabric texture
    // metal legs texture
}


void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, &mesh.vbo);
}


/*  DRAW ELEMENT TEMPLATE
    // Using indexed drawing- store only the unique vertices and then specify the order at which we want to draw these vertices in.
    // Position and Color data of pyramid
    GLfloat verts[] = {
        // Vertex Positions    // Normal            //texture coordinates
        // peek of pyramid
         0.0f,  0.5f,  0.0f,   0.0f,  1.0f,  0.0f,  0.5f, 0.5f, // V0 Top: center vertex
        // sides of pyramid
         0.5f, -0.5f,  0.5f,  -1.0f, -1.0f,  1.0f,  1.0f, 0.0f, // V1: Side: Front Bottom-Right
        -0.5f, -0.5f,  0.5f,  -1.0f, -1.0f,  1.0f,  0.0f, 0.0f, // V2 Side: Front Bottom-Left
         0.5f, -0.5f, -0.5f,  -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, // V3 Side: Back bottom-right
        -0.5f, -0.5f, -0.5f,  -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, // V4 Side: Back bottom-left

        // Base of pyramid. (Texture coordinates are different than sides)
         0.5f, -0.5f,  0.5f,  -1.0f, -1.0f,  1.0f,  1.0f, 1.0f, // V5 Base: Front-Right
        -0.5f, -0.5f,  0.5f,  -1.0f, -1.0f,  1.0f,  0.0f, 1.0f, // V6 Base: Front-Left
         0.5f, -0.5f, -0.5f,  -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, // V7 Base: Back-right
        -0.5f, -0.5f, -0.5f,  -1.0f, -1.0f, -1.0f,  0.0f, 0.0f  // V8 Base: Back-left
    };

    // Index data to share position data of pyramid
    GLushort indices[] = {
        0, 1, 2,  // Triangle 1 - front
        0, 1, 3,  // Triangle 2 - right
        0, 3, 4,  // Triangle 3 - back
        0, 2, 4,  // Triangle 4 - left
        7, 8, 5,  // Triangle 5 - bottom/front
        5, 8, 6   // Triangle 6 - bottom/back
    };

    //multi-light how-to
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        //glm::vec3(-4.0f,  2.0f, -12.0f),
        //glm::vec3(0.0f,  0.0f, -3.0f)
    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // buffer for vertex data
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    // buffer for indices data
    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // calculate strides between vertex coordinates
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    // position pointer
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);
    // normal pointer
    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
    // texture pointer
    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);

    //NEED TO ADD IN UV SCALE

*/




// ********** DRAW OBJECTS **********
void drawFloor()
{

}

void drawWall()
{

}

/* TO DRAW BOTH SIDE TABLES, ONLY NEED TO ADD ADDITIONAL POSITIONS
 * for multiple tables, dresser cuboid will need the loop uncomment and positions added
 * fore more legs, legs wil need extra positiongs added to the array used in the leg loop
 * ROTATION: legs uses same rotation as dresser cuboid.
 *            IF needed, add loop for rotation changes
 */
//  IMPORTANT: remember to keep in position with drawers (drawDresserDrawer)
void drawSideTables()
{

    // declare objects
    glm::mat4 scale;
    glm::mat4 rotation;
    glm::mat4 translation;
    glm::mat4 model = translation * rotation * scale;;
    GLint modelLoc;

    // ********** dresser main cuboid ************************
   // dresser cuboid - 1 per table, currently 2 tables
   // create model view: scale, rotate, translate
    scale = glm::scale(glm::vec3(1.3f, 1.2f, 1.3f));
    rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    translation = glm::translate(glm::vec3(-4.0f, 0.8f, 1.5f));

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    // Creates a perspective projection
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Set the shader to be used
    glUseProgram(gLampProgramId);

    /* sets up loop for multiple tables. not needed yet
    // each table has a unique position
    glm::vec3 tablePosition[] = {
    glm::vec3(-3.5f, 0.1f, 1.0f), // 1st dresser
    // position currently unknown for 2nd dresser
    glm::vec3(-3.5f, 0.1f, 1.0f), // 2nd dresser
    };

    //counts the number of objects
    int tableCount = sizeof(tablePosition) / sizeof(tablePosition[0]);

    // draws each dresser cuboid
    // note: rotation is assumed to be the same. if differs, needs to add/integrate array for rotations for both cuboid and legs
    // for (unsigned int i = 0; i < tableCount; i++)
    {
    */
    // Retrieves and passes transform matrices to the Shader program
    modelLoc = glGetUniformLocation(gLampProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gLampProgramId, "view");
    GLint projLoc = glGetUniformLocation(gLampProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint UVScaleLoc = glGetUniformLocation(gLampProgramId, "uvScaleSideDresser");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScaleSideDresser));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMeshTable.vao);

    // bind textures on corresponding texture units
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, gTextureId);
    glActiveTexture(GL_TEXTURE1); // 12
    glBindTexture(GL_TEXTURE_2D, texRusticWood);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMeshTable.vao);
    // draws primary dresser cube
    // Draws the triangles

    glDrawArrays(GL_TRIANGLES, 0, gMeshTable.nVertices);
    // IMPORTANT: uncomment when loop is integrated. closes for loop
    //}

    // ********** side table legs ************************
    // small legs: 4 per table, currently 2 tables
    
    // ROTATION NOTE: legs uses same rotation as dresser cuboid.
    //                add rotation loop if rotation varies between tables
    // scale for legs (uniform size for all 4 legs)
    scale = glm::scale(glm::vec3(0.15f, 0.3f, 0.2f));

    // each leg has a unique position
    glm::vec3 legPosition[] = {
    // 1st dresser
    glm::vec3(-3.5f, 0.1f, 1.0f), // right front leg
    glm::vec3(-4.5f, 0.1f, 1.0f), // left front leg
    glm::vec3(-3.5f, 0.1f, 2.0f), // right back leg
    glm::vec3(-4.5f, 0.1f, 2.0f) // left back leg
    /* needed for 2nd dresser. position currently unknown
    // 2nd dresser
    glm::vec3(-3.5f, 0.1f, 1.0f), // right front leg
    glm::vec3(-4.5f, 0.1f, 1.0f), // left front leg
    glm::vec3(-3.5f, 0.1f, 2.0f), // right back leg
    glm::vec3(-4.5f, 0.1f, 2.0f) // left back leg
    */
    };

    // counts the number of objects
    int legCount = sizeof(legPosition) / sizeof(legPosition[0]);

    // draws each leg
    for (unsigned int i = 0; i < legCount; i++)
    {
        // recalculates model matrix with new position
        translation = glm::translate(glm::vec3(legPosition[i]));
        model = translation * rotation * scale;

        // Retrieves and passes transform matrices to the Shader program
        modelLoc = glGetUniformLocation(gLampProgramId, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // UV scale unique to legs
        GLint UVScaleLoc = glGetUniformLocation(gLampProgramId, "uvScaleDresserLegs");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScaleDresserLegs));
        
        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshTable.vao);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texRusticWood);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMeshTable.nVertices);
    }
}


/* TO DRAW BOTH SIDE TABLES, ONLY NEED TO ADD ADDITIONAL POSITIONS
 * for multiple tables, dresser cuboid will need the loop uncomment and positions added
 * fore more legs, legs wil need extra positiongs added to the array used in the leg loop
 * ROTATION: legs uses same rotation as dresser cuboid.
 *            IF needed, add loop for rotation changes
 */
//IMPORTANT: remember to keep in-sync (positioning) with side tables (drawSideTables)
void drawDresserDrawer()
{
        // declare objects
        glm::mat4 scale;
        glm::mat4 rotation;
        glm::mat4 translation;
        glm::mat4 model = translation * rotation * scale;;
        GLint modelLoc;

        // ********** dresser main cuboid ************************
       // dresser drawers - 1 per table, currently 2 tables. possible 2 if scaling is off
       // create model view: scale, rotate, translate

        // TODO: ADJUST SCALE and possible translation. PRE-SET TO DRESSER 
        scale = glm::scale(glm::vec3(1.8f, 1.8f, 1.8f));
        rotation = glm::rotate(0.0f, glm::vec3(3.0f, 0.1f, 0.0f));
        translation = glm::translate(glm::vec3(4.0f, 0.8f, 1.5f));

        // camera/view transformation
        glm::mat4 view = gCamera.GetViewMatrix();

        // Creates a perspective projection
        glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

        // Model matrix: transformations are applied right-to-left order
        model = translation * rotation * scale;

        // Set the shader to be used
        glUseProgram(gLampProgramId);

        /* sets up loop for multiple tables. each to check coordination with legs (below) and drawer function
         // each drawer has a unique position
        glm::vec3 drawerPosition[] = {
        glm::vec3(-3.5f, 0.1f, 1.0f), // 1st dresser
        // position currently unknown for 2nd dresser
        glm::vec3(-3.5f, 0.1f, 1.0f), // 2nd dresser
        };

        // counts the number of objects
        int drawerCount = sizeof(drawerPosition) / sizeof(drawerPosition[0]);

        // draws dresser draw for each table
        // note: rotation is assumed to be the same. if differs, needs to add/integrate array for rotations for both cuboid and legs
        // for (unsigned int i = 0; i < drawerCount; i++)
        {
        */
        // Retrieves and passes transform matrices to the Shader program
        modelLoc = glGetUniformLocation(gLampProgramId, "model");
        GLint viewLoc = glGetUniformLocation(gLampProgramId, "view");
        GLint projLoc = glGetUniformLocation(gLampProgramId, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLint UVScaleLoc = glGetUniformLocation(gLampProgramId, "uvScaleDresserDrawer");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScaleDresserDrawer));

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMeshDresserDrawer.vao);

        // bind textures on corresponding texture units
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, gTextureId);
        glActiveTexture(GL_TEXTURE2); // 15
        glBindTexture(GL_TEXTURE_2D, texDresserDrawer);
        
        // Activate the VBOs contained within the mesh's VA
        glBindVertexArray(gMeshDresserDrawer.vao);
        // draws primary dresser cube
        // 
        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMeshDresserDrawer.nVertices);
        // IMPORTANT: uncomment when loop is integrated. close for loop
        //}
}


void drawCoffeeTable()
{

}

void drawBalloons()
{

}

void drawWreath()
{

}

void drawLamp()
{

}

void drawDoor()
{

}

void drawCouch()
{

}

void DrawCube()
{
        glBindVertexArray(gMesh.vao);
    // CUBE: draw cube
    //----------------
    // Set the shader to be used
    glUseProgram(gCubeProgramId);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = glm::translate(gCubePosition) * glm::scale(gCubeScale);

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();

    // Creates a perspective projection
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(gCubeProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gCubeProgramId, "view");
    GLint projLoc = glGetUniformLocation(gCubeProgramId, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Reference matrix uniforms from the Cube Shader program for the cub color, light color, light position, and camera position
    GLint objectColorLoc = glGetUniformLocation(gCubeProgramId, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(gCubeProgramId, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(gCubeProgramId, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(gCubeProgramId, "viewPosition");

    // Pass color, light, and camera data to the Cube Shader program's corresponding uniforms
    glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(gCubeProgramId, "uvScale");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texWoodSolidDark);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

}

/*
void DrawLight() {
    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = glm::translate(gCubePosition) * glm::scale(gCubeScale);

    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();
    // LAMP: draw lamp
//----------------
    glUseProgram(gLampProgramId);

    //Transform the smaller cube used as a visual que for the light source
    model = glm::translate(gLightPosition) * glm::scale(gLightScale);

    // Reference matrix uniforms from the Lamp Shader program
    GLint modelLoc = glGetUniformLocation(gLampProgramId, "model");
    GLint viewLoc = glGetUniformLocation(gLampProgramId, "view");
    GLint projLoc = glGetUniformLocation(gLampProgramId, "projection");
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // Pass matrix data to the Lamp Shader program's matrix uniforms
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

}
*/
