//
//  Arrow.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/12/22.
//

#ifndef Arrow_hpp
#define Arrow_hpp

#include <stdio.h>
#include "Actor.h"

class Arrow : public Actor
{
    public:
    Arrow(class Game* game, Actor* parent);
    void OnUpdate(float deltaTime) override;
};
#endif /* Arrow_hpp */
