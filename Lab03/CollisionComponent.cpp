#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	// TODO: Implement
    //Go through the four conditions to prove there's no intersetions.
    //If neither of these conditons are true, then they objects DO intersect
    if (GetMax().x < other->GetMin().x)
    {
        return false;
    }
    else if (other->GetMax().x < GetMin().x)
    {
        return false;
    }
    else if (GetMax().y < other->GetMin().y)
    {
        return false;
    }
    else if (other->GetMax().y < GetMin().y)
    {
        return false;
    }
    
    //By this point, the objects intersect
	return true;
}

Vector2 CollisionComponent::GetMin() const
{
	// TODO: Implement
    //Create vector2 to return
    Vector2 min;
    
    //Get the min value for both the x and y positions and assign them to the vector
    min.x = mOwner->GetPosition().x - ((mWidth * mOwner->GetScale()) / 2.0f);
    min.y = mOwner->GetPosition().y - ((mHeight * mOwner->GetScale()) / 2.0f);
    
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	// TODO: Implement
    //Create vector2 to return
    Vector2 max;
    
    //Get the max value for both the x and y positions and assign them to the vector
    max.x = mOwner->GetPosition().x + ((mWidth * mOwner->GetScale()) / 2.0f);
    max.y = mOwner->GetPosition().y + ((mHeight * mOwner->GetScale()) / 2.0f);
    
    return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
	offset = Vector2::Zero;
	// TODO: Implement
	return CollSide::None;
}
