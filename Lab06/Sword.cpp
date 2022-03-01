//
//  Sword.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/28/22.
//

#include "Sword.hpp"
#include "CollisionComponent.h"
#include "Game.h"

Sword::Sword(class Game* game)
:Actor(game)
{
    //Allocate a new collision component and sprite component
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(28.0f, 28.0f);
}
