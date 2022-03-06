//
//  SideBlock.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/5/22.
//

#ifndef SideBlock_hpp
#define SideBlock_hpp

#include <stdio.h>
#include "Actor.h"

class SideBlock : public Actor
{
public:
    SideBlock(class Game* game, size_t textureIndex);
    void OnUpdate(float deltaTime);
    class MeshComponent* meshComponent;
};

#endif /* SideBlock_hpp */
