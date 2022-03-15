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
#include "PlayerMove.hpp"

Player::Player(Game* game)
: Actor(game)
{
    //Set scale
    SetScale(0.75f);
    
    //Create MeshComponent and set the mesh
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
    
    //Create Player Move
    playerMove = new PlayerMove(this);
    
}
