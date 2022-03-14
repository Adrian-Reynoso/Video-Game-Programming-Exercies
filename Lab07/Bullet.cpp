//
//  Bullet.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/6/22.
//

#include "Bullet.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "Player.hpp"
#include "Block.hpp"
#include "PlayerMove.hpp"

Bullet::Bullet(class Game* game)
: Actor(game)
{
    //Set scale of 5.0f
    SetScale(5.0f);
    
    //Initialize meshComponent and determine what texture it is from the letter being passed in
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
    
    //Initialize collisionComponent
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(10.0f, 10.0f, 10.0f);
    
    //Initialize moveComponent
    moveComponent = new MoveComponent(this);
    moveComponent->SetForwardSpeed(900.0f);
}

void Bullet::OnUpdate(float deltaTime)
{
    //Update the forward speed of the bullet by multiplying it by the speedMultiplier in playerMove
    moveComponent->SetForwardSpeed(900.0f * mGame->GetPlayer()->playerMove->GetSpeedMultiplier());
    
    //Check if lifetime + deltaTime is greater than 1. If so, destroy bullet
    lifetime += deltaTime;
    if (lifetime >= 1.0f)
    {
        SetState(ActorState::Destroy);
    }
    
    //Check if the bullet collides with any of the blocks. If so, destroy the bullet
    for (Block* block : GetGame()->GetBlockVector())
    {
        if (collisionComponent->Intersect(block->collisionComponent))
        {
            //Check if the block that bullet collided with was an exploding type
            if (block->explodingType)
            {
                //If so, call the function to destroy exploding blocks
                DestroyExplodingBlock(block);
                Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
            }
            SetState(ActorState::Destroy);
        }
    }
}

void Bullet::DestroyExplodingBlock(class Block* block)
{
    //Delete the exploding block and all others in a 50 unit radius
    block->SetState(ActorState::Destroy);
    
    //Remove the current block from the blockVector so it doesn't intersect with itself
    mGame->RemoveBlock(block);
    
    //Check if the explosion collides with any other blocks. If so, destroy the blocks
    for (Block* otherBlock : GetGame()->GetBlockVector())
    {
        if (Vector3::Distance(block->GetPosition(), otherBlock->GetPosition()) <= 50.0f)
        {
            //Check if the block explosion collided with another exploding type, if so, recurse with the new block
            if (otherBlock->explodingType)
            {
                DestroyExplodingBlock(otherBlock);
            }
            else
            {
                //If it's not exploding type, then it's regular, so set the state to destroy
                otherBlock->SetState(ActorState::Destroy);
            }
        }
    }
}
