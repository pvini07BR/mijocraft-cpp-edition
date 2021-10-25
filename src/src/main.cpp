#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"

#include <iostream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void window_focus_callback(GLFWwindow* window, int focused);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1280;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 720;

bool isFocused;

Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, true);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "mijocraft C++ edition", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowFocusCallback(window, window_focus_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwFocusWindow(window);
    isFocused = true;

    // initialize game
    // ---------------
    game.Init();

    // deltaTime variables
    // -------------------
    float beginTime = (float)glfwGetTime();
    float endTime;
    float deltaTime = -1.0f;

    //Init ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("assets/fonts/nokiafc22.ttf", 24);
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // render
        // ------
        glClearColor(0.48f,0.48f,0.67f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        game.Render();

        if(deltaTime >= 0)
        {
            // manage user input
            // -----------------
            if (isFocused == true)
            {
                game.ProcessInput(deltaTime);
                
                double xmousepos;
                double ymousepos;
                glfwGetCursorPos(window, &xmousepos, &ymousepos);
                game.ProcessMousePos(xmousepos, ymousepos);
            }

            // update game state
            // -----------------
            game.Update(deltaTime);
        }

        endTime = (float)glfwGetTime();
        deltaTime = endTime - beginTime;
        beginTime = endTime;
        
        //imgui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow((bool*)true);

        ImGui::Begin("Game Inspector Window", (bool*)false);

        game.ImGuiRender();

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (isFocused == true)
    {
        if (key >= 0 && key < 1024)
        {
            if(action != GLFW_REPEAT)
            {
                if (action == GLFW_PRESS)
                {
                    game.Keys[key] = true;
                    game.ProcessInputOnce();
                }

                else if (action == GLFW_RELEASE)
                {
                    game.Keys[key] = false;
                    game.KeysProcessed[key] = false;
                }
            }
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (isFocused == true)
    {
        if (button >= 0 && button < 3)
        {
            if(action != GLFW_REPEAT)
            {
                if (action == GLFW_PRESS)
                {
                    game.MouseButtons[button] = true;
                    game.ProcessInputOnce();
                }

                else if (action == GLFW_RELEASE)
                {
                    game.MouseButtons[button] = false;
                    game.MouseButtonsProcessed[button] = false;
                }
            }
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    game.ProcessScrolling(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    game.UpdateProjection(width, height);
}

void window_focus_callback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        isFocused = true;
    }
    else
    {
        isFocused = false;
    }
}



