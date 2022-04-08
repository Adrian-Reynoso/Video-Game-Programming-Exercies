//
//  Block.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/24/22.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"

class Block : public Actor
{
    public:
    Block(class Game* game, Actor* parent);
    ~Block();
    void OnUpdate(float deltaTime) override;
    class CollisionComponent* collisionComponent;
    class MeshComponent* meshComponent;
    bool GetIsMirror() {return isMirror;}
    void SetIsMirror(bool value) {isMirror = value;}
    void SetShouldRotate(bool value) {shouldRotate = value;}
    bool GetShouldRotate() {return shouldRotate;}
    
    private:
    bool isMirror = false;
    bool shouldRotate = false;
};
#endif /* Block_hpp */
