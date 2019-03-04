#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "debug_draw.hpp"

#include "Global.h"
#include "Debug.h"
#include "Camera.h"

#include "GameObjectManager.h"
#include "PhysicManager.h"
#include "GraphicSystem.h"
#include "PhysicSystem.h"
#include "UISystem.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, float deltaTime);
void window_size_callback(GLFWwindow* window, int width, int height);
glm::vec3 ScreenToWorldPoint(float x, float y);
void ScreenPosToWorldRay(int mouseX, int mouseY, int screenWidth, int screenHeight, glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::vec3& out_origin, glm::vec3& out_direction);

// Settings
unsigned int SCR_WIDTH = 1500;
unsigned int SCR_HEIGHT = 1000;
float DELTATIME = 0.0f;	// time between current frame and last frame
bool DEBUG_DRAW_ENABLE = false;
bool TEXTURE_DRAW_ENABLE = true;
float OCTTREE_MAXSIZE = 100.0f;
float OCTTREE_MINSIZE = 2.0f;
float MOUSEPICK_FORCE = 20.0f;

// Static Var
static bool ImGuiMouseOnWindow = false;
static bool isRightMouseButtonPressed = false;

// CAMERA
Camera CAMERA(glm::vec3(0.0f, 3.0f, 8.0f));

// Window Size
double lastX = SCR_WIDTH / 2.0;
double lastY = SCR_HEIGHT / 2.0;

// Input
bool firstMouse = true;
bool mouseOnUI = false;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Things we need for the window / GL render context:
    //glfwWindowHint(GLFW_RESIZABLE, false);
    //glfwWindowHint(GLFW_DEPTH_BITS, 32);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Graphic Framework", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Limit the framerate. */
    glfwSwapInterval(1);

    /* Initilize GLEW */
    if (glewInit() != GLEW_OK) {
        std::cout << "Unable to initialize GLEW." << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    // configure global opengl state
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    /* Game Object Manager */
    //GameObjectManager::LoadLevel("levels/Level_1.txt");
    GameObjectManager::LoadLevel("levels/Level_2_DebugTest.txt");

    /* Graphic System */
    GraphicSystem *graphicSystem = new GraphicSystem();
    graphicSystem->Init();

    /* Physic System */
    PhysicSystem *physicSystem = new PhysicSystem();
    physicSystem->Init();

    /* UI System */
    UISystem *uiSystem = new UISystem();
    uiSystem->Init(window);

    // timing
    float lastFrame = 0.0f;

    do
    {
        // per-frame time logic
        float currentFrame = (float)glfwGetTime();
        DELTATIME = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window, DELTATIME);

        // Mouse Picking Update
        PhysicManager::MousePickUpdate(isRightMouseButtonPressed);

        // Physic Update
        physicSystem->Update();

        // Graphic Rendering Update
        graphicSystem->Update();
        uiSystem->Update();
      
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
    
    // Terminate UI System
    uiSystem->Terminate();

    glfwTerminate();

    delete uiSystem;
    delete physicSystem;
    delete graphicSystem;
    GameObjectManager::CleanUp();

    return 0;
}

glm::vec3 ScreenToWorldPoint(float x, float y)
{
    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT);
    glm::mat4 tmpView = CAMERA.GetViewMatrix();
    glm::mat4 tmpProj = glm::perspective(glm::radians(CAMERA.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    float WinX, WinY, WinZ;
    WinX = x;
    WinY = SCR_HEIGHT - y;
    glReadPixels((GLint)WinX, (GLint)WinY, (GLsizei)1.0f, (GLsizei)1.0f, GL_DEPTH_COMPONENT, GL_FLOAT, &WinZ);

    glm::vec3 screenPos = glm::vec3(WinX, WinY, WinZ);
    glm::vec3 worldPos = glm::unProject(screenPos, tmpView, tmpProj, viewport);

    return worldPos;
}

void ScreenPosToWorldRay(
    int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
    int screenWidth, int screenHeight,  // Window size, in pixels
    glm::mat4 ViewMatrix,               // Camera position and orientation
    glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
    glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the CAMERA's position instead, ignore this.
    glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
)
{
    // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
    glm::vec4 lRayStart_NDC(
        ((float)mouseX / (float)screenWidth - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
        ((float)mouseY / (float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
        -1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
        1.0f
    );
    glm::vec4 lRayEnd_NDC(
        ((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
        ((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
        0.0,
        1.0f
    );

    // The Projection matrix goes from Camera Space to NDC.
    // So inverse(ProjectionMatrix) goes from NDC to Camera Space.
    glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

    // The View Matrix goes from World Space to Camera Space.
    // So inverse(ViewMatrix) goes from Camera Space to World Space.
    glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

    glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
    glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
    glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
    glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


    // Faster way (just one inverse)
    //glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
    //glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
    //glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


    glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
    lRayDir_world = glm::normalize(lRayDir_world);


    out_origin = glm::vec3(lRayStart_world);
    out_direction = glm::normalize(lRayDir_world);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        CAMERA.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        CAMERA.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        CAMERA.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        CAMERA.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Mouse Pick Handle
    if (isRightMouseButtonPressed && PhysicManager::GetMousePickObject() != NULL)
    {
        glm::vec3 worldPosition = ScreenToWorldPoint((float)xpos, (float)ypos);
        PhysicManager::MousePickHandle(worldPosition);
    }

    // Camera Handle
    if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
    {
        firstMouse = true;
        return;
    }

    if (mouseOnUI)
        return;

    ImGuiMouseOnWindow = true;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    CAMERA.ProcessMouseMovement((float)xoffset, (float)yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //std::cout << "At the moment of Mouse Press:" << std::endl;
        //std::cout << "ImGui" << ImGui::IsMouseHoveringAnyWindow() << std::endl;
        if (ImGui::IsMouseHoveringAnyWindow())
            mouseOnUI = true;
    }
        
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        mouseOnUI = false;
        //std::cout << "Mouse Release" << std::endl;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        glm::vec3 worldPosition = ScreenToWorldPoint((float)xpos, (float)ypos);

        #define GLM_TO_FLOAT(v, glmV) \
            v[0] = glmV.x; \
            v[1] = glmV.y; \
            v[2] = glmV.z

        //float Center[3];
        //GLM_TO_FLOAT(Center, worldPosition);
        //dd::sphere(Center, dd::colors::Yellow, 0.1f, 1000);

        // Raycast to all gameobjects
        glm::normalize(worldPosition - CAMERA.Position);

        glm::vec3 rayOrigin = CAMERA.Position;
        glm::vec3 rayDir = glm::normalize(worldPosition - CAMERA.Position);

        PhysicManager::RayCastAll(rayOrigin, rayDir);

        isRightMouseButtonPressed = true;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        isRightMouseButtonPressed = false;
    }

    return;
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (isRightMouseButtonPressed && PhysicManager::GetMousePickObject() != NULL)
    {
        glm::vec3 worldPosition = ScreenToWorldPoint((float)xpos, (float)ypos);
        PhysicManager::MousePickHandle(worldPosition);
    }
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    CAMERA.ProcessMouseScroll((float)yoffset);
}
