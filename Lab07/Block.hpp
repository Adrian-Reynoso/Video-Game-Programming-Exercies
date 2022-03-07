//
//  Block.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/6/22.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"
#include <vector>

class Block : public Actor
{
public:
    Block(class Game* game, char letter);
    ~Block();
    void OnUpdate(float deltaTime) override;
    class MeshComponent* meshComponent = nullptr;
    class CollisionComponent* collisionComponent = nullptr;
    bool explodingType = false;
};

#endif /* Block_hpp */
