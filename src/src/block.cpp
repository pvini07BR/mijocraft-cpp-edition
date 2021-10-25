#include <block.h>

static unsigned int ID = 0;

Block::Block(Texture2D texture, int posX, int posY, unsigned int type, b2World *physWorld)
{
    this->texture = texture;
    this->scale = 1.0f;
    this->position.x = posX;
    this->position.y = posY;
    this->type = type;

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set((float)posX, (float)posY);

    groundBody = physWorld->CreateBody(&groundBodyDef);

    //b2PolygonShape Box;
    //Box.SetAsBox(0.11f, 0.11f, b2Vec2(this->scale / 2.0f, this->scale / 2.0f), 0.0f);
    b2EdgeShape Box;
    b2Vec2 v1(0.39f, 0.39f);
    b2Vec2 v2(0.61f, 0.61f);
    
    Box.SetTwoSided(v1, v2);

    groundBody->CreateFixture(&Box, 0.0f);

    ID++;
}

Block::~Block()
{

}