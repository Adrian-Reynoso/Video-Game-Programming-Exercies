//
//  Block.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/24/22.
//

#include "Block.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"

Block::Block(Game* game)
: Actor(game)
{
    SetScale(64.0f);
    
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(1.0f, 1.0f, 1.0f);
    
    //Add to block vector
    GetGame()->AddBlock(this);
}

Block::~Block()
{
    GetGame()->RemoveBlock(this);
}


