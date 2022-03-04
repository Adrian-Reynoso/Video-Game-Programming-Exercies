#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
    //1.Update the owning actor’s rotation over time
    mOwner->SetRotation(mOwner->GetRotation() + (mAngularSpeed * deltaTime));
    
    //2.Updates the owning actor’s position based on the owning actor’s forward vector
    //find velocity
    Vector3 velocity = mOwner->GetForward() * mForwardSpeed;
    mOwner->SetPosition(mOwner->GetPosition() + (velocity * deltaTime));
}
