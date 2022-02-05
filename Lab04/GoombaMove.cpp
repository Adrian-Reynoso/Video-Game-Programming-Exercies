//
//  GoombaMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/4/22.
//

#include "GoombaMove.hpp"
#include "Goomba.hpp"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Block.hpp"
#include "Game.h"

GoombaMove::GoombaMove(class Actor* owner)
:MoveComponent(owner)
{
    SetForwardSpeed(speed);
}

void GoombaMove::Update(float deltaTime)
{
    //Create temporary position
    Vector2 tempPos = mOwner->GetPosition();
    
    
    //Update the Goomba's position by its speed
    tempPos.x += (GetForwardSpeed() * deltaTime);
    
    //Check if Goomba collides with blocks
    CollSide onBlock = CollSide::None;
    //For Top Collisions
    for (Block* block : mOwner->GetGame()->GetBlockVector())
    {
        Vector2 offSet {0.0f, 0.0f};
        onBlock = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(block->collisionComponent, offSet);

        if (onBlock != CollSide::None)
        {
            //Add offset to temPos
            tempPos += offSet;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);
            
            if (onBlock == CollSide::Top && mYSpeed > 0.0f)
            {
                //Goomba is landing on top of block
                mYSpeed = 0;
            }
        }
    }
    
    //For Left/Right Collisions with Blocks
    for (Block* block : mOwner->GetGame()->GetBlockVector())
    {
        Vector2 offSet {0.0f, 0.0f};
        onBlock = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(block->collisionComponent, offSet);

        if (onBlock != CollSide::None)
        {
            //Add offset to temPos
            tempPos += offSet;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);

            if (onBlock == CollSide::Right && block->GetPosition().y == tempPos.y)
            {
                //Send Goomba the opposite way
                SetForwardSpeed(100.0f);
            }
            else if (onBlock == CollSide::Left && block->GetPosition().y == tempPos.y)
            {
                //Send Goomba the opposite way
                SetForwardSpeed(-100.0f);
            }

        }
    }
    
    CollSide onBlock2 = CollSide::None;
    //Check if Goomba collides with other Goombas
    for (Goomba* goomba : mOwner->GetGame()->GetGoombaVector())
    {
        Vector2 offSet {0.0f, 0.0f};
        onBlock2 = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(goomba->collisionComponent, offSet);

        if (onBlock2 != CollSide::None)
        {
            //Add offset to temPos
            tempPos.x += offSet.x;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);

            if (onBlock == CollSide::Right)
            {
                //Send Goomba the opposite way
                SetForwardSpeed(100.0f);
            }
            else if (onBlock == CollSide::Left)
            {
                //Send Goomba the opposite way
                SetForwardSpeed(-100.0f);
            }
        }
    }
    
    //Updates the owning actor’s y position based on the owning actor’s y speed (Using Euler Integration)
    tempPos.y += (mYSpeed * deltaTime);
    mYSpeed += mYAccel * deltaTime;
    
    //Check if player if less greater than 448.0f, if so destroy them
    if (tempPos.y > 448.0f)
    {
        mOwner->SetState(ActorState::Destroy);
    }
    
    //Set the position based off our temporary position vector
    mOwner->SetPosition(tempPos);
}
