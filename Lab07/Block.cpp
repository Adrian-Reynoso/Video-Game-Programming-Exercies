//
//  Block.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/6/22.
//

#include "Block.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.hpp"

Block::Block(class Game* game, char letter)
: Actor(game)
{
    //Set scale of 25.0f
    SetScale(25.0f);
    
    //Initialize meshComponent and determine what texture it is from the letter being passed in
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    if (letter == 'A')
    {
        meshComponent->SetTextureIndex(3);
    }
    else
    {
        meshComponent->SetTextureIndex(4);
        
        //Set Exploding type to true
        explodingType = true;
    }
    
    //Initialize collisionComponent
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(1.0f, 1.0f, 1.0f);
    
    //Add this Block actor to the vector in Game
    game->AddBlock(this);
}

Block::~Block()
{
    GetGame()->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime)
{
    //Check if the block is more than 2000 units behind the ship. If so, destroy the actor
    if (mGame->GetPlayer()->GetPosition().x - GetPosition().x >= 2000.0f)
    {
        SetState(ActorState::Destroy);
    }
}
