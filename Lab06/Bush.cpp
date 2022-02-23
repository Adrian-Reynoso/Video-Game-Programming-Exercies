//
//  Bush.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#include "Bush.hpp"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "Game.h"

Bush::Bush(class Game* game)
:Actor(game)
{
    //Allocate a new collision component and sprite component
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(32.0f, 32.0f);
    
    spriteComponent = new SpriteComponent(this);
    spriteComponent->SetTexture(game->GetTexture("Assets/Bush.png"));
}
