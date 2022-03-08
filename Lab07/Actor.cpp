#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
    //Actor needs to add itself to the game via AddActor
    game->AddActor(this);
    //Assign the game input parameter to member variable mGame
    mGame = game;
}

Actor::~Actor()
{
    //The Actor destructor should call RemoveActor
    mGame->RemoveActor(this);
    
    //Loop to delete all the components inside the member variable vector
    for (unsigned long i = 0; i < mComponents.size(); i++)
    {
        delete mComponents[i];
    }
    
    //Clear the vector of components
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    //If Actor’s mState is ActorState::Active
    if (mState == ActorState::Active)
    {
        //Loop over all its components and call Update on each of them
        for (unsigned long i = 0; i < mComponents.size(); i++)
        {
            mComponents[i]->Update(deltaTime);
        }
        
        //Call the OnUpdate member function
        OnUpdate(deltaTime);
    }
    
    //Calculate the worldTransform matrix
    //Create the Scale, Rotation, and Translation Matrices
    Matrix4 scale = Matrix4::CreateScale(mScale);
    Matrix4 rotationZ = Matrix4::CreateRotationZ(mRotation);
    Matrix4 rotationX = Matrix4::CreateRotationX(mRollAngle);
    Matrix4 translation = Matrix4::CreateTranslation(mPosition);
    
    mWorldTransform = scale * rotationZ * rotationX * translation;
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
    //If Actor’s mState is ActorState::Active
    if (mState == ActorState::Active)
    {
        //Loop over all its components and call ProcessInput on each of them
        for (unsigned long i = 0; i < mComponents.size(); i++)
        {
            mComponents[i]->ProcessInput(keyState);
        }
        
        //Call the OnProcessInput member function
        OnProcessInput(keyState);
    }
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
