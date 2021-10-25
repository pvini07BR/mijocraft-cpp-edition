
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <vector>
#include <block.h>
#include <player.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <box2d/box2d.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <nlohmann/json.hpp>

enum PLACE_MODES {
    BLOCK,
    WALL
};

class Game
{
public:
    bool Keys[1024];
    bool KeysProcessed[1024];
    bool MouseButtons[3];
    bool MouseButtonsProcessed[3];
    unsigned int Width, Height;
    float zoom = 1.0f;

    //vectors
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec2 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec2 mousePos;
    glm::vec2 roundedMousePos;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void ProcessInputOnce();
    void ProcessMousePos(double xpos, double ypos);
    void ProcessScrolling(double xoffset, double yoffset);
    void Update(float dt);
    void UpdateProjection(int width, int height);
    void Render();
    void ImGuiRender();
};

class BlockManager
{
    public:
    static void PlaceBlock(unsigned int type, int posX, int posY, PLACE_MODES placMod);
    static void DestroyBlock(int posX, int posY, PLACE_MODES placMod);
    ~BlockManager();
};

#endif

