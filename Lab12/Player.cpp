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
#include "Arrow.hpp"
#include "HUD.hpp"

Player::Player(Game* game, Actor* parent)
: Actor(game, parent)
{
    //Create Player Move
    playerMove = new PlayerMove(this);
    
    //Create a camera component
    cameraComponent = new CameraComponent(this);
    
    //Create a collision Component
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(50.0f, 175.0f, 50.0f);
    
    //Create the arrow
    arrow = new Arrow(game, parent);
    
    //Create a HUD
    mHUD = new HUD(this);
}


