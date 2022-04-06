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
    Block(class Game* game);
    ~Block();
    class CollisionComponent* collisionComponent;
    class MeshComponent* meshComponent;
    bool GetIsMirror() {return isMirror;}
    void SetIsMirror(bool value) {isMirror = value;}
    
    private:
    bool isMirror = false;
};
#endif /* Block_hpp */
