// ********** IMPORTS/INCLUDE ***********
// libraries
#include <iostream>         // cout, cerr
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#include <math.h>

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// other imports
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>      // Image loading Utility function
#include <learnOpengl/camera.h> // Camera class
// not coded yet: #include <learnOpengl/shaders.h>

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std; // Standard namespace


// ********** NAMESPACE ***********
// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Katie's OpenGL Graphics Project (C++)"; // Macro for window title

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;

    // Variables for window width and height
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 800;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

    // camera 
    // defines where camera is positioned
    Camera gCamera(glm::vec3(-0.4f, 2.2f, 10.5f)); // do not change. precalcated to fit scene at 0.2f, 5.6f, 9.9f
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // Frag shader program IDs
    GLuint gSideTableProgramId;
    GLuint gSideDrawerProgramId;
    GLuint gHouseFloorProgramId;
    GLuint gHouseWallProgramId;
    GLuint gHouseDoorProgramId;
    GLuint gPaintingProgramId;
    GLuint gCoffeeTableProgramId;
    GLuint gTableLegsProgramId;
    GLuint gLampBottomProgramId;
    GLuint gLampTopProgramId;
    GLuint gBalloonsProgramId;
    GLuint gCouchProgramId;


    // TODO: check into bindless textures later
    // Texture
    // defaults
    GLint gTexWrapMode = GL_REPEAT;
    GLint gTexFilterMode = GL_LINEAR;
    GLuint gTextureId; // original
    // texture IDs
    GLuint texWoodSolidDark;
    GLuint texSideTable;
    GLuint texSideDrawer;
    GLuint texHouseFloor;
    GLuint texHouseWall;
    GLuint texHouseDoor; 
    GLuint texPainting;  
    GLuint texCoffeeTable; 
    GLuint texMarbleCream; 
    GLuint texCottonCream; 
    GLenum texLampTop;
    GLenum texLampBottom;
    GLuint texMetalBlack;
    GLuint texTableLegs;
    GLuint texBalloons;
    GLuint texCouch;

    // assign texture slots (TEXTURE NUM #)
    GLenum texNumSideTable;
    GLenum texNumSideDrawer;
    GLenum texNumHouseFloor;
    GLenum texNumHouseWall;
    GLenum texNumHouseDoor;
    GLenum texNumPainting;
    GLenum texNumCoffeeTable;
    GLenum texNumTableLegs;
    GLenum texNumLampTop;
    GLenum texNumLampBottom;
    GLenum texNumBalloons;
    GLenum texNumCouch;

    // UV scale of textures
    // check if can define values within shader, draw or mesh later
    glm::vec2 gUVScale;
    glm::vec2 gUVScaleSideTable;
    glm::vec2 gUVScaleSideLegs;
    glm::vec2 gUVScaleSideDrawer;
    glm::vec2 gUVScaleHouseFloor;
    glm::vec2 gUVScaleHouseWall;
    glm::vec2 gUVScaleHouseDoor;
    glm::vec2 gUVScalePainting;
    glm::vec2 gUVScaleCoffeeTable;    
    glm::vec2 gUVScaleTableLegs;    
    glm::vec2 gUVScaleLampTop;
    glm::vec2 gUVScaleLampBottom;
    glm::vec2 gUVScaleBalloons;
    glm::vec2 gUVScaleCouch;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;         // Handle for the vertex array object
        GLuint vbo;         // Handle for the vertex buffer object
        GLuint vbos[2];
        GLuint nVertices;    // Number of indices of the mesh
        GLuint nIndices;    // Number of indices of the mesh
    };

    // mesh data
    GLMesh gMeshSideTable;
    GLMesh gMeshSideDrawer;
    GLMesh gMeshHouseFloor;
    GLMesh gMeshHouseWall;
    GLMesh gMeshHouseDoor;
    GLMesh gMeshPainting;
    GLMesh gMeshLamp;
    GLMesh gMeshBalloons;
    GLMesh gMeshCoffeeTable;
    GLMesh gMeshTableLegs;
    GLMesh gMeshCouch;


    // key light
    glm::vec3 gLightPosition(-0.5f, 5.6f, 4.6f);
    glm::vec3 gLightColor(0.0f, 1.0f, 0.0f);
    glm::vec3 gLightScale(1.0f);

    // fill/spot light
    glm::vec3 gLightPositionSpot(-10.5f, 1.0f, 4.6f);
    glm::vec3 gLightColorSpot(1.0f, 0.0f, 0.0f);
    glm::vec3 gLightScaleSpot(0.2f);
}


// *********** USER-DEFINED FUNCTIONS **********
// initialize libraries and events
bool initializeOGL(int, char* [], GLFWwindow** window);
void rendering(glm::mat4 view, glm::mat4 projection);

// create and destroy shaders
bool createShaderProgram(const char* vtxShaderSource, const char* FragShader, GLuint& programId);
void destroyShaderProgram(GLuint programId);

// window resize and projection
void resizeWindow(GLFWwindow* window, int width, int height);

// user Inputs
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void keyboardNavigationCallback(GLFWwindow* window);
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// textures
void flipImageVertical(unsigned char* image, int width, int height, int channels);
bool createTexture(const char* filename, GLuint& textureId, GLint gTexWrapMode, GLint gTexFilterMode);
void destroyTexture(GLuint textureId);
// TODO: research batch texture deletion
// void deleteAllTexture(GLuint& textureIdAll, int texCount);


// create mesh
// TODO: check for method to destroy as a batch
void destroyMesh(GLMesh& gMesh);
void createMeshSideTable(GLMesh& gMesh);
void createMeshSideDrawer(GLMesh& gMesh);
void createMeshHouseFloor(GLMesh& gMesh);
void createMeshHouseWall(GLMesh& gMesh);
void createMeshHouseDoor(GLMesh& gMesh);
void createMeshPainting(GLMesh& gMesh);
void createMeshLamp(GLMesh& gMesh);
void createMeshCoffeeTable(GLMesh& gMesh);
void createMeshTableLegs(GLMesh& gMesh);
void createMeshBalloons(GLMesh& gMesh);
void createMeshCouch(GLMesh& gMesh);


