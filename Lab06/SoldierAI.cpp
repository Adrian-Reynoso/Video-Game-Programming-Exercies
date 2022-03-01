#include "SoldierAI.h"
#include "Actor.h"
#include "Game.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <algorithm>
#include "Soldier.hpp"

SoldierAI::SoldierAI(Actor* owner)
:SpriteComponent(owner)
{
	//SetIsVisible(false); // Uncomment to hide debug paths
    pathFinder = new PathFinder(mOwner->GetGame());
    mSoldier = static_cast<Soldier*>(owner);
}

void SoldierAI::Setup(PathNode* start, PathNode* end)
{
	// TODO: Implement
    //Set the mPatrolStart and mPatrolEnd member variables to start/end respectively
    mPatrolStart = start;
    mPatrolEnd = end;
    
    //Add a call to CalculatePath to compute the path from mPatrolStart to mPatrolEnd. For the outPath parameter, pass in mPath
    pathFinder->CalculatePath(mPatrolStart, mPatrolEnd, mPath);
    
    //Initialize mPrev, mNext, and mPath
    mPrev = mPatrolStart;
    mNext = mPath.back();
    mPath.pop_back();
    
    //Update direction vector
    calculateDirection();
}

void SoldierAI::Update(float deltaTime)
{
	// TODO: Implement
    
    //Make a temp variable for Soldier Position
    Vector2 tempPos {mOwner->GetPosition()};
    
    //Update Soldiers Position
    tempPos += (currDirection * SOLDIER_SPEED * deltaTime);
    
    //Check to see whether the soldier intersects with the next node
    if (Vector2::Distance(tempPos, mNext->GetPosition()) <= 3.0f)
    {
        //Set the Soldier's position to next node's
        tempPos = mNext->GetPosition();
        
        //If mPath is not empty
        if (!mPath.empty())
        {
            //1. Set mPrev to mNext
            mPrev = mNext;
            
            //2. Setting mNext to the last node in mPath
            mNext = mPath.back();
            
            //3. Remove the last node in mPath
            mPath.pop_back();
        }
        //Make the soldier to start a new path but in reverse
        else
        {
            Setup(mPatrolEnd, mPatrolStart);
        }
        
        //Update direction vector
        calculateDirection();
    }

    mOwner->SetPosition(tempPos);
}

// This helper is to just debug draw the soldier's path to visualize it
// (only called if this component is set to visible)
void SoldierAI::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();
	
	// Draw from current position to next
	SDL_RenderDrawLine(renderer,
					   static_cast<int>(mOwner->GetPosition().x - cameraPos.x),
					   static_cast<int>(mOwner->GetPosition().y - cameraPos.y),
					   static_cast<int>(mNext->GetPosition().x - cameraPos.x),
					   static_cast<int>(mNext->GetPosition().y - cameraPos.y));

	// Draw from next to first node on path
	if (!mPath.empty())
	{
		// Draw from current position to next
		SDL_RenderDrawLine(renderer,
						   static_cast<int>(mNext->GetPosition().x - cameraPos.x),
						   static_cast<int>(mNext->GetPosition().y - cameraPos.y),
						   static_cast<int>(mPath.back()->GetPosition().x - cameraPos.x),
						   static_cast<int>(mPath.back()->GetPosition().y - cameraPos.y));
	}
	
	// Draw each node on the path
	if (mPath.size() > 1)
	{
		for (size_t i = 0; i < mPath.size() - 1; i++)
		{
			SDL_RenderDrawLine(renderer,
							   static_cast<int>(mPath[i]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i]->GetPosition().y - cameraPos.y),
							   static_cast<int>(mPath[i + 1]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i + 1]->GetPosition().y - cameraPos.y));
		}
	}
}

void SoldierAI::calculateDirection()
{
    //Calculates the direction as the vector from mPrevNode to mNextNode
    Vector2 temp = mNext->GetPosition() - mPrev->GetPosition();

    if (temp.x < 0)
    {
        currDirection.x = -1.0f;
        currDirection.y = 0.0f;
        mSoldier->spriteComponent->SetAnimation("WalkLeft");
    }
    else if (temp.x > 0)
    {
        currDirection.x = 1.0f;
        currDirection.y = 0.0f;
        mSoldier->spriteComponent->SetAnimation("WalkRight");
    }
    else if (temp.y < 0)
    {
        currDirection.y = -1.0f;
        currDirection.x = 0.0f;
        mSoldier->spriteComponent->SetAnimation("WalkUp");
    }
    else
    {
        currDirection.y = 1.0f;
        currDirection.x = 0.0f;
        mSoldier->spriteComponent->SetAnimation("WalkDown");
    }
}

