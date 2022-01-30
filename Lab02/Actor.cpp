#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector2::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
    //Actor needs to add itself to the game via AddActor
    game->AddActor(this);
}

Actor::~Actor()
{
    //The Actor destructor should call RemoveActor
    mGame->RemoveActor(this);
    
    //Loop to delete all the components inside the member variable vector
    for (Component* component : mComponents)
    {
        delete component;
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
        for (Component* component : mComponents)
        {
            component->Update(deltaTime);
        }
        
        OnUpdate(deltaTime);
    }
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{

    if (mState == ActorState::Active)
    {
        //Loop over all its components and call ProcessInput on each of them
        for (Component* component : mComponents)
        {
            component->ProcessInput(keyState);
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
