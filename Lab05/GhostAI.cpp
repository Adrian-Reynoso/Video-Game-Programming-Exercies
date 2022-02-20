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
    //Update the time in state variable with delta time
    timeInState += deltaTime;
    
    //Update the ghost’s position based on the movement direction, speed, and delta time
    float speed;
    if (GetState() == State::Scatter || GetState() == State::Chase)
    {
        speed = 90.0;
    }
    else if (GetState() == State::Frightened)
    {
        speed = 65.0;
    }
    else
    {
        speed = 125.0;
    }
    mGhost->SetPosition(mGhost->GetPosition() + (currDirection * speed * deltaTime));
    
    
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
        
        //Create a vector of all the nodes from the adjacency list that aren't PathNode::Ghost, PathNode::Tunnel, or previous Node
        std::vector<PathNode*> availableNodes;
        for (PathNode* node : mNextNode->mAdjacent)
        {
            if (node->GetType() != PathNode::Ghost && node->GetType() != PathNode::Tunnel && node != mPrevNode)
            {
                availableNodes.push_back(node);
            }
        }
        
        //If the vector isn't empty, then pick a random node from that vector and go to it
        if (availableNodes.empty() != true)
        {
            int randNum = rand.GetIntRange(0, (int)availableNodes.size()-1);
            desiredNode = availableNodes[randNum];
        }
        
        //If vector is empty, then fill vector with nodes that could also be ghost nodes
        else
        {
            for (PathNode* node : mNextNode->mAdjacent)
            {
                if (node->GetType() != PathNode::Tunnel && node != mPrevNode)
                {
                    availableNodes.push_back(node);
                }
            }
            
            //Check once more if vector is empty, if not pick a random node from that vector, else pick any node in the adjacency list
            if (availableNodes.empty() != true)
            {
                int randNum = rand.GetIntRange(0, (int)availableNodes.size()-1);
                desiredNode = availableNodes[randNum];
            }
            else
            {
                int randNum = rand.GetIntRange(0, (int)mNextNode->mAdjacent.size()-1);
                desiredNode = mNextNode->mAdjacent[randNum];
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
    
    if (mState == Chase)
    {
        //Set the target node for each of the four ghosts
        if (mGhost->GetType() == Ghost::Blinky)
        {
            //Targets Pac-Man’s previous path node if its not a tunnel node
            if (mOwner->GetGame()->mPlayer->GetPrevNode()->GetType() != PathNode::Tunnel)
            {
                 mTargetNode = mOwner->GetGame()->mPlayer->GetPrevNode();
            }
            else
            {
                //Iterate through the pathNodes and find a default closest to pacman
                mTargetNode = findClosestDefault(mOwner->GetGame()->mPlayer->GetPosition());
            }
        }
        
        else if(mGhost->GetType() == Ghost::Pinky)
        {
            //Targets 80 units in front of Pac-Man
            Vector2 inFrontOf = mOwner->GetGame()->mPlayer->GetPointInFrontOf(80.0f);
            
            //Iterate through the pathNodes and find a default closest to the point 80 units forward
            mTargetNode = findClosestDefault(inFrontOf);
        }
        
        else if(mGhost->GetType() == Ghost::Inky)
        {
            //Targets 40 units in front of Pac-Man
            Vector2 P = mOwner->GetGame()->mPlayer->GetPointInFrontOf(40.0f);
            
            //Make a vector V from Blinky to P and double the size of it
            Vector2 blinkyPos;
            for (Ghost* ghost : mOwner->GetGame()->mGhosts)
            {
                if (ghost->GetType() == Ghost::Blinky)
                {
                    blinkyPos = ghost->GetPosition();
                }
            }
            
            Vector2 V = P - blinkyPos;
            V *= 2;
            
            //Add blinky's position and V
            V += blinkyPos;
            
            //Get Pathnode of type Pathnode::Deafult closest to this point
            mTargetNode = findClosestDefault(V);
        }
        
        else
        {
            //If > 150 units from Pac-Man, same behavior as Blinky
            if (Vector2::Distance(mGhost->GetPosition(), mOwner->GetGame()->mPlayer->GetPosition()) > 150)
            {
                //DO the same as Blinky
                //Targets Pac-Man’s previous path node if its not a tunnel node
                if (mOwner->GetGame()->mPlayer->GetPrevNode()->GetType() != PathNode::Tunnel)
                {
                     mTargetNode = mOwner->GetGame()->mPlayer->GetPrevNode();
                }
                else
                {
                    //Iterate through the pathNodes and find a default closest to pacman
                    mTargetNode = findClosestDefault(mOwner->GetGame()->mPlayer->GetPosition());
                }
            }
            else
            {
                //Path toward the scatter home corner
                mTargetNode = mGhost->GetScatterNode();
            }
        }
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
    AnimatedSprite* animatedSprite = mGhost->GetComponent<AnimatedSprite>();
    if (temp.x < 0)
    {
        currDirection.x = -1.0f;
        currDirection.y = 0.0f;
        if (mState == Dead)
        {
            animatedSprite->SetAnimation("deadleft");
        }
        else
        {
            animatedSprite->SetAnimation("left");
        }
    }
    else if (temp.x > 0)
    {
        currDirection.x = 1.0f;
        currDirection.y = 0.0f;
        if (mState == Dead)
        {
            animatedSprite->SetAnimation("deadright");
        }
        else
        {
            animatedSprite->SetAnimation("right");
        }
    }
    else if (temp.y < 0)
    {
        currDirection.y = -1.0f;
        currDirection.x = 0.0f;
        if (mState == Dead)
        {
            animatedSprite->SetAnimation("deadup");
        }
        else
        {
            animatedSprite->SetAnimation("up");
        }
    }
    else
    {
        currDirection.y = 1.0f;
        currDirection.x = 0.0f;
        if (mState == Dead)
        {
            animatedSprite->SetAnimation("deaddown");
        }
        else
        {
            animatedSprite->SetAnimation("down");
        }
    }
    
    //For animation thats not in scatter
    //Set respective animations here depending on timeInState
    if (timeInState <= 5.0f && animatedSprite->GetAnimName() != "scared0" && mState == Frightened)
    {
        animatedSprite->SetAnimation("scared0");
    }
    else if (animatedSprite->GetAnimName() != "scared1" && mState == Frightened)
    {
        animatedSprite->SetAnimation("scared1");
    }
}

void GhostAI::needToChangeState()
{
    //If you've been in Frightened for 7seconds or more, change state back to scatter and reset timer
    if ((GetState() == State::Frightened && timeInState >= 7.0f) || (GetState() == State::Chase && timeInState >= 20.0f) || (mTargetNode == mNextNode && mState == Dead))
    {
        mState = Scatter;
        timeInState = 0.0f;
    }

    //Change state from scatter to chase
    if (GetState() == State::Scatter && timeInState >= 5.0f)
    {
        //Change to Chase state and reset timer
        mState = Chase;
        timeInState = 0.0f;
    }
}

PathNode* GhostAI::findClosestDefault(Vector2 position)
{
    //Iterate through the pathNodes and find a default closest to pacman
    float minDistance = 100000.0f;
    PathNode* desiredNode;
    
    for (PathNode* node : mOwner->GetGame()->mPathNodes)
    {
        if (node->GetType() == PathNode::Default && Vector2::Distance(position, node->GetPosition()) < minDistance)
        {
            desiredNode = node;
            minDistance = Vector2::Distance(position, node->GetPosition());
        }
    }
    
    return desiredNode;
}
