#include <block.h>

static unsigned int ID = 0;

Block::Block(Texture2D texture, int posX, int posY, unsigned int type, bool isWall, b2World *physWorld)
{
    this->texture = texture;
    this->scale = 1.0f;
    this->position.x = posX;
    this->position.y = posY;
    this->type = type;
    this->isWall = isWall;

    if(!isWall)
    {
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set((float)posX, (float)posY);

        groundBody = physWorld->CreateBody(&groundBodyDef);

        b2EdgeShape Box;
        b2Vec2 v1(0.39f, 0.39f);
        b2Vec2 v2(0.61f, 0.61f);
        
        Box.SetTwoSided(v1, v2);

        groundBody->CreateFixture(&Box, 0.0f);
    }
    
    ID++;
}

void Block::Render(SpriteRenderer *Renderer)
{
    if(!isWall)
    {
        Renderer->DrawSprite(this->texture, this->position, glm::vec2(this->scale, this->scale), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
    else
    {
        Renderer->DrawSprite(this->texture, this->position, glm::vec2(this->scale, this->scale), 0.0f, glm::vec3(0.5f, 0.5f, 0.5f));
    }
}

Block::~Block()
{

}