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
    //Update the time in state variable with delta time
    timeInState += deltaTime;
    
    //Update the ghost’s position based on the movement direction, speed, and delta time
    if (GetState() == State::Scatter || GetState() == State::Chase)
    {
        mGhost->SetPosition(mGhost->GetPosition() + (currDirection * 90 * deltaTime));
    }
    else if (GetState() == State::Frightened)
    {
        mGhost->SetPosition(mGhost->GetPosition() + (currDirection * 65 * deltaTime));
    }
    else
    {
        mGhost->SetPosition(mGhost->GetPosition() + (currDirection * 125 * deltaTime));
    }
    
    
    //Check to see whether the ghost intersects with the next node
    CollisionComponent* collisionComponent = mOwner->GetComponent<CollisionComponent>();
    if (collisionComponent->Intersect(mNextNode->GetComponent<CollisionComponent>()))
    {
        //Set the ghost's position to next node's
        mGhost->SetPosition(mNextNode->GetPosition());
        
        //See if you need to change the state of the ghost
        needToChangeState();
        
        //Change the state (if needed) and update the target node
        changeStateTargetNode();
        
        //Update mPrevNode and mNextNode for the next move the ghost will make
        updateNodes();
    }
}


void GhostAI::Frighten()
{
	// TODO: Implement
    //If ghost state is not dead
    if (GetState() != State::Dead)
    {
        //Reset time variable
        timeInState = 0.0f;
        
        //If mState is not currently Frightened, set it to frightened, swap direction, and set targent node to null
        mState = Frightened;
        
        PathNode* temp = mPrevNode;
        mPrevNode = mNextNode;
        mNextNode = temp;
        calculateDirection();
        
        mTargetNode = nullptr;
    }
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
    timeInState = 0.0f;
    
}

void GhostAI::Die()
{
	// TODO: Implement
    mState = Dead;
    timeInState = 0.0f;
    calculateDirection();
    //Target node should always be set to the mGhostPen
    mTargetNode = mGhost->GetGame()->mGhostPen;
    
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
    if (mState == Scatter)
    {
        //Set new target node
        mTargetNode = mGhost->GetScatterNode();
    }
    
    if(mState == Frightened)
    {
        //Set the target node by finding the random adjacent nodes that follow the restrictions
        PathNode* desiredNode = nullptr;
        Random rand;

        for (unsigned long i = 0; i < mNextNode->mAdjacent.size(); i++)
        {
            //Pick a random node from the adjacency list that will satisfy the if statement
            int randNum = rand.GetIntRange(0, (int)mNextNode->mAdjacent.size()-1);
            
            if (mNextNode->mAdjacent[randNum]->GetType() != PathNode::Ghost && mNextNode->mAdjacent[randNum]->GetType() != PathNode::Tunnel && mNextNode->mAdjacent[randNum] != mPrevNode)
            {
                desiredNode = mNextNode->mAdjacent[randNum];
                break;
            }
        }
        
        //Check to see if desired node is null. If so allow ghost nodes
        if (desiredNode == nullptr)
        {
            for (unsigned long i = 0; i < mNextNode->mAdjacent.size(); i++)
            {
                //Pick a random node from the adjacency list that will satisfy the if statement
                int randNum = rand.GetIntRange(0, (int)mNextNode->mAdjacent.size()-1);
                
                if (mNextNode->mAdjacent[randNum]->GetType() != PathNode::Tunnel && mNextNode->mAdjacent[randNum] != mPrevNode)
                {
                    desiredNode = mNextNode->mAdjacent[randNum];
                    break;
                }
            }
        }
        
        //Check to see if desired node is still null one last time. If so allow all node options
        if (desiredNode == nullptr)
        {
            for (unsigned long i = 0; i < mNextNode->mAdjacent.size(); i++)
            {
                //Pick a random node from the adjacency list that will satisfy the if statement
                int randNum = rand.GetIntRange(0, (int)mNextNode->mAdjacent.size()-1);
                desiredNode = mNextNode->mAdjacent[randNum];
                break;
            }
        }
        
        //Assign target node as the desired node
        mTargetNode = desiredNode;
    }
    
    if (mState == Dead)
    {
        //Target node should always be set to the mGhostPen
        mTargetNode = mGhost->GetGame()->mGhostPen;
    }
    
}

void GhostAI::updateNodes()
{
    //Set the next node by finding the smallest magnitude of the path to target node while still ensuring this node isn't a ghost or tunnel node
    PathNode* desiredNode = nullptr;
    float minMagnitude = 10000.0f;

    for (PathNode* node : mNextNode->mAdjacent)
    {
        if (mState == Dead)
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
        else
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
        if (mState == Dead)
        {
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadleft");
        }
        else
        {
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("left");
        }
    }
    else if (temp.x > 0)
    {
        currDirection.x = 1.0f;
        currDirection.y = 0.0f;
        if (mState == Dead)
        {
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadright");
        }
        else
        {
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("right");
        }
    }
    else if (temp.y < 0)
    {
        currDirection.y = -1.0f;
        currDirection.x = 0.0f;
        if (mState == Dead)
        {
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deadup");
        }
        else
        {
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("up");
        }
    }
    else
    {
        currDirection.y = 1.0f;
        currDirection.x = 0.0f;
        if (mState == Dead)
        {
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("deaddown");
        }
        else
        {
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("down");
        }
    }
    
    //For animation thats not in scatter
    //Set respective animations here depending on timeInState
    if (timeInState <= 5.0f && mGhost->GetComponent<AnimatedSprite>()->GetAnimName() != "scared0" && mState == Frightened)
    {
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
    }
    else if (mGhost->GetComponent<AnimatedSprite>()->GetAnimName() != "scared1" && mState == Frightened)
    {
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
    }
}

void GhostAI::needToChangeState()
{
    //If you've been in Frightened for 7seconds or more, change state back to scatter and reset timer
    if (GetState() == State::Frightened && timeInState >= 7.0f)
    {
        mState = Scatter;
        timeInState = 0.0f;
    }
    
    //Change state from Dead to Scatter if ghost reaches back to GhostPen
    if (mTargetNode == mNextNode && mState == Dead)
    {
        mState = Scatter;
    }
}
