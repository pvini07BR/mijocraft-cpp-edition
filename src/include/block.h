#ifndef BLOCK_H
#define BLOCK_H

#include "texture.h"
#include "sprite_renderer.h"
#include <box2d/box2d.h>

class Block{
    public:
    Block(Texture2D texture, int posX, int posY, unsigned int type, b2World *physWorld);
    ~Block();
    Texture2D texture;
    glm::vec2 position;
    float scale;
    unsigned int type;
    b2Body* groundBody;
};

#endif