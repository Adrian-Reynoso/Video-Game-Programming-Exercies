//
//  Coin.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/12/22.
//

#ifndef Coin_hpp
#define Coin_hpp

#include <stdio.h>
#include "Actor.h"

class Coin : public Actor
{
    public:
        Coin(class Game* game, Actor* parent);
        void OnUpdate(float deltaTime) override;
        class MeshComponent* meshComponent;
        class CollisionComponent* collisionComponent;
};
#endif /* Coin_hpp */
