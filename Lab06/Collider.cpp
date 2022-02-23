//
//  Collider.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#include "Collider.hpp"
#include "CollisionComponent.h"

Collider::Collider(class Game* game, float width, float height)
:Actor(game)
{
    //Allocate a new collision component and make the dimensions the width in heights passed throught the parameters
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(width, height);
}
