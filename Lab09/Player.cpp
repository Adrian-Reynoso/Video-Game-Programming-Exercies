//
//  Player.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "PlayerMove.hpp"
#include "CameraComponent.hpp"
#include "CollisionComponent.h"

Player::Player(Game* game)
: Actor(game)
{
    //Create Player Move
    playerMove = new PlayerMove(this);
    
    //Create a camera component
    cameraComponent = new CameraComponent(this);
    
    //Create a collision Component
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(50, 175, 50);
    
}


