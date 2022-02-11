#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"
#include "Math.h"

GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
	// TODO: Implement
    //Update the ghost’s position based on the movement direction, speed, and delta time
    if (GetState() == State::Scatter || GetState() == State::Chase)
    {
        mGhost->SetPosition(mGhost->GetPosition() + (currDirection * 90 * deltaTime));
    }
    else if (GetState() == State::Frightened)
    {
        mGhost->SetPosition(currDirection * 65 * deltaTime);
    }
    else
    {
        mGhost->SetPosition(currDirection * 125 * deltaTime);
    }
    
    
    //Check to see whether the ghost intersects with the next node
    CollisionComponent* collisionComponent = mOwner->GetComponent<CollisionComponent>();
    if (collisionComponent->Intersect(mNextNode->GetComponent<CollisionComponent>()))
    {
        //Set the ghost's position to next node's
        mGhost->SetPosition(mNextNode->GetPosition());
        
        //Change the state (if needed) and update the target node
        changeStateTargetNode();
        
        //Update mPrevNode and mNextNode for the next move the ghost will make
        updateNodes();
    }
}


void GhostAI::Frighten()
{
	// TODO: Implement
}

void GhostAI::Start(PathNode* startNode)
{
	// TODO: Implement
    //Set the position of the owner to the position of startNode
    mGhost->SetPosition(startNode->GetPosition());
    
    //Other start conditions
    mState = Scatter;
    mPrevNode = nullptr;
    mNextNode = startNode;
    
}

void GhostAI::Die()
{
	// TODO: Implement
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}
	
	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}
}

void GhostAI::changeStateTargetNode()
{
    if (GetState() == State::Scatter)
    {
        //Set new target node
        mTargetNode = mGhost->GetScatterNode();
    }
}

void GhostAI::updateNodes()
{
    
    //Set the next node by finding the smallest magnitude of the path to target node while still ensuring this node isn't a ghost or tunnel node
    PathNode* desiredNode = nullptr;
    float minMagnitude = 10000.0f;

    for (PathNode* node : mNextNode->mAdjacent)
    {
        if (node->GetType() != PathNode::Ghost && node->GetType() != PathNode::Tunnel && node != mPrevNode)
        {
            //Calculate magnitude and see if its smaller than min, if so assign to desired node and update min magnitude
            if (Vector2::Distance(mTargetNode->GetPosition(), node->GetPosition()) < minMagnitude)
            {
                minMagnitude = Vector2::Distance(mTargetNode->GetPosition(), node->GetPosition());
                desiredNode = node;
            }
        }
    }
    
    //Check to see if desired node is null. If so allow ghost nodes
    if (desiredNode == nullptr)
    {
        for (PathNode* node : mNextNode->mAdjacent)
        {
            if (node->GetType() != PathNode::Tunnel && node != mPrevNode)
            {
                //Calculate magnitude and see if its smaller than min, if so assign to desired node and update min magnitude
                if (Vector2::Distance(mTargetNode->GetPosition(), node->GetPosition()) < minMagnitude)
                {
                    minMagnitude = Vector2::Distance(mTargetNode->GetPosition(), node->GetPosition());
                    desiredNode = node;
                }
            }
        }
    }
    
    //Check to see if desired node is still null one last time. If so allow all node options
    if (desiredNode == nullptr)
    {
        for (PathNode* node : mNextNode->mAdjacent)
        {
            //Calculate magnitude and see if its smaller than min, if so assign to desired node and update min magnitude
            if (Vector2::Distance(mTargetNode->GetPosition(), node->GetPosition()) < minMagnitude)
            {
                minMagnitude = Vector2::Distance(mTargetNode->GetPosition(), node->GetPosition());
                desiredNode = node;
            }
        }
    }
    
    //Assign next node as the desired node with the smallest magnitude to current node
    mPrevNode = mNextNode;
    mNextNode = desiredNode;
    
    //Call calculate direction function to figure out where the ghost is moving
    calculateDirection();
}

void GhostAI::calculateDirection()
{
    //Calculates the direction as the vector from mPrevNode to mNextNode
    Vector2 temp = mNextNode->GetPosition() - mPrevNode->GetPosition();
    if (temp.x < 0)
    {
        currDirection.x = -1.0f;
        currDirection.y = 0.0f;
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("left");
    }
    else if (temp.x > 0)
    {
        currDirection.x = 1.0f;
        currDirection.y = 0.0f;
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("right");
    }
    else if (temp.y < 0)
    {
        currDirection.y = -1.0f;
        currDirection.x = 0.0f;
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("up");
    }
    else
    {
        currDirection.y = 1.0f;
        currDirection.x = 0.0f;
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("down");
    }
}
