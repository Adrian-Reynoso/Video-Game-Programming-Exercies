#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
    ,mParent(parent)
{
    //Actor needs to add itself to the game via AddActor
    if (mParent == nullptr)
    {
        game->AddActor(this);
    }
    else
    {
        mParent->AddChild(this);
    }
    //Assign the game input parameter to member variable mGame
    mGame = game;
}

Actor::~Actor()
{
    //Delete all children from the children vector
    while (!mChildren.empty())
    {
        delete mChildren.back();
    }
    
    //The Actor destructor should call RemoveActor
    if (mParent == nullptr)
    {
        mGame->RemoveActor(this);
    }
    else
    {
        mParent->RemoveChild(this);
    }
    
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
    //Calclulate the world transform initially
    CalcWorldTransform();
    
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
    
    //Calclulate the world transform
    CalcWorldTransform();
    
    //Loop over mChildren and call Update on each child
    for (Actor* child : mChildren)
    {
        child->Update(deltaTime);
    }
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

Vector3 Actor::GetQuatForward()
{
    Vector3 forward = Vector3::Transform(Vector3::UnitX, mQuaternion);
    forward.Normalize();
    return forward;
}

void Actor::CalcWorldTransform()
{
    //Calculate the worldTransform matrix
    //Create the Scale, Rotation, and Translation Matrices
    Matrix4 scale = Matrix4::CreateScale(mScale);
    Matrix4 rotationZ = Matrix4::CreateRotationZ(mRotation);
    Matrix4 rotationX = Matrix4::CreateRotationX(mRollAngle);
    Matrix4 rotationQ = Matrix4::CreateFromQuaternion(mQuaternion);
    Matrix4 translation = Matrix4::CreateTranslation(mPosition);
    
    mWorldTransform = scale * rotationZ * rotationX * rotationQ * translation;
    
    //Check if mParent is set
    if (mParent != nullptr)
    {
        if (mInheritScale)
        {
            mWorldTransform *= mParent->GetWorldTransform();
        }
        else
        {
            mWorldTransform *= mParent->GetWorldRotTrans();
        }
    }
}

Matrix4 Actor::GetWorldRotTrans()
{
    //Calculate temporary World trasnform matrix w/out scale
    //Create the Rotation and Translation Matrices
    Matrix4 rotationZ = Matrix4::CreateRotationZ(mRotation);
    Matrix4 rotationX = Matrix4::CreateRotationX(mRollAngle);
    Matrix4 rotationQ = Matrix4::CreateFromQuaternion(mQuaternion);
    Matrix4 translation = Matrix4::CreateTranslation(mPosition);
    
    Matrix4 tempWorldTransform = rotationZ * rotationX * rotationQ * translation;
    
    //If there’s a parent, multiply the return value by the parent’s result of GetWorldRotTrans
    if (mParent != nullptr)
    {
        tempWorldTransform *= mParent->GetWorldRotTrans();
    }
    
    return tempWorldTransform;
}

Vector3 Actor::GetWorldPosition()
{
    //extracts the translation component from the world transform
    return mWorldTransform.GetTranslation();
}

Vector3 Actor::GetWorldForward()
{
    //extracts the x-axis from the world transform
    return mWorldTransform.GetXAxis();
}
