//
//  Bullet.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/6/22.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <stdio.h>
#include "Actor.h"
#include <vector>

class Bullet : public Actor
{
public:
    Bullet(class Game* game);
    void OnUpdate(float deltaTime) override;
    void DestroyExplodingBlock(class Block* block);
    class MeshComponent* meshComponent = nullptr;
    class MoveComponent* moveComponent = nullptr;
    class CollisionComponent* collisionComponent = nullptr;
    float lifetime = 0.0f;
};
#endif /* Bullet_hpp */
