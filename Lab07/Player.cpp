//
//  Player.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.hpp"

Player::Player(Game* game)
: Actor(game)
{
    //Set scale
    SetScale(2.0f);
    
    //Create MeshComponent and set the mesh
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arwing.gpmesh"));
    
    //Create Collision Component
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(40.0f, 25.0f, 15.0f);
    
    //Create Player Move
    playerMove = new PlayerMove(this);
    
}
