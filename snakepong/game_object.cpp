
#include "game_object.h"

GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite() {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite) {}

void GameObject::Draw(Renderer2D &renderer)
{
    if (this->UsesSprite == true)
        renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Color, this->Rotation);
    else
        renderer.DrawQuad(this->Position, this->Size, this->Color, this->Rotation);

}