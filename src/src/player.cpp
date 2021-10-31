#include <player.h>

float maxVelocity = 4.0f;

ActivePlayer::ActivePlayer(std::string nickname, glm::vec4 color, b2World *physWorld)
{
    this->color = color;
    this->position = glm::vec2(0.0f, -3.0f);
    this->scale = 0.75f;
    this->spriteRotation = 0.0f;
    this->nickname = nickname;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(this->position.x, this->position.y);
    bodyDef.fixedRotation = true;
    bodyDef.allowSleep = false;
    bodyDef.awake = true;
    body = physWorld->CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(this->scale, this->scale, b2Vec2(this->scale / 2.0f, this->scale / 2.0f), 0.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 2.0f;
    fixtureDef.friction = 5.0f;

    body->CreateFixture(&fixtureDef);
}

ActivePlayer::~ActivePlayer()
{

}

void ActivePlayer::Render(SpriteRenderer *renderer)
{
    Texture2D colorTex = ResourceManager::GenerateColorTexture(color);
    renderer->DrawSprite(colorTex, this->position, glm::vec2(this->scale, this->scale), this->spriteRotation, glm::vec3(1.0f, 1.0f, 1.0f));
}

void ActivePlayer::PhysicsUpdate(float dt)
{
    b2Vec2 physPosition = this->body->GetPosition();
    float angle = this->body->GetAngle();
    this->position = glm::vec2((float)physPosition.x, (float)physPosition.y);
    if(!body->GetLinearVelocity().y == 0.0f)
    {
        this->spriteRotation += body->GetLinearVelocity().x / 2;
    } else {
        this->spriteRotation = 0.0f;
    }

    if(body->GetLinearVelocity().x > maxVelocity)
    {
        body->SetLinearVelocity(b2Vec2(maxVelocity, body->GetLinearVelocity().y));
    } else if(body->GetLinearVelocity().x < -maxVelocity)
    {
        body->SetLinearVelocity(b2Vec2(-maxVelocity, body->GetLinearVelocity().y));
    }
}

void ActivePlayer::InputUpdate(float dt, bool *keyArray)
{
    float velocity = 2.0f;
    if(keyArray[GLFW_KEY_D])
    {
        body->ApplyLinearImpulse(b2Vec2(velocity, 0.0f), b2Vec2(0.0f, 0.0f), true);
    }

    if(keyArray[GLFW_KEY_A])
    {
        body->ApplyLinearImpulse(b2Vec2(-velocity, 0.0f), b2Vec2(0.0f, 0.0f), true);
    }

    if(keyArray[GLFW_KEY_SPACE] && body->GetLinearVelocity().y == 0.0f)
    {
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -5.0f));
    }
}