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
    //If they do intersect, the function returns the CollSide that is the minimum overlap side.
    if (Intersect(other))
    {
        Vector2 otherMax = other->GetMax();
        Vector2 otherMin = other->GetMin();
        
        //Do the calculations and determine which has the least overlap of the four sides
        float otherMaxXDiff = otherMax.x - GetMin().x;
        float otherMaxYDiff = otherMax.y - GetMin().y;
        float otherMinXDiff = otherMin.x - GetMax().x;
        float otherMinYDiff = otherMin.y - GetMax().y;
        
        //See which variable is min value and return that side of the "other" object. Also manipulate the offset vector so that "this" directly touches our object and doesn't overlap
        float minimum1 =  Math::Min(Math::Abs(otherMaxXDiff), Math::Abs(otherMaxYDiff));
        float minimum2 =  Math::Min(Math::Abs(otherMinXDiff), Math::Abs(otherMinYDiff));
        float minimum = Math::Min(minimum1, minimum2);

        if (minimum == Math::Abs(otherMaxXDiff))
        {
            offset.x = otherMaxXDiff;
            return CollSide::Right;
        }
        else if (minimum == Math::Abs(otherMaxYDiff))
        {
            offset.y = otherMaxYDiff;
            return CollSide::Bottom;
        }
        else if (minimum == Math::Abs(otherMinXDiff))
        {
            offset.x = otherMinXDiff;
            return CollSide::Left;
        }
        else if (minimum == Math::Abs(otherMinYDiff))
        {
            offset.y = otherMinYDiff;
            return CollSide::Top;
        }
    }
    
    // if “this” does not intersect with “other”
	return CollSide::None;
}
