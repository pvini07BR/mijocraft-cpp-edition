#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include <text_renderer.h>

// Game-related State data
SpriteRenderer *Renderer;
TextRenderer *textRender;
ActivePlayer *player;
b2World *world;

static std::vector<Block> blocks;
static std::vector<Block> walls;
static std::vector<Texture2D> blockTextures;
unsigned int selectedBlock = 0;
bool toggleWorldStep = true;
bool cameraFollowingPlayer = true;

PLACE_MODES placeMode = BLOCK;

float physicsTimeStep = 1.0f / 60.0f;
int32 velocityIterations = 6;
int32 positionIterations = 2;

b2Vec2 gravity(0.0f, 10.0f);

Game::Game(unsigned int width, unsigned int height) : Keys(), Width(width), Height(height)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete textRender;
}

void Game::Init()
{
    world = new b2World(gravity);
    cameraPos = glm::vec2(0.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // load shaders
    ResourceManager::LoadShader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl", nullptr, "sprite");
    // configure shaders

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    // set render-specific controls
    Shader myShader;
    myShader = ResourceManager::GetShader("sprite");
    std::srand(static_cast <unsigned> (std::time(0)));
    float r = static_cast <float> (std::rand()) / static_cast <float> (1.0f);
    float g = static_cast <float> (std::rand()) / static_cast <float> (1.0f);
    float b = static_cast <float> (std::rand()) / static_cast <float> (1.0f);
    player = new ActivePlayer("Player", glm::vec4(r, g, b, 1.0f), world);
    Renderer = new SpriteRenderer(myShader);
    textRender = new TextRenderer(this->Width, this->Height);
    textRender->Load("assets/fonts/nokiafc22.ttf", 8);
    // load textures
    
    blockTextures.push_back(ResourceManager::LoadTexture("assets/sprites/grass.png", "grass"));
    blockTextures.push_back(ResourceManager::LoadTexture("assets/sprites/dirt.png", "dirt"));
    blockTextures.push_back(ResourceManager::LoadTexture("assets/sprites/planks.png", "planks"));
    blockTextures.push_back(ResourceManager::LoadTexture("assets/sprites/stone.png", "stone"));
    blockTextures.push_back(ResourceManager::LoadTexture("assets/sprites/glass.png", "glass"));

    ResourceManager::LoadTexture("assets/sprites/placeMode_block.png", "placeMode_block");
    ResourceManager::LoadTexture("assets/sprites/placeMode_wall.png", "placeMode_wall");
    
    BlockManager::PlaceBlock(0, 0, 1, BLOCK);
    BlockManager::PlaceBlock(0, 1, 1, BLOCK);
    BlockManager::PlaceBlock(0, 2, 1, BLOCK);
    BlockManager::PlaceBlock(0, 3, 1, BLOCK);
    BlockManager::PlaceBlock(0, 4, 1, BLOCK);
}

void Game::Update(float dt)
{
    if(toggleWorldStep)
    {
        world->Step(physicsTimeStep, velocityIterations, positionIterations);
    }
    player->PhysicsUpdate(dt);

    roundedMousePos = glm::vec2(round(mousePos.x - 0.5f), round(mousePos.y - 0.5f));
    if (cameraFollowingPlayer)
    {
        cameraPos = glm::vec2(player->position.x + (player->scale / 2.0f), player->position.y + (player->scale / 2.0f));
    }
}

void Game::ProcessInput(float dt)
{
    player->InputUpdate(dt, Keys);
}

void Game::ProcessInputOnce()
{
    if(MouseButtons[GLFW_MOUSE_BUTTON_RIGHT])
    {
        BlockManager::PlaceBlock(selectedBlock, roundedMousePos.x, roundedMousePos.y, placeMode);
    }

    if (MouseButtons[GLFW_MOUSE_BUTTON_LEFT])
    {
        BlockManager::DestroyBlock(roundedMousePos.x, roundedMousePos.y, placeMode);
    }

    if(Keys[GLFW_KEY_TAB])
    {
        if(placeMode == BLOCK){
            placeMode = WALL;
        } else if(placeMode == WALL)
        {
            placeMode = BLOCK;
        }
    }

    if(Keys[GLFW_KEY_LEFT_CONTROL] && Keys[GLFW_KEY_EQUAL] || Keys[GLFW_KEY_RIGHT_CONTROL] && Keys[GLFW_KEY_EQUAL])
    {
        zoom += 0.5f;
    }
    if(Keys[GLFW_KEY_LEFT_CONTROL] && Keys[GLFW_KEY_MINUS] || Keys[GLFW_KEY_RIGHT_CONTROL] && Keys[GLFW_KEY_MINUS])
    {
        if(zoom > 0.5f)
        {
            zoom -= 0.5f;
        }
    }
}

void Game::ProcessMousePos(double xpos, double ypos)
{
    float currentX = xpos;
    currentX = (currentX / Width) * 2.0f - 1.0f;
    glm::vec4 tmpX = glm::vec4(currentX, 0.0f, 0.0f, 1.0f);
    tmpX = tmpX * (glm::inverse(projection) * glm::inverse(view));
    currentX = tmpX.x + cameraPos.x;

    float currentY = Height - ypos;
    currentY = (currentY / Height) * 2.0f - 1.0f;
    glm::vec4 tmpY = glm::vec4(0.0f, currentY, 0.0f, 1.0f);
    tmpY = tmpY * (glm::inverse(projection) * glm::inverse(view));
    currentY = tmpY.y + cameraPos.y;

    mousePos = glm::vec2(currentX, currentY);
}

void Game::ProcessScrolling(double xoffset, double yoffset)
{
    if(yoffset == 1 && selectedBlock < 4)
    {
        selectedBlock += 1;
    }
    if(yoffset == -1 && selectedBlock > 0)
    {
        selectedBlock -= 1;
    }
}

void Game::UpdateProjection(int width, int height){
    this->Width = width;
    this->Height = height;
}

void Game::Render()
{   
    std::vector<Block>::iterator wallRend;
    for (wallRend = walls.begin(); wallRend != walls.end(); ++wallRend)
    {
        wallRend->Render(Renderer);
    }

    std::vector<Block>::iterator blockRend;
    for (blockRend = blocks.begin(); blockRend != blocks.end(); ++blockRend)
    {
        blockRend->Render(Renderer);
    }

    player->Render(Renderer);

    textRender->RenderText(player->nickname, player->position.x - (player->nickname.length() / 4), player->position.y - 1, 0.075f);

    Texture2D selectorTex = ResourceManager::GenerateColorTexture(glm::vec4(1.0f, 1.0f, 1.0f, 0.5f));
    Renderer->DrawSprite(selectorTex, roundedMousePos, glm::vec2(1.0f, 1.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    Renderer->DrawSprite(blockTextures[selectedBlock], mousePos + 0.25f, glm::vec2(0.75f, 0.75f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    Texture2D placeModeTex;
    if(placeMode == BLOCK)
    {
        placeModeTex = ResourceManager::GetTexture("placeMode_block");
    } else if(placeMode == WALL)
    {
        placeModeTex = ResourceManager::GetTexture("placeMode_wall");
    }
    Renderer->DrawSprite(placeModeTex, mousePos + 0.8f, glm::vec2(0.5f, 0.5f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    projection = glm::ortho(0.0f, (Width / (32.0f * zoom)), Height / (32.0f * zoom), 0.0f, -1.0f, 100.0f);
    glm::vec3 centralizedCamera = glm::vec3(cameraPos.x - (((Width / (32.0f * zoom)) / 2)), cameraPos.y - (((Height / (32.0f * zoom)) / 2)), 3.0f);
    view = glm::lookAt(centralizedCamera, centralizedCamera + cameraFront, cameraUp);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("sprite").SetMatrix4("view", view);
    
    textRender->TextShader.SetMatrix4("projection", projection, true);
    textRender->TextShader.SetMatrix4("view", view, true);
}

void Game::ImGuiRender()
{
    if (ImGui::CollapsingHeader("Player Inspector"))
    {
        static char* playerNick = const_cast<char *>(player->nickname.c_str());
        if (ImGui::InputText("Nickname", playerNick, IM_ARRAYSIZE(playerNick)))
        {
            std::stringstream test;
            test << playerNick;
            player->nickname = test.str();
        }

        static float playerColor[4] = { player->color.x, player->color.y, player->color.z, player->color.w};
        if (ImGui::ColorEdit4("Color", playerColor))
        {
            player->color = glm::vec4(playerColor[0], playerColor[1], playerColor[2], playerColor[3]);
        }

        ImVec2 playerPos = ImVec2(player->body->GetPosition().x, player->body->GetPosition().y);
        if (ImGui::DragFloat2("Position", (float*)&playerPos))
        {
            player->body->SetTransform(b2Vec2(playerPos.x, playerPos.y), 0.0f);
        }

        ImVec2 playerLinVel = ImVec2(player->body->GetLinearVelocity().x, player->body->GetLinearVelocity().y);
        if (ImGui::DragFloat2("Linear Velocity", (float*)&playerLinVel))
        {
            player->body->SetLinearVelocity(b2Vec2(playerLinVel.x, playerLinVel.y));
        }
    }

    if (ImGui::CollapsingHeader("Camera Inspector"))
    {
        ImGui::Checkbox("Following Player", &cameraFollowingPlayer);
        ImGui::DragFloat("Zoom factor", &zoom, 0.5f, 0.5f);

        ImVec2 imCameraPos = ImVec2(cameraPos.x, cameraPos.y);
        if (ImGui::DragFloat2("Position", (float*)&imCameraPos, 0.25f))
        {
            cameraPos = glm::vec2(imCameraPos.x, imCameraPos.y);
        }
    }
    
    if (ImGui::CollapsingHeader("Save World"))
    {
        static char filename[64] = "";
        ImGui::InputText("World File Name", filename, 64);
        if(ImGui::Button("Generate and Save World JSON File"))
        {
            SaveWorld(filename);
        }
    }

    if(ImGui::CollapsingHeader("Load World"))
    {
        std::vector<std::string> files;
        std::filesystem::path daPath = std::filesystem::current_path() / "worlds";
        for (const auto & entry : std::filesystem::directory_iterator(daPath))
        {
            files.push_back(entry.path().filename().string());
        }

        static int selectedItem = 0;
        if (ImGui::BeginListBox("Select a world file from the list"))
        {
            for (int n = 0; n < files.size(); n++)
            {
                const bool is_selected = (selectedItem == n);
                if (ImGui::Selectable(files[n].c_str(), is_selected))
                    selectedItem = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        if(ImGui::Button("Load Selected World"))
        {
            std::stringstream ii;
            ii << "worlds/" << files[selectedItem];
            std::string is = ii.str();
            LoadWorld(is.c_str());
        }
    }
}

void Game::SaveWorld(std::string filename)
{
    nlohmann::json worldData;

    for(int i = 0; i < blocks.size(); i++)
    {
        worldData["blocks"][static_cast<char>(i)]["type"] = blocks[i].type;
        worldData["blocks"][static_cast<char>(i)]["X"] = blocks[i].position.x;
        worldData["blocks"][static_cast<char>(i)]["Y"] = blocks[i].position.y;
    }

    for(int j = 0; j < walls.size(); j++)
    {
        worldData["walls"][static_cast<char>(j)]["type"] = walls[j].type;
        worldData["walls"][static_cast<char>(j)]["X"] = walls[j].position.x;
        worldData["walls"][static_cast<char>(j)]["Y"] = walls[j].position.y;
    }

    std::stringstream ss;
    ss << "worlds/" << filename << ".json";
    std::string s = ss.str();
    std::ofstream out_file(s.c_str());
    out_file << worldData;
}

void Game::LoadWorld(const char* fileDirectory)
{
    std::ifstream i(fileDirectory);
    nlohmann::json j;
    j << i;

    for(int i = 0; i < blocks.size(); i++)
    {
        world->DestroyBody(blocks[i].groundBody);
    }

    blocks.clear();
    walls.clear();

    for(int i = 0; i < j["blocks"].size(); i++)
    {
        int type = j["blocks"][static_cast<char>(i)]["type"];
        int posX = j["blocks"][static_cast<char>(i)]["X"];
        int posY = j["blocks"][static_cast<char>(i)]["Y"];

        BlockManager::PlaceBlock(type, posX, posY, BLOCK);
    }

    for(int l = 0; l < j["walls"].size(); l++)
    {
        int type2 = j["walls"][static_cast<char>(l)]["type"];
        int posX2 = j["walls"][static_cast<char>(l)]["X"];
        int posY2 = j["walls"][static_cast<char>(l)]["Y"];

        BlockManager::PlaceBlock(type2, posX2, posY2, WALL);
    }
}

void BlockManager::PlaceBlock(unsigned int type, int posX, int posY, PLACE_MODES placMod)
{
    float isThereABlockOnTheWay = false;
    if(placMod == BLOCK)
    {
        std::vector<Block>::iterator blockRend;
        for (blockRend = blocks.begin(); blockRend != blocks.end(); ++blockRend)
        {
            if(blockRend->position.x == posX && blockRend->position.y == posY)
            {
                isThereABlockOnTheWay = true;
                break;
            }
        }
    } else if (placMod == WALL)
    {
        std::vector<Block>::iterator uol;
        for (uol = walls.begin(); uol != walls.end(); ++uol)
        {
            if(uol->position.x == posX && uol->position.y == posY)
            {
                isThereABlockOnTheWay = true;
                break;
            }
        }
    }
    
    if(!isThereABlockOnTheWay)
    {
        if(placMod == BLOCK)
        {
            Block *b = new Block(blockTextures[type], posX, posY, type, false, world);
            blocks.push_back(*b);
        } else if(placMod == WALL){
            Block *b = new Block(blockTextures[type], posX, posY, type, true, world);
            walls.push_back(*b);
        }
    }
}

void BlockManager::DestroyBlock(int posX, int posY, PLACE_MODES placMod)
{
    if(placMod == BLOCK){
        std::vector<Block>::iterator blockRend;
        for (blockRend = blocks.begin(); blockRend != blocks.end(); ++blockRend)
        {
            if(blockRend->position.x == posX && blockRend->position.y == posY)
            {
                world->DestroyBody(blockRend->groundBody);
                blocks.erase(blockRend);
                break;
            }
        }
    }
    else if(placMod == WALL)
    {
        std::vector<Block>::iterator uol;
        for (uol = walls.begin(); uol != walls.end(); ++uol)
        {
            if(uol->position.x == posX && uol->position.y == posY)
            {
                walls.erase(uol);
                break;
            }
        }
    }
}

BlockManager::~BlockManager()
{
    blocks.clear();
    walls.clear();
}
