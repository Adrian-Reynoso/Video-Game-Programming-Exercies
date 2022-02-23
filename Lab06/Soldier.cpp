//
//  Soldier.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#include "Soldier.hpp"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Game.h"

Soldier::Soldier(class Game* game)
:Actor(game)
{
    //Allocate a new collision component and Animatedsprite component
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(32.0f, 32.0f);
    
    spriteComponent = new AnimatedSprite(this);
    spriteComponent->LoadAnimations("Assets/Soldier");
    spriteComponent->SetAnimation("WalkDown");
    spriteComponent->SetAnimFPS(5.0f);
}
