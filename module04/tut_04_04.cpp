// UPDATED CAMERA.H FILE REQUIRED.
#include <iostream>             // cout, cerr
#include <cstdlib>              // EXIT_FAILURE
#include <GL/glew.h>            // GLEW library
#include <GLFW/glfw3.h>         // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnOpengl/camera.h> // Camera class

using namespace std; // Standard namespace

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Unnamed namespace
namespace
{
    const char* const WINDOW_TITLE = "Katie'S Assignment"; // Macro for window title

    // Variables for window width and height
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Stores the GL data relative to a given mesh
    struct GLMesh
    {
        GLuint vao;         // Handle for the vertex array object
        GLuint vbos[2];     // Handles for the vertex buffer objects
        GLuint vbo;
        GLuint nIndices;    // Number of indices of the mesh
    };

    // Main GLFW window
    GLFWwindow* gWindow = nullptr;
    // Triangle mesh data
    GLMesh gMesh;
    // Shader program
    GLuint gProgramId;

    // camera
    Camera gCamera(glm::vec3(0.0f, 0.0f, 7.0f));
    float gLastX = WINDOW_WIDTH / 2.0f;
    float gLastY = WINDOW_HEIGHT / 2.0f;
    bool gFirstMouse = true;

    // timing
    float gDeltaTime = 0.0f; // time between current frame and last frame
    float gLastFrame = 0.0f;

}

/* User-defined Function prototypes to:
 * initialize the program, set the window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
bool UInitialize(int, char* [], GLFWwindow** window);
void UResizeWindow(GLFWwindow* window, int width, int height);
void UProcessKeyboard(GLFWwindow* window);
void switchOrthoPerspective(GLFWwindow* window, int key, int scancode, int action, int mods);
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void UDestroyMesh(GLMesh& mesh);
bool UCreateShaderProgram(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void UDestroyShaderProgram(GLuint programId);

void enableView(GLFWwindow* window);
void URender_SideTable();
void URender_Background();
void UCreateMesh_SideTable(GLMesh& mesh);
void UCreateMesh_Background(GLMesh& mesh);


/* Vertex Shader Source Code*/
const GLchar* vertexShaderSource = GLSL(440,
    layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
layout(location = 1) in vec4 color;  // Color data from Vertex Attrib Pointer 1

out vec4 vertexColor; // variable to transfer color data to the fragment shader

//Global variables for the  transform matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
    vertexColor = color; // references incoming color data
}
);


/* Fragment Shader Source Code*/
const GLchar* fragmentShaderSource = GLSL(440,
    in vec4 vertexColor; // Variable to hold incoming color data from vertex shader

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(vertexColor);
}
);


int main(int argc, char* argv[])
{
    if (!UInitialize(argc, argv, &gWindow))
        return EXIT_FAILURE;

    // Create the mesh
   // UCreateMesh(gMesh); // Calls the function to create the Vertex Buffer Object
    UCreateMesh_SideTable(gMesh);
    UCreateMesh_Background(gMesh);


    // Create the shader program
    if (!UCreateShaderProgram(vertexShaderSource, fragmentShaderSource, gProgramId))
        return EXIT_FAILURE;

    // Sets the background color of the window to black (it will be implicitely used by glClear)
    // Converts standard RGB decimal colors (r, g, b, a) into floats.
    // RGB = (1.0f/255.0) * decimalCode; It doesn't hurt to add .0 to the end
    float red = (1.0f / 255.0) * 55.0;
    float grn = (1.0f / 255.0) * 55.0;
    float blu = (1.0f / 255.0) * 255.0;
    glClearColor(red, grn, blu, 1.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(gWindow))
    {
        // per-frame timing
        // --------------------
        float currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - gLastFrame;
        gLastFrame = currentFrame;

        // input
        // -----
        UProcessKeyboard(gWindow);

        // enable and adjust view
        enableView(gWindow);
        // Render this frame
        //URender();
        URender_SideTable();
        URender_Background();
        glfwPollEvents();
    }

    // Release mesh data
    UDestroyMesh(gMesh);

    // Release shader program
    UDestroyShaderProgram(gProgramId);

    exit(EXIT_SUCCESS); // Terminates the program successfully
}


