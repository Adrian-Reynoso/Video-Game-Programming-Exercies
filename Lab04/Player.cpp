//
//  Player.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "Player.hpp"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.hpp"
#include "Game.h"

Player::Player(class Game* game)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this, 200);
    spriteComponent->SetTexture(game->GetTexture("Assets/Mario/Idle.png"));
    
    //Dynamically allocate a collisionComponent
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(32.0f, 32.0f);
    
    //Create a PlayerMove in the Player constructor
    playerMove = new PlayerMove(this);
}