// draw objects
void drawSideTable(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawSideDrawer(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawHouseFloor(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawHouseWall(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawHouseDoor(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawPainting(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawLampBottom(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawLampTop(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawCoffeeTable(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawBalloons(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawTableLegs(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);
void drawCouch(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName);


// ****** VERTEX SHADER SOURCE CODE
// Vertex Shader Source Code
const GLchar* vertexShaderSource = GLSL(440,

    layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
    layout(location = 1) in vec3 normal; // VAP position 1 for normals
    layout(location = 2) in vec2 textureCoordinate; // VAP position 2 for textures and iamges

    out vec3 vertexFragmentPos; // outgoing color/pixels to fragment shader
    out vec3 vertexNormal; // outgoing normals to fragment shader
    out vec2 vertexTextureCoordinate;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices into clip coordinates
        vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // get fragment/pixel position in world space only (exclude view and projection)
        vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
        vertexTextureCoordinate = textureCoordinate;
    }
);


// ****** FRAGMENT SHADER SOURCE CODE
// UNABLE TO GET VERTEX TO ACCEPT A 2ND/DIFFERENT SET OF COORDINATES
// SOLUTION: create another frag shader (research other opportunities later)

// Fragment Shader: side table
const GLchar* sideTableFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texSideTable;
    uniform vec2 gUVScaleSideTable;
    uniform vec2 gUVScaleSideLegs;

    void main()
    {
        // https://open.gl/textures
        vec4 fragTexFrame = texture(texSideTable, vertexTextureCoordinate * gUVScaleSideTable);
        vec4 fragTexLegs = texture(texSideTable, vertexTextureCoordinate * gUVScaleSideLegs);

        if (fragTexFrame.a < 0.1)
            discard;
        if (fragTexLegs.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTexFrame.xyz; //*fragTexLegs.xyz;

        fragmentColor = vec4(keyLight, 1.0f);

    }
);

// Fragment Shader: side table drawer
const GLchar* sideDrawerFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texSideDrawer;
    uniform vec2 gUVScaleSideDrawer;

    void main()
    {
        vec4 fragTex = texture(texSideDrawer, vertexTextureCoordinate * gUVScaleSideDrawer);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);


// fragment shader: house floor 
const GLchar* houseFloorFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal; 
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texHouseFloor;
    uniform vec2 gUVScaleHouseFloor;

    void main()
    {
        //texture
        vec4 fragTex = texture(texHouseFloor, vertexTextureCoordinate * gUVScaleHouseFloor);
        if (fragTex.a < 0.1)
            discard;        
        
        /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

        // calculate ambient lighting
        float ambientStrength = 0.5f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate specular lighting
        float specularIntensity = 0.8f; // Set specular light strength
        float highlightSize = 16.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;
        
        // Calculate phong result with texture
        vec3 phong = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
        //fragmentColor = fragTex;

    }
);


// Fragment Shader: house wall 
const GLchar* houseWallFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texHouseWall;
    uniform vec2 gUVScaleHouseWall;

    void main()
    {
        vec4 fragTex = texture(texHouseWall, vertexTextureCoordinate * gUVScaleHouseWall);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);


// fragment shader: house door
const GLchar* houseDoorFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texHouseDoor;
    uniform vec2 gUVScaleHouseDoor;

    void main()
    {
        vec4 fragTex = texture(texHouseDoor, vertexTextureCoordinate * gUVScaleHouseDoor);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);



// fragment shader: painting
const GLchar* paintingFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texPainting;
    uniform vec2 gUVScalePainting;

    void main()
    {
        vec4 fragTex = texture(texPainting, vertexTextureCoordinate * gUVScalePainting);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);


// Fragment Shader: coffee table
const GLchar* coffeeTableFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texCoffeeTable;
    uniform vec2 gUVScaleCoffeeTable;

    void main()
    {
        vec4 fragTex = texture(texCoffeeTable, vertexTextureCoordinate * gUVScaleCoffeeTable);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);


// Fragment Shader: legs for tables (and other furniture)
const GLchar* tableLegsFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texTableLegs;
    uniform vec2 gUVScaleTableLegs;

    void main()
    {
        vec4 fragTex = texture(texTableLegs, vertexTextureCoordinate * gUVScaleTableLegs);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);


// Fragment Shader: lamp bottom (base)
const GLchar* lampBottomFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texLampBottom;
    uniform vec2 gUVScaleLampBottom;

    void main()
    {
        vec4 fragTex = texture(texLampBottom, vertexTextureCoordinate * gUVScaleLampBottom);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);


// Fragment Shader: lamp top (shade)
const GLchar* lampTopFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texLampTop;
    uniform vec2 gUVScaleLampTop;

    void main()
    {
        vec4 fragTex = texture(texLampTop, vertexTextureCoordinate * gUVScaleLampTop);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);

    }
);


// Fragment Shader: balloons
const GLchar* balloonsFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
uniform vec3 viewPosition;
    uniform sampler2D texBalloons;
    uniform vec2 gUVScaleBalloons;

    void main()
    {
        vec4 fragTex = texture(texBalloons, vertexTextureCoordinate * gUVScaleBalloons);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);


// Fragment Shader: couch seats and couch back rest
const GLchar* couchFragShader = GLSL(440,
    in vec3 vertexFragmentPos;
    in vec3 vertexNormal;
    in vec2 vertexTextureCoordinate;

    out vec4 fragmentColor;

    uniform vec3 lightColor;
    uniform vec3 lightPos;
    uniform vec3 viewPosition;
    uniform sampler2D texCouch;
    uniform vec2 gUVScaleCouch;

    void main()
    {
        vec4 fragTex = texture(texCouch, vertexTextureCoordinate * gUVScaleCouch);
        if (fragTex.a < 0.1)
            discard;

        //Phong lighting model calculations to generate ambient, diffuse, and specular components
        // calculate Ambient lighting
        float ambientStrength = 0.2f; // Set ambient or global lighting strength
        vec3 ambient = ambientStrength * lightColor; // Generate ambient light color

        // calculate Diffuse lighting
        vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
        vec3 lightDirection = normalize(lightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
        float impact = max(dot(norm, lightDirection), 0.5);// Calculate diffuse impact by generating dot product of normal and light
        vec3 diffuse = impact * lightColor; // Generate diffuse light color

        // calculate Specular lighting
        float specularIntensity = 0.0f; // Set specular light strength
        float highlightSize = 5.0f; // Set specular highlight size
        vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
        vec3 reflectDir = reflect(-lightDirection, norm);// Calculate reflection vector
        // calculate specular component
        float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
        vec3 specular = specularIntensity * specularComponent * lightColor;

        // calculate result
        vec3 keyLight = (ambient + diffuse + specular) * fragTex.xyz;

        fragmentColor = vec4(keyLight, 1.0f);
    }
);



// flip images vertically
// Images are loaded with Y axis going down. OpenGL's Y axis goes up
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
    // initialize OpenGL, GLFW, glfw events, and GLEW
    if (!initializeOGL(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // create the mesh for objects (creas VBO)
    // TODO: raise exceptions
    createMeshSideTable(gMeshSideTable);
    createMeshSideDrawer(gMeshSideDrawer);
    createMeshHouseFloor(gMeshHouseFloor);;
    createMeshHouseWall(gMeshHouseWall);
    createMeshHouseDoor(gMeshHouseDoor);
    createMeshPainting(gMeshPainting);
    createMeshCoffeeTable(gMeshCoffeeTable);
    createMeshTableLegs(gMeshTableLegs);
    createMeshLamp(gMeshLamp);
    createMeshBalloons(gMeshBalloons);
    createMeshCouch(gMeshCouch);


    /* ISSUE: started once added door in program.
    * when all exceptions are disabled within shaders, program works
    * when expections are enable, throws ANY/ALL expections. in either form, statement is not shown
    * started when added programming for texture5 for the door
    * TRIED: viewing fragshader, draw in rendering, and draw function. did not find conflicts
    */

    if (!createShaderProgram(vertexShaderSource, sideTableFragShader, gSideTableProgramId))
        cout << "Shader crash/return false: side table" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, sideDrawerFragShader, gSideDrawerProgramId))
        cout << "Shader crash/return false: side table drawers" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, houseFloorFragShader, gHouseFloorProgramId))
        cout << "Shader crash/return false: floor" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, houseWallFragShader, gHouseWallProgramId))
        cout << "Shader crash/return false: wall" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, houseDoorFragShader, gHouseDoorProgramId))
        cout << "Shader crash/return false: door" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, paintingFragShader, gPaintingProgramId))
        cout << "Shader crash/return false: painting" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, coffeeTableFragShader, gCoffeeTableProgramId))
        cout << "Shader crash/return false: coffee table" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, tableLegsFragShader, gTableLegsProgramId))
        cout << "Shader crash/return false: table legs" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, lampBottomFragShader, gLampBottomProgramId))
        cout << "Shader crash/return false: lamp bottom" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, lampTopFragShader, gLampTopProgramId))
        cout << "Shader crash/return false: lamp top" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, balloonsFragShader, gBalloonsProgramId))
        cout << "Shader crash/return false: balloons" << endl;
        //return EXIT_FAILURE;

    if (!createShaderProgram(vertexShaderSource, couchFragShader, gCouchProgramId))
        cout << "Shader crash/return false: couch" << endl;
        //return EXIT_FAILURE;


    // TODO: MINIMIZE INTO FUNCTION(S) LATER
    // TEXTURE: wood, dark brown/red solid
    const char* texFilename = "../../resources/textures/wood-dark-rotate.png";
    if (!createTexture(texFilename, texCoffeeTable, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // coffee table
    glUseProgram(gCoffeeTableProgramId);
    glUniform1i(glGetUniformLocation(gCoffeeTableProgramId, "texCoffeeTable"), 0);
    texNumCoffeeTable = GL_TEXTURE0;

    // TEXTURE: wood, straigtened rustic scratched
    texFilename = "../../resources/textures/wood-dark.png";
    if (!createTexture(texFilename, texSideTable, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // side tables
    glUseProgram(gSideTableProgramId);
    glUniform1i(glGetUniformLocation(gSideTableProgramId, "texSideTable"), 1);
    texNumSideTable = GL_TEXTURE1;

    // TEXTURE (IMAGE): dresser drawer, side tables
    texFilename = "../../resources/images/dresserdrawer1.png";
    if (!createTexture(texFilename, texSideDrawer, GL_CLAMP_TO_EDGE, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // side tables drawers
    glUseProgram(gSideDrawerProgramId);
    glUniform1i(glGetUniformLocation(gSideDrawerProgramId, "texSideDrawer"), 2);
    texNumSideDrawer = GL_TEXTURE2;


    // TEXTURE: wood (herring pattern)/dark
    texFilename = "../../resources/textures/wood-floor-herringdark.jpg";
    if (!createTexture(texFilename, texHouseFloor, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // floor
    glUseProgram(gHouseFloorProgramId);
    glUniform1i(glGetUniformLocation(gHouseFloorProgramId, "texHouseFloor"), 3);
    texNumHouseFloor = GL_TEXTURE3;


    // TEXTURE: blue geo pattern
    texFilename = "../../resources/textures/pattern-geo.png";
    if (!createTexture(texFilename, texHouseWall, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // wall
    glUseProgram(gHouseWallProgramId);
    glUniform1i(glGetUniformLocation(gHouseWallProgramId, "texHouseWall"), 4);
    texNumHouseWall = GL_TEXTURE4;


    // TEXTURE: door, old
    texFilename = "../../resources/images/door-dark-wood.png";
    if (!createTexture(texFilename, texHouseDoor, GL_CLAMP_TO_EDGE, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    //  door
    glUseProgram(gHouseDoorProgramId);
    glUniform1i(glGetUniformLocation(gHouseDoorProgramId, "texHouseDoor"), 5);
    texNumHouseDoor = GL_TEXTURE5;


    // TEXTURE: painted image
    texFilename = "../../resources/images/framed-sunset-lrg.png";
    if (!createTexture(texFilename, texPainting, GL_CLAMP_TO_EDGE, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    //  image
    glUseProgram(gPaintingProgramId);
    glUniform1i(glGetUniformLocation(gPaintingProgramId, "texPainting"), 6);
    texNumPainting = GL_TEXTURE6;


    // TEXTURE: brushed metal
    texFilename = "../../resources/textures/metal-black-brush.jpg";
    if (!createTexture(texFilename, texTableLegs, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    } 
    //  table legs, furniture legs
    glUseProgram(gTableLegsProgramId);
    glUniform1i(glGetUniformLocation(gTableLegsProgramId, "texTableLegs"), 7);
    texNumTableLegs = GL_TEXTURE7;


    // TEXTURE: marble, cream colored
    texFilename = "../../resources/textures/marble-cream.jpg";
    if (!createTexture(texFilename, texLampBottom, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // lamp base
    glUseProgram(gLampBottomProgramId);
    glUniform1i(glGetUniformLocation(gLampBottomProgramId, "texLampBottom"), 8);
    texNumLampBottom = GL_TEXTURE8;
    
    // TEXTURE: cotton fabric, off-white cream color
    texFilename = "../../resources/textures/cotton-seamless.jpg";
    if (!createTexture(texFilename, texLampTop, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // lamp shade
    glUseProgram(gLampTopProgramId);
    glUniform1i(glGetUniformLocation(gLampTopProgramId, "texLampTop"), 9);
    texNumLampTop = GL_TEXTURE9;

    // TEXTURE: smooth rubber
    texFilename = "../../resources/textures/rubber-green.jpg";
    if (!createTexture(texFilename, texBalloons, GL_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // balloons
    glUseProgram(gBalloonsProgramId);
    glUniform1i(glGetUniformLocation(gBalloonsProgramId, "texBalloons"), 10);
    texNumBalloons = GL_TEXTURE10;

    // TEXTURE: red woven fabric
    texFilename = "../../resources/textures/fabric-red-seamless.png";
    if (!createTexture(texFilename, texCouch, GL_MIRRORED_REPEAT, GL_LINEAR))
    {
        cout << "Failed to load texture " << texFilename << endl;
        return EXIT_FAILURE;
    }
    // balloons
    glUseProgram(gCouchProgramId);
    glUniform1i(glGetUniformLocation(gCouchProgramId, "texCouch"), 11);
    texNumCouch = GL_TEXTURE11;


    // render loop. 
    // (exit key is esc, defined at "exit" (at end of main class)
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;
        // input
        keyboardNavigationCallback(gWindow);

        glm::mat4 view = gCamera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

       // creates and draws all objects
        rendering(view, projection);

        // poll for user or OS activity
        glfwPollEvents();
    }


    // release mesh data
    // TODO: look into batch release destroy/release
    destroyMesh(gMeshSideTable);
    destroyMesh(gMeshSideDrawer);
    destroyMesh(gMeshHouseFloor);
    destroyMesh(gMeshHouseWall);
    destroyMesh(gMeshHouseDoor);
    destroyMesh(gMeshPainting);
    destroyMesh(gMeshCoffeeTable);
    destroyMesh(gMeshTableLegs);
    destroyMesh(gMeshLamp);
    destroyMesh(gMeshBalloons);
    destroyMesh(gMeshCouch);
    /*
    destroyMesh(gMeshRug);
    destroyMesh(gMeshHouseWreath);
    destroyMesh(gMeshCouchArmRests);
    */

    // release texture
    //     // TODO: look into batch release destroy/release. 
    // OpenGL 4.4 destroy textures function (don't forget that "s")
    destroyTexture(gTextureId);
    destroyTexture(texSideTable);
    destroyTexture(texSideDrawer);
    destroyTexture(texHouseFloor);
    destroyTexture(texHouseWall);
    destroyTexture(texHouseDoor);
    destroyTexture(texPainting);
    destroyTexture(texWoodSolidDark);
    destroyTexture(texCoffeeTable);
    destroyTexture(texMetalBlack);
    destroyTexture(texTableLegs);
    destroyTexture(texLampBottom);
    destroyTexture(texLampTop);
    destroyTexture(texBalloons);
    destroyTexture(texCouch);
    /*
    destroyTexture(texRug);
    destroyTexture(texWreath);
    */

    // release fragment shader programs
    destroyShaderProgram(gSideTableProgramId);
    destroyShaderProgram(gSideDrawerProgramId);
    destroyShaderProgram(gHouseFloorProgramId);
    destroyShaderProgram(gHouseWallProgramId);
    destroyShaderProgram(gHouseDoorProgramId);
    destroyShaderProgram(gPaintingProgramId);
    destroyShaderProgram(gCoffeeTableProgramId);
    destroyShaderProgram(gTableLegsProgramId);
    destroyShaderProgram(gLampBottomProgramId);
    destroyShaderProgram(gLampTopProgramId);
    destroyShaderProgram(gBalloonsProgramId);
    destroyShaderProgram(gCouchProgramId);
    /*
     destroyShaderProgram(gRugProgramId);
     destroyShaderProgram(gHouseWreathProgramId);
     destroyShaderProgram(gCouchArmRestsProgramId);
     */

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// ********** RENDERING **********
// Functioned called to render a frame
void rendering(glm::mat4 view, glm::mat4 projection)
{

    // Clear the frame and z buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable z-depth
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // move to cube (or remove if cube is deleted)
    // Activate the cube VAO (used by cube)

    // TODO: assign variable to hold texture number
    drawSideTable(view, projection, gSideTableProgramId, gMeshSideTable, texNumSideTable, texSideTable);
    drawSideDrawer(view, projection, gSideDrawerProgramId, gMeshSideDrawer, texNumSideDrawer, texSideDrawer);
    drawHouseFloor(view, projection, gHouseFloorProgramId, gMeshHouseFloor, texNumHouseFloor, texHouseFloor);
    drawHouseWall(view, projection, gHouseWallProgramId, gMeshHouseWall, texNumHouseWall, texHouseWall);
    drawHouseDoor(view, projection, gHouseDoorProgramId, gMeshHouseDoor, texNumHouseDoor, texHouseDoor);
    drawPainting(view, projection, gPaintingProgramId, gMeshPainting, texNumPainting, texPainting);
    drawCoffeeTable(view, projection, gCoffeeTableProgramId, gMeshCoffeeTable, texNumCoffeeTable, texCoffeeTable);
    drawTableLegs(view, projection, gTableLegsProgramId, gMeshTableLegs, texNumTableLegs, texTableLegs);
    drawLampBottom(view, projection, gLampBottomProgramId, gMeshLamp, texNumLampBottom, texLampBottom);
    drawLampTop(view, projection, gLampTopProgramId, gMeshLamp, texNumLampTop, texLampTop);
    drawBalloons(view, projection, gBalloonsProgramId, gMeshBalloons, texNumBalloons, texBalloons);
    drawCouch(view, projection, gCouchProgramId, gMeshCouch, texNumCouch, texCouch);

    
    // Deactivate the Vertex Array Object and shader program
    glBindVertexArray(0);
    glUseProgram(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}


// ********** INTIALIZE OPENGL AND EVENTS **********
// Initialize GLFW, GLEW, and create a window
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


    // tell GLFW to capture our mouse
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(*window, mousePositionCallback);
    glfwSetScrollCallback(*window, mouseScrollCallback);
    glfwSetMouseButtonCallback(*window, mouseButtonCallback);
    glfwSetKeyCallback(*window, keyboardCallback);

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


// ********** KEYBOARD-BASED NAVIGATION **********
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void keyboardNavigationCallback(GLFWwindow* window)
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

// Change projection to ortho
// FIX ME: does not change to ortho. Tried GetKey method, but causes indefinite loop when ortho projection change is added
// source: https://stackoverflow.com/questions/51873906/is-there-a-way-to-process-only-one-input-event-after-a-key-is-pressed-using-glfw
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
        cout << "Switching from perspective to ortho projection" << endl;
    }
}



// ********** MOUSE-BASED NAVIGATION **********
// glfw: mouse movement changes orientation of camera
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


// glfw: mouse scroll wheel increase/decreases movement speed
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}


// glfw: mouse buttons (test mode. only prints text)
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



// ********** TEXTURES AND IMAGES **********
// generates settings for individual texture 
// assumes all images are declared and processed as GL_TEXTURE_2D
bool createTexture(const char* filename, GLuint& textureId, GLint gTexWrapMode, GLint gTexFilterMode)
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

        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

        return true;
    }

    // Error loading the image
    return false;
}


// load all textures
/* IMPORTANT: By default, assumes all images are declared and processed as GL_TEXTURE_2D.
 *    Specify location, textureID, Wrap Parameter and Filter Parameter
 *    To make S or T different from each other, a custom loop needs to be made, based on the createTexture */

 // delete textures. change this to default later
void deleteAllTexture(GLuint& textureIdAll, int texCount)
{
    // uses glDeleteTextures
    // source: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteTextures.xhtml
    glDeleteTextures(texCount, &textureIdAll);
}

// remove once deleteTexture properly functions/enabled
void destroyTexture(GLuint textureId)
{
    glGenTextures(1, &textureId);
}


// ********** SHADERS: CREATE AND DESTROY **********
// Implements the UCreateShaders function
bool createShaderProgram(const char* vtxShaderSource, const char* FragShader, GLuint& programId)
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
    glShaderSource(fragmentShaderId, 1, &FragShader, NULL);

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


void destroyShaderProgram(GLuint programId)
{
    glDeleteProgram(programId);
}

// ********** OBJECT MESH **********

void createMeshSideTable(GLMesh& gMesh)
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

    gMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo); // Activates the buffer
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


void createMeshSideDrawer(GLMesh& gMesh)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // place at front of side table dresser cube
        -0.55f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left bottom
         0.55f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // right bottom
         0.55f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // right top
         0.55f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // right top
        -0.55f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // left top
        -0.55f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f  // left bottom
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo); // Activates the buffer
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


void createMeshHouseFloor(GLMesh& gMesh)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // place at front of side table dresser cube
        -1.0f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // left back
         1.0f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // right back
         1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // right front
         1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // right front
        -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left front
        -1.0f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f  // left back
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo); // Activates the buffer
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


void createMeshHouseWall(GLMesh& gMesh)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // place at front of side table dresser cube
        -1.0f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // left bottom
         1.0f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // right bottom
         1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // right top
         1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // right top
        -1.0f, 1.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // left top
        -1.0f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f  // left bottom
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo); // Activates the buffer
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


void createMeshHouseDoor(GLMesh& gMesh)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // front
        -0.55f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.55f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.55f, 1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.55f, 1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.55f, 1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.55f, 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo); // Activates the buffer
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


void createMeshPainting(GLMesh& gMesh)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // front
         -0.55f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.55f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.55f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.55f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.55f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.55f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo); // Activates the buffer
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


void createMeshCoffeeTable(GLMesh& gMesh)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // front
        -1.0f, -0.25f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -0.25f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  0.25f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  0.25f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  0.25f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -0.25f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        // back
        -1.0f, -0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -0.25f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        // left Coffee
        -1.0f,  0.25f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f,  0.25f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -0.25f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -0.25f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -0.25f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  0.25f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        // right Coffee
         1.0f,  0.25f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  0.25f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -0.25f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -0.25f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -0.25f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         1.0f,  0.25f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         // bottom
        -1.0f, -0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         1.0f, -0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -0.25f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         1.0f, -0.25f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -0.25f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -0.25f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        // top
        -1.0f,  0.25f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         1.0f,  0.25f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  0.25f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         1.0f,  0.25f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f,  0.25f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f,  0.25f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo); // Activates the buffer
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


// mesh for coffee table legs and couch legs (only tube, has no top/bottom)
void createMeshTableLegs(GLMesh& gMesh)
{
    // source: https://stackoverflow.com/questions/59468388/how-the-heck-do-you-use-gl-triangle-fan-to-draw-a-circle-in-opengl
    // coordinate calculations: https://www.sr-research.com/circular-coordinate-calculator/

    GLfloat verts[] = {
        // Vertex Positions       // normals         // textures
        // top
         0.000f,  1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 0, center top center

         0.813f,  1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 1, top right middle - DONE
         0.704f,  1.0f,  0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 2, top right front - DONE
         0.406f,  1.0f,  0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 3, top right front
         0.000f,  1.0f,  0.813f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 4, top middle front - done
        -0.406f,  1.0f,  0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 5, top left front - p
        -0.704f,  1.0f,  0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 6, top left front - done
        -0.813f,  1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 7, top left middle -p
        -0.704f,  1.0f, -0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 8, top left back - done
        -0.406f,  1.0f, -0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 9, top left back  
         0.000f,  1.0f, -0.813f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 10, top middle back - p
         0.406f,  1.0f, -0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 11, top right back
         0.704f,  1.0f, -0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 12, top right back - done
         // bottom
         0.813f, -1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 13 (1), bottom right middle
         0.704f, -1.0f,  0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 14 (2), bottom right front - DONE
         0.406f, -1.0f,  0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 15 (3), bottom right front - DONE
         0.000f, -1.0f,  0.813f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 16 (4), bottom middle front
        -0.406f, -1.0f,  0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 17 (5), bottom left front   (0.0, 0.1)
        -0.704f, -1.0f,  0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 18 (6), bottom left front
        -0.813f, -1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 19 (7), bottom left middle
        -0.704f, -1.0f, -0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 20 (8), bottom left back
        -0.406f, -1.0f, -0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 21 (9), bottom left back
         0.000f, -1.0f, -0.813f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 22 (10), bottom middle back
         0.406f, -1.0f, -0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 23 (11), bottom right back
         0.704f, -1.0f, -0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 24 (12), bottom right back

         0.000f, -1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 25 (0), center bottom center

    };

    // displays cylinder tube only
    // top and bottom are NOT needed for display. 
    GLushort indices[] = {
        /*
        // minimizes number of triangles needed for top and bottom
        // top
         4,  8, 12, // top large center
        12,  2,  4, // top medium left
         4,  6,  8, // top medium right
         8, 10, 12, // top medium front
        12,  1,  2, // top small center right
         2,  3,  4, // top small back right
         4,  5,  6, // top small back left
         6,  7,  8, // top small center left
         8,  9, 10, // top small front left
        10, 11, 12, // top small front right
        
        // bottom
        16, 20, 24, // bottom large center
        24, 14, 16, // bottom medium left
        16, 18, 20, // bottom medium right
        20, 22, 24, // bottom medium front
        24, 13, 14, // bottom small center right
        14, 15, 16, // bottom small back right
        16, 17, 18, // bottom small back left
        18, 19, 20, // bottom small center left
        20, 21, 22, // bottom small front left
        22, 23, 24,  // bottom small front right
        */
        // TUBE
        // back right panels
         1,  2, 13, // top 
        13, 14,  2, // bottom
         2,  3, 14, // top
        14, 15,  3, // bottom
         3,  4, 15, // top
        15, 16,  4,
        // back left panels
         4,  5, 16, // top
        16, 17,  5, // bottom
         5,  6, 17, // top
        17, 18,  6, // bottom
         6,  7, 18, // top
        18, 19,  7, // bottom
        // front left panels
         7,  8, 19, // top
        19, 20,  8, // bottom
         8,  9, 20, // top
        20, 21,  9, // bottom
         9, 10, 21, // top
        21, 22, 10,
        // front right panels
        10, 11, 22, // top 
        22, 23, 11, // bottom
        11, 12, 23, // top
        23, 24, 12, // bottom
        12,  1, 24, // top
        24, 13,  1  // bottom
    };
    

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;


    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);


    gMesh.nIndices = sizeof(indices) / (sizeof(indices[0]));
    glGenBuffers(2, gMesh.vbos); // Create 2 buffers: vertex data and the indices
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gMesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}

// mesh for lamp, both top and bottom
void createMeshLamp(GLMesh& gMesh)
{

    // source: https://stackoverflow.com/questions/59468388/how-the-heck-do-you-use-gl-triangle-fan-to-draw-a-circle-in-opengl
    // coordinate calculations: https://www.sr-research.com/circular-coordinate-calculator/

    GLfloat verts[] = {
        // Vertex Positions       // normals         // textures
        // top center point
         0.000f,  1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 0, center top center
        // top
         0.813f,  1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 1, top right middle - DONE
         0.704f,  1.0f,  0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 2, top right front - DONE
         0.406f,  1.0f,  0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 3, top right front
         0.000f,  1.0f,  0.813f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 4, top middle front - done
        -0.406f,  1.0f,  0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 5, top left front - p
        -0.704f,  1.0f,  0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 6, top left front - done
        -0.813f,  1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 7, top left middle -p
        -0.704f,  1.0f, -0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 8, top left back - done
        -0.406f,  1.0f, -0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 9, top left back  
         0.000f,  1.0f, -0.813f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 10, top middle back - p
         0.406f,  1.0f, -0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 11, top right back
         0.704f,  1.0f, -0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 12, top right back - done
         
        // bottom
         0.813f, -1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 13 (1), bottom right middle
         0.704f, -1.0f,  0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 14 (2), bottom right front - DONE
         0.406f, -1.0f,  0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 15 (3), bottom right front - DONE
         0.000f, -1.0f,  0.813f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 16 (4), bottom middle front
        -0.406f, -1.0f,  0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 17 (5), bottom left front   (0.0, 0.1)
        -0.704f, -1.0f,  0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 18 (6), bottom left front
        -0.813f, -1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 19 (7), bottom left middle
        -0.704f, -1.0f, -0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 20 (8), bottom left back
        -0.406f, -1.0f, -0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 21 (9), bottom left back
         0.000f, -1.0f, -0.813f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 22 (10), bottom middle back
         0.406f, -1.0f, -0.704f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 23 (11), bottom right back
         0.704f, -1.0f, -0.406f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 24 (12), bottom right back
        // bottom center point
         0.000f, -1.0f,  0.000f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 25 (0), center bottom center

    };

    // Index data to share position data of cylinder
    // top and bottom are "triangle fan" for a better texture display
    GLushort indices[] = {
        
        // top
        0, 1, 2, 
        0, 2, 3,
        0, 3, 4, 
        0, 4, 5, 
        0, 5, 6, 
        0, 6, 7, 
        0, 7, 8, 
        0, 8, 9,
        0, 9, 10,
        0, 10, 11,
        0, 11, 12,
        0, 12, 1,
        // bottom
        25, 13, 14,
        25, 14, 15,
        25, 15, 16,
        25, 16, 17,
        25, 17, 18,
        25, 18, 19,
        25, 19, 20,
        25, 20, 21,
        25, 21, 22,
        25, 22, 23,
        25, 23, 24,
        25, 24, 13,
        // TUBE
        // back right panels
         1,  2, 13, // top 
        13, 14,  2, // bottom
         2,  3, 14, // top
        14, 15,  3, // bottom
         3,  4, 15, // top
        15, 16,  4,
        // back left panels
         4,  5, 16, // top
        16, 17,  5, // bottom
         5,  6, 17, // top
        17, 18,  6, // bottom
         6,  7, 18, // top
        18, 19,  7, // bottom
        // front left panels
         7,  8, 19, // top
        19, 20,  8, // bottom
         8,  9, 20, // top
        20, 21,  9, // bottom
         9, 10, 21, // top
        21, 22, 10,
        // front right panels
        10, 11, 22, // top 
        22, 23, 11, // bottom
        11, 12, 23, // top
        23, 24, 12, // bottom
        12,  1, 24, // top
        24, 13,  1  // bottom
    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;


    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);


    gMesh.nIndices = sizeof(indices) / (sizeof(indices[0]));
    glGenBuffers(2, gMesh.vbos); // Create 2 buffers: vertex data and the indices
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gMesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerNormal + floatsPerUV);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* floatsPerVertex));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float)* (floatsPerVertex + floatsPerNormal)));
    glEnableVertexAttribArray(2);
}


void createMeshBalloons(GLMesh& gMesh)
{
    // coordinate calculations: https://www.sr-research.com/circular-coordinate-calculator/

    GLfloat verts[] = {
        // Vertex Positions       // normals         // textures
        // top center point
         0.000f,  0.75f,  0.000f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 0, center top center
        // top
         0.579f,  0.5f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 1, top right middle
         0.501f,  0.5f,  0.289f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 2, top right front 
         0.289f,  0.5f,  0.501f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 3, top right front
         0.000f,  0.5f,  0.579f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 4, top middle front
        -0.289f,  0.5f,  0.501f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 5, top left front 
        -0.501f,  0.5f,  0.289f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 6, top left front
        -0.579f,  0.5f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 7, top left middle
        -0.501f,  0.5f, -0.289f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 8, top left back
        -0.289f,  0.5f, -0.501f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 9, top left back  
         0.000f,  0.5f, -0.579f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 10, top middle back
         0.289f,  0.5f, -0.501f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 11, top right back
         0.501f,  0.5f, -0.289f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 12, top right back

        // bottom
         0.579f,  -0.5f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 13 (1), bottom right middle
         0.501f,  -0.5f,  0.289f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 14 (2), bottom right front
         0.289f,  -0.5f,  0.501f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 15 (3), bottom right front
         0.000f,  -0.5f,  0.579f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 16 (4), bottom middle front
        -0.289f,  -0.5f,  0.501f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 17 (5), bottom left front
        -0.501f,  -0.5f,  0.289f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 18 (6), bottom left front
        -0.579f,  -0.5f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 19 (7), bottom left middle
        -0.501f,  -0.5f, -0.289f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 20 (8), bottom left back
        -0.289f,  -0.5f, -0.501f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 21 (9), bottom left back
         0.000f,  -0.5f, -0.579f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 22 (10), bottom middle back
         0.289f,  -0.5f, -0.501f,  0.0f, 0.0f, 0.1f,  1.0f, 1.0f, // 23 (11), bottom right back
         0.501f,  -0.5f, -0.289f,  0.0f, 0.0f, 0.1f,  0.0f, 1.0f, // 24 (12), bottom right back
        
		//center
         0.965f,  0.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 25 (13, 1), center right middle
         0.835f,  0.0f,  0.482f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 26 (14, 2), center right front
         0.482f,  0.0f,  0.835f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 27 (15, 3), center right front
         0.000f,  0.0f,  0.965f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 28 (16, 4), center middle front
        -0.482f,  0.0f,  0.835f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 29 (17, 5), center left front
        -0.835f,  0.0f,  0.482f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 30 (18, 6), center left front
        -0.965f,  0.0f,  0.000f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 31 (19, 7), center left middle
        -0.835f,  0.0f, -0.482f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 32 (20, 8), center left back
        -0.482f,  0.0f, -0.835f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 33 (21, 9), center left back
         0.000f,  0.0f, -0.965f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 34 (22, 10), center middle back
         0.482f,  0.0f, -0.835f,  0.0f, 0.0f, 0.1f,  1.0f, 0.0f, // 35 (23, 11), center right back
         0.835f,  0.0f, -0.482f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f, // 36 (24, 12), center right back

        // bottom center point
         0.000f, -0.75f,  0.000f,  0.0f, 0.0f, 0.1f,  0.0f, 0.0f // 37 (0), center bottom center

    };

    // Index data to share position data of cylinder
    // top and bottom are "triangle fan" for a better texture display
    GLushort indices[] = {
        
        // top
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 9,
        0, 9, 10,
        0, 10, 11,
        0, 11, 12,
        0, 12, 1,
		
        // bottom
        37, 13, 14,
        37, 14, 15,
        37, 15, 16,
        37, 16, 17,
        37, 17, 18,
        37, 18, 19,
        37, 19, 20,
        37, 20, 21,
        37, 21, 22,
        37, 22, 23,
        37, 23, 24,
        37, 24, 13,
        
        // mid-upper tube
        // upper back right panels
         1,  2, 25, // top 
        25, 26,  2, // bottom
         2,  3, 26, // top
        26, 27,  3, // bottom
         3,  4, 27, // top
        27, 28,  4,
        // upper back left panels
         4,  5, 28, // top
        28, 29,  5, // bottom
         5,  6, 29, // top
        29, 30,  6, // bottom
         6,  7, 30, // top
        30, 31,  7, // bottom
        // upper front left panels
         7,  8, 31, // top
        31, 32,  8, // bottom
         8,  9, 32, // top
        32, 33,  9, // bottom
         9, 10, 33, // top
        33, 34, 10,
        // upper front right panels
        10, 11, 34, // top 
        34, 35, 11, // bottom
        11, 12, 35, // top
        35, 36, 12, // bottom
        12,  1, 36, // top
        36, 25,  1,  // bottom
		
		// mid-lower tube
        // lower back right panels
        25, 26, 13,
        13, 14, 26,
        26, 27, 14,
        14, 15, 27,
        27, 28, 15,
        15, 16, 28,        
        // lower back left panels
        28, 29, 16, 
        16, 17, 29, 
        29, 30, 17, 
        17, 18, 30, 
        30, 31, 18, 
        18, 19, 31, 
        // lower front left panels
        31, 32, 19, 
        19, 20, 32, 
        32, 33, 20, 
        20, 21, 33, 
        33, 34, 21, 
        21, 22, 34, 
        // lower front right panels
        34, 35, 22, 
        22, 23, 35, 
        35, 36, 23, 
        23, 24, 36,
        36, 25, 24,
        24, 13, 25,
    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;


    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);


    gMesh.nIndices = sizeof(indices) / (sizeof(indices[0]));
    glGenBuffers(2, gMesh.vbos); // Create 2 buffers: vertex data and the indices
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gMesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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


void createMeshCouch(GLMesh& gMesh)
{
    GLfloat verts[] = {
        // Vertex Positions    // normals  // textures
        // front
        -1.0f, -0.2f,  0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -0.2f,  0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  0.2f,  0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  0.2f,  0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  0.2f,  0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -0.2f,  0.75f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        // back
        -1.0f, -0.2f, -0.75f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -0.2f, -0.75f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  0.2f, -0.75f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  0.2f, -0.75f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  0.2f, -0.75f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -0.2f, -0.75f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        // left Coffee
        -1.0f,  0.2f,  0.75f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f,  0.2f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -0.2f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -0.2f, -0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -0.2f,  0.75f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  0.2f,  0.75f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        // right Coffee
         1.0f,  0.2f,  0.75f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  0.2f, -0.75f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -0.2f, -0.75f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -0.2f, -0.75f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -0.2f,  0.75f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         1.0f,  0.2f,  0.75f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         // bottom
        -1.0f, -0.2f, -0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         1.0f, -0.2f, -0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -0.2f,  0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         1.0f, -0.2f,  0.75f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -0.2f,  0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -0.2f, -0.75f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        // top
        -1.0f,  0.2f, -0.75f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         1.0f,  0.2f, -0.75f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         1.0f,  0.2f,  0.75f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         1.0f,  0.2f,  0.75f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -1.0f,  0.2f,  0.75f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f,  0.2f, -0.75f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };


    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerNormal = 3;
    const GLuint floatsPerUV = 2;

    gMesh.nVertices = sizeof(verts) / (sizeof(verts[0]) * (floatsPerVertex + floatsPerNormal + floatsPerUV));

    glGenVertexArrays(1, &gMesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(gMesh.vao);

    // buffer for vertex data
    glGenBuffers(1, &gMesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gMesh.vbo); // Activates the buffer
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


void destroyMesh(GLMesh& gMesh)
{
    glDeleteVertexArrays(1, &gMesh.vao);
    glDeleteBuffers(1, &gMesh.vbo);
}


// ********** DRAW OBJECTS **********

void drawSideTable(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    glUseProgram(shaderProgramID);

    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));


    // ********** dresser main cuboid ************************
    // dresser cuboid - 1 per table, currently 2 tables
    // create model view: scale, rotate, translate
    glm::mat4 scale = glm::scale(glm::vec3(1.3f, 1.2f, 1.3f));

    glm::vec3 dresserPosition[] = {
        // 1st dresser (A/left)
        glm::vec3(-5.0f, 0.8f, 1.5f), // dresser A: left
        // 2nd dresser (B/right)
        glm::vec3(2.5f, 0.8f, 1.5f) // dresser B: right
    };

    // counts the number of dressers
    int dresserCount = sizeof(dresserPosition) / sizeof(dresserPosition[0]);

    // draws each leg
    for (unsigned int i = 0; i < dresserCount; i++)
    {
        // recalculates model matrix with new position
        glm::mat4 translation = glm::translate(glm::vec3(dresserPosition[i]));
        glm::vec2 gUVScale(0.50f, 0.50f);

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 model = translation * rotation * scale;

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleSideTable");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMesh.vao);

        // bind textures on corresponding texture units
        glActiveTexture(textureNum);
        glBindTexture(GL_TEXTURE_2D, textureName);

        glBindVertexArray(gMesh.vao);

        glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
    }

    // ********** side table legs (4 per table) ************************
    scale = glm::scale(glm::vec3(0.15f, 0.3f, 0.2f));
    // legs uses same rotation as dresser cuboid.
    glm::vec2 gUVScaleLegs(0.25f, 0.25f);

    // transition / each leg has a unique position
    glm::vec3 legPosition[] = {
        // 1st dresser (A/left)
        glm::vec3(-4.5f, 0.1f, 1.0f), // A: right front leg
        glm::vec3(-5.5f, 0.1f, 1.0f), // A: left front leg
        glm::vec3(-4.5f, 0.1f, 2.0f), // A: right back leg
        glm::vec3(-5.5f, 0.1f, 2.0f), // A: left back leg
        // 2nd dresser (B/right)
        glm::vec3( 2.0f, 0.1f, 1.0f), // B: right front leg
        glm::vec3( 3.0f, 0.1f, 1.0f), // B: left front leg
        glm::vec3( 2.0f, 0.1f, 2.0f), // B: right back leg
        glm::vec3( 3.0f, 0.1f, 2.0f), // B: left back leg
    };

    // counts the number of legs
    int legCount = sizeof(legPosition) / sizeof(legPosition[0]);

    // draws each leg
    for (unsigned int i = 0; i < legCount; i++)
    {
        // recalculates model matrix with new position
        glm::mat4 translation = glm::translate(glm::vec3(legPosition[i]));
        glm::mat4 model = translation * rotation * scale;

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        // UV scale for dresser legs
        GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleSideLegs");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScaleLegs));

        // Activate the VBOs contained within the mesh's VAO
        glBindVertexArray(gMesh.vao);

        // bind textures on corresponding texture units
        glActiveTexture(textureNum);
        glBindTexture(GL_TEXTURE_2D, textureName);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
    }
}


//IMPORTANT: remember to keep in-sync (positioning) with side tables (drawSideTable)
void drawSideDrawer(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    glUseProgram(shaderProgramID);

    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.3f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::vec2 gUVScale(1.0f, 1.00f);

    glm::vec3 dresserPosition[] = {
        // 1st dresser (A/left)
        glm::vec3(-5.0f, 0.8f, 1.51f), // dresser A: left
        // 2nd dresser (B/right)
        glm::vec3( 2.5f, 0.8f, 1.51f), // dresser B: right
    };

    // counts the number of objects
    int dresserCount = sizeof(dresserPosition) / sizeof(dresserPosition[0]);

    // draws each leg
    for (unsigned int i = 0; i < dresserCount; i++)
    {
        glm::mat4 translation = glm::translate(glm::vec3(dresserPosition[i]));

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 model = translation * rotation * scale;

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
 
        GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleSideDrawer");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(textureNum);
        glBindTexture(GL_TEXTURE_2D, textureName);

        // Activate the VBOs contained within the mesh's VA
        glBindVertexArray(gMesh.vao);

        // Draws the triangles
        glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
    }
}


void drawHouseFloor(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    glm::mat4 scale = glm::scale(glm::vec3(20.0f, 0.2f, 20.0f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 10.0f));
    glm::vec2 gUVScale(5.0f, 4.00f);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Set the shader to be used
    glUseProgram(shaderProgramID);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleHouseFloor");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    //  glBindTexture(GL_TEXTURE_2D, texWoodHerring);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
}


void drawHouseWall(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    glm::mat4 scale = glm::scale(glm::vec3(20.0f, 15.0f, -0.1f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec2 gUVScale(40.0f, 20.0f);


    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Set the shader to be used
    glUseProgram(shaderProgramID);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);


    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleHouseWall");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
}


void drawHouseDoor(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    glm::mat4 scale = glm::scale(glm::vec3(2.25f, 5.0f, 0.0f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(5.0f, -0.1f, 0.1f));
    glm::vec2 gUVScale(1.0f, 1.0f);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Set the shader to be used
    glUseProgram(shaderProgramID);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleHouseDoor");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
}


void drawPainting(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    //painting
    glm::mat4 scale = glm::scale(glm::vec3(2.9f, 2.3f, 0.1f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(-1.4f, 4.0f, 0.1f));
    glm::vec2 gUVScale(1.0f, 1.0f);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Set the shader to be used
    glUseProgram(shaderProgramID);

    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScalePainting");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);
}


void drawCoffeeTable(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{

    // ********** TOP of coffee table ******************
    // create model view: scale, rotate, translate
    glm::mat4 scale = glm::scale(glm::vec3(2.0f, 0.5f, 1.3f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(-1.25f, 0.8f, 5.5f));
    glm::vec2 gUVScale(2.0f, 0.25f);


    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Set the shader to be used
    glUseProgram(shaderProgramID);

    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
    
    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleCoffeeTable");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // Activate the VBOs contained within the mesh's VAO
    glBindVertexArray(gMesh.vao);

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    glBindVertexArray(gMesh.vao);

    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

}


// draw coffee table legs, couch legs, balloon string
void drawTableLegs(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    // Set the shader to be used
    glUseProgram(shaderProgramID);
    
    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
    
   
    // ********* COFFEE TABLE LEGS (4) *********
    glm::mat4 scale = glm::scale(glm::vec3(0.1f, 0.5f, 0.1f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::vec2 gUVScale(1.0f, 1.0f);
    
    // transition. each leg has a unique position
    glm::vec3 legPositionCoffee[] = {
        // 1st dresser
        glm::vec3(-3.0f, 0.3f, 5.2f), // right front leg
        glm::vec3( 0.5f, 0.3f, 5.2f), // left front leg
        glm::vec3(-3.0f, 0.3f, 5.8f), // right back leg
        glm::vec3( 0.5f, 0.3f, 5.8f) // left back leg
    };

    // counts the number of objects
    int legCount = sizeof(legPositionCoffee) / sizeof(legPositionCoffee[0]);

    // draws each leg
    for (unsigned int i = 0; i < legCount; i++)
    {
        glm::mat4 translation = glm::translate(glm::vec3(legPositionCoffee[i]));

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 model = translation * rotation * scale;

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleTableLegs");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(textureNum);
        glBindTexture(GL_TEXTURE_2D, textureName);

        // Activate the VBOs contained within the mesh's VA
        glBindVertexArray(gMesh.vao);

        // Draws the triangles
        glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle
    }

        // ********* COFFEE TABLE LEGS (4) *********
    scale = glm::scale(glm::vec3(0.15f, 0.5f, 0.15f));
    
    // transition. each leg has a unique position
    glm::vec3 legPositionCouch[] = {
        // 1st dresser
        glm::vec3(-3.25f, 0.3f, 0.51f), // right front leg
        glm::vec3( 0.75f, 0.3f, 0.51f), // left front leg
        glm::vec3(-3.25f, 0.3f, 2.51f), // right back leg
        glm::vec3( 0.75f, 0.3f, 2.51f) // left back leg
    };

    // counts the number of objects
    legCount = sizeof(legPositionCouch) / sizeof(legPositionCouch[0]);

    // draws each leg
    for (unsigned int i = 0; i < legCount; i++)
    {
        glm::mat4 translation = glm::translate(glm::vec3(legPositionCouch[i]));

        // Model matrix: transformations are applied right-to-left order
        glm::mat4 model = translation * rotation * scale;

        // Retrieves and passes transform matrices to the Shader program
        GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleTableLegs");
        glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

        // bind textures on corresponding texture units
        glActiveTexture(textureNum);
        glBindTexture(GL_TEXTURE_2D, textureName);

        // Activate the VBOs contained within the mesh's VA
        glBindVertexArray(gMesh.vao);

        // Draws the triangles
        glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle
    }


    // ********* Balloon string *********
    scale = glm::scale(glm::vec3(0.01f, 1.1f, 0.01f));
    rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(2.5f, 2.0f, 1.6f));

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleTableLegs");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle
}


// draws lamp bottom and balloon holder
void drawLampBottom(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    // Set the shader to be used
    glUseProgram(shaderProgramID);

    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    // ********* LAMP BOTTOM *********
    glm::mat4 scale = glm::scale(glm::vec3(0.25f, 0.5f, 0.25f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(-5.0f, 1.85f, 1.5f));
    glm::vec2 gUVScale(1.25f, 1.5f);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleLampBottom");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle


    // ********* BALLOON HOLDER *********
    scale = glm::scale(glm::vec3(0.2f, 0.1f, 0.2f));
    rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    translation = glm::translate(glm::vec3(2.5f, 1.32f, 1.6f));

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    modelLoc = glGetUniformLocation(shaderProgramID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleLampBottom");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle
}


void drawLampTop(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    glm::mat4 scale = glm::scale(glm::vec3(0.6f, 0.4f, 0.6f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(-5.0f, 2.5f, 1.5f));
    glm::vec2 gUVScale(0.4f, 0.6f);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Set the shader to be used
    glUseProgram(shaderProgramID);

    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleLampTop");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

}


void drawBalloons(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    glm::mat4 scale = glm::scale(glm::vec3(0.4f, 0.6f, 0.4f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(2.5f, 3.5f, 1.6f));
    glm::vec2 gUVScale(1.0f, 1.0f);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;

    // Set the shader to be used
    glUseProgram(shaderProgramID);

    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
    
    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleBalloons");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawElements(GL_TRIANGLES, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle

}


void drawCouch(glm::mat4 view, glm::mat4 projection, GLuint shaderProgramID, GLMesh& gMesh, GLenum textureNum, GLuint textureName)
{
    // Set the shader to be used
    glUseProgram(shaderProgramID);

    GLint viewLoc = glGetUniformLocation(shaderProgramID, "view");
    GLint projLoc = glGetUniformLocation(shaderProgramID, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
    GLint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPos");
    GLint viewPositionLoc = glGetUniformLocation(shaderProgramID, "viewPosition");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(lightColorLoc, gLightColor.r, gLightColor.g, gLightColor.b);
    glUniform3f(lightPositionLoc, gLightPosition.x, gLightPosition.y, gLightPosition.z);
    const glm::vec3 cameraPosition = gCamera.Position;
    glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

    // ********* COUCH SEATS *********
    glm::mat4 scale = glm::scale(glm::vec3(2.5f, 1.5f, 1.5f));
    glm::mat4 rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    glm::mat4 translation = glm::translate(glm::vec3(-1.25f, 0.8f, 1.51f));
    glm::vec2 gUVScale(3.0f, 1.0f);

    // Model matrix: transformations are applied right-to-left order
    glm::mat4 model = translation * rotation * scale;


    // Retrieves and passes transform matrices to the Shader program
    GLint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    GLint UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleCouch");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    //  glBindTexture(GL_TEXTURE_2D, texWoodHerring);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

    // Draws the triangles

  
    // ********* COUCH BACK REST *********
    scale = glm::scale(glm::vec3(2.5f, 4.5f, 0.5f));
    rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.1f, 0.0f));
    translation = glm::translate(glm::vec3(-1.25f, 1.5f, 0.75f));

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;

    // Retrieves and passes transform matrices to the Shader program
    modelLoc = glGetUniformLocation(shaderProgramID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    UVScaleLoc = glGetUniformLocation(shaderProgramID, "gUVScaleCouch");
    glUniform2fv(UVScaleLoc, 1, glm::value_ptr(gUVScale));

    // bind textures on corresponding texture units
    glActiveTexture(textureNum);
    //  glBindTexture(GL_TEXTURE_2D, texWoodHerring);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);

    // Draws the triangles
    glDrawArrays(GL_TRIANGLES, 0, gMesh.nVertices);

}