// Initialize GLFW, GLEW, and create a window
bool UInitialize(int argc, char* argv[], GLFWwindow** window)
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
    glfwSetFramebufferSizeCallback(*window, UResizeWindow);
    glfwSetCursorPosCallback(*window, UMousePositionCallback);
    glfwSetScrollCallback(*window, UMouseScrollCallback);
    glfwSetMouseButtonCallback(*window, UMouseButtonCallback);


    // tell GLFW to capture mouse
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



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void UProcessKeyboard(GLFWwindow* window)
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



// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void UMousePositionCallback(GLFWwindow* window, double xpos, double ypos)
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
void UMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    gCamera.ProcessMouseScroll(yoffset);
}

// glfw: handle mouse button events
// --------------------------------
void UMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void UResizeWindow(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


// switch to Ortho project upon keyboard input
void switchOrthoPerspective(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        std::cout << "Space Key Pressed" << std::endl;
    }

    // source: https://www.glfw.org/docs/3.3/input_guide.html#input_key
    // Creates a ortho projection
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        // if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        //gCamera.ProcessKeyboard(DOWN, gDeltaTime);
        //glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    }
}

void enableView(GLFWwindow* window)
{    // Enable z-depth
    glEnable(GL_DEPTH_TEST);

    // Clear the frame and z buffers
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transforms the camera: move the camera
    // stationary glm::mat4 view = glm::translate(glm::vec3(0.0f, 0.0f, -5.0f));
    // camera/view transformation
    glm::mat4 view = gCamera.GetViewMatrix();
    GLint viewLoc = glGetUniformLocation(gProgramId, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // defaulted to perspective projection
    glm::mat4 projection = glm::perspective(glm::radians(gCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

    // switched between Ortho and Perspective projection via key callback
    // source: https://www.glfw.org/docs/latest/input_guide.html#input_keyboard
    /* -----FIX ME----- "switches between Ortho and Perspective projection via key callback"
     * ISSUE: CALLBACK IS NOT RESPONDING PROPERLY.
     * calls on OrthoPerspective - testing by pressing "space" and looking at the log
     * however, does not response to key press "p" to change and save view
     * key press "P" works in Enable function, but only when P is held.
     * this issue was supposed to resolved by using a callback.
     * switched between Ortho and Perspective projection via key callback
     * ATTEMPTED: tried switching views - no changes tried if/else loop - invalid method
     * RESEARCH: Stackoverflow, OpenGL documents, GLFW documents, how-tos.
     * unable to find materials that matches situation. some material too complex for me at this time
     */
     // switches between Ortho and Perspective projection via key callback
    glfwSetKeyCallback(window, switchOrthoPerspective);

    /* code does responsed, but only when holding key P. Uncomment to test
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    }
    */




    GLint projLoc = glGetUniformLocation(gProgramId, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the shader to be used
    glUseProgram(gProgramId);
};


// Function called to render a frame
void URender_SideTable()
{

    // declare objects
    glm::mat4 scale;
    glm::mat4 rotation;
    glm::mat4 translation;
    glm::mat4 model = translation * rotation * scale;;
    GLint modelLoc;

    // **********************************
   // dresser cuboid
   // create model view: scale, rotate, translate
    scale = glm::scale(glm::vec3(1.3f, 1.2f, 1.3f));
    rotation = glm::rotate(0.0f, glm::vec3(0.0f, 0.5f, 0.0f));
    translation = glm::translate(glm::vec3(-4.6f, -1.5f, -1.6f));

    // Model matrix: transformations are applied right-to-left order
    model = translation * rotation * scale;
    // Retrieves and passes transform matrices to the Shader program
    modelLoc = glGetUniformLocation(gProgramId, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Activate the VBOs contained within the mesh's VA
    glBindVertexArray(gMesh.vao);
    // draws primary dresser cube
    glDrawElements(GL_TRIANGLE_STRIP, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle


    // **********************************
    // small legs (4) cuboid
    // uses same rotation as dresser cuboid. does not need to be redefined

    // scale for legs (uniform size for all 4 legs)
    scale = glm::scale(glm::vec3(0.15f, 0.4f, 0.2f));

    // each leg has a unique position
    glm::vec3 legPosition[] = {
    glm::vec3(-3.5f, -1.8f, -0.6f), // right front leg
    glm::vec3(-4.5f, -1.8f, -0.6f), // left front leg
    glm::vec3(-3.5f, -1.8f, -1.6f), // right back leg
    glm::vec3(-4.5f, -1.8f, -1.6f) // left back leg
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
        modelLoc = glGetUniformLocation(gProgramId, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(gMesh.vao);

        // draws each legasdfasdf
        glDrawElements(GL_TRIANGLE_STRIP, gMesh.nIndices, GL_UNSIGNED_SHORT, NULL); // Draws the triangle
    }

    //----------------------------------------------------------------
    // Deactivate the Vertex Array Object
    glBindVertexArray(0);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    glfwSwapBuffers(gWindow);    // Flips the the back buffer with the front buffer every frame.
}


void URender_Background()
{

}

// Implements the UCreateMesh function
void UCreateMesh_SideTable(GLMesh& mesh)
{
    // color conversion formula
    //converts RGB decimal colors into floats.
    // Colors(r, g, b, a) enter as decimal code 0.0 - 255.0 (make sure to include the .0 at the end)
    float dec = (1.0f / 255.0);


    // Position and Color data
    GLfloat verts[] = {
        // CUBE/cuboid  
        // SOURCE: Triangle strips https://stackoverflow.com/questions/28375338/cube-using-single-gl-triangle-strip
        // Vertex Positions    // Colors (r,g,b,a) enter as decimal code 0-255
        1.0f,  1.0f, 1.0f,   0 * dec, 1 * dec, 73 * dec, 1.0f, // 0 right top front 
        0.0f,  1.0f, 1.0f,   152 * dec, 116 * dec, 0 * dec, 1.0f, // 1 left top front
        1.0f,  1.0f, 0.0f,   60 * dec, 116 * dec, 250 * dec, 1.0f, // 2 right top back
        0.0f,  1.0f, 0.0f,   250 * dec, 0 * dec, 73 * dec, 1.0f, // 3 left top back

         1.0f, 0.0f,  1.0f,  (120 * dec), (255 * dec), (173 * dec), 1.0f, // 4 right bottom front
         0.0f, 0.0f,  1.0f,  (75 * dec), (0 * dec), (255 * dec), 1.0f, // 5 left bottom front
         0.0f, 0.0f,  0.0f,  (0 * dec), (116 * dec), (73 * dec), 1.0f, // 6 right bottom back
         1.0f, 0.0f,  0.0f,  (164 * dec), (116 * dec), (93 * dec), 1.0f,  // 7 left bottom back

    };

    // Index data to share position data
    GLushort indices[] = {
         3, 2, 6, 7, 4, 2, 0,
         3, 1, 6, 5, 4, 1, 0
    };

    const GLuint floatsPerVertex = 3;
    const GLuint floatsPerColor = 4;

    glGenVertexArrays(1, &mesh.vao); // we can also generate multiple VAOs or buffers at the same time
    glBindVertexArray(mesh.vao);

    // Create 2 buffers: first one for the vertex data; second one for the indices
    glGenBuffers(2, mesh.vbos);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // Activates the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

    mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Strides between vertex coordinates is 6 (x, y, z, r, g, b, a). A tightly packed stride is 0.
    GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor);// The number of floats before each

    // Create Vertex Attribute Pointers
    glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (char*)(sizeof(float) * floatsPerVertex));
    glEnableVertexAttribArray(1);
}


void UCreateMesh_Background(GLMesh& mesh) {

}

void UDestroyMesh(GLMesh& mesh)
{
    glDeleteVertexArrays(1, &mesh.vao);
    glDeleteBuffers(1, mesh.vbos);
}


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
