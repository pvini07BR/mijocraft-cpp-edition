#ifndef PLAYER_H
#define PLAYER_H

#include "texture.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <box2d/box2d.h>

class ActivePlayer{
    public:
    ActivePlayer(glm::vec4 color, b2World *physWorld);
    ~ActivePlayer();
    void Render(SpriteRenderer *renderer);
    void PhysicsUpdate(float dt);
    void InputUpdate(float dt, bool *keyArray);
    glm::vec2 position;
    glm::vec4 color;
    float spriteRotation;
    float scale;
    b2Body* body;
};

#endif