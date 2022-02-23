//
//  Player.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "Player.hpp"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "PlayerMove.hpp"
#include "Game.h"

Player::Player(class Game* game)
:Actor(game)
{
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new AnimatedSprite(this, 200);
    spriteComponent->LoadAnimations("Assets/Link");
    
    //Set the animation to "StandDown" initially
    spriteComponent->SetAnimation("StandDown");
    
    //Dynamically allocate a collisionComponent
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(20.0f, 20.0f);
    
    //Create a PlayerMove in the Player constructor
    playerMove = new PlayerMove(this);
}
