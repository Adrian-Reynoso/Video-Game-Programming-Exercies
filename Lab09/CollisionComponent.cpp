#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	offset = Vector3::Zero;

    //If they do intersect, the function returns the CollSide that is the minimum overlap side.
    if (Intersect(other))
    {
        Vector3 otherMax = other->GetMax();
        Vector3 otherMin = other->GetMin();
        
        //Do the calculations and determine which has the least overlap of the four sides
        float otherMaxXDiff = otherMax.x - GetMin().x;
        float otherMaxYDiff = otherMax.y - GetMin().y;
        float otherMaxZDiff = otherMax.z - GetMin().z;
        float otherMinXDiff = otherMin.x - GetMax().x;
        float otherMinYDiff = otherMin.y - GetMax().y;
        float otherMinZDiff = otherMin.z - GetMax().z;
        
        //See which variable is min value and return that side of the "other" object. Also manipulate the offset vector so that "this" directly touches our object and doesn't overlap
        float minimum1 =  Math::Min(Math::Abs(otherMaxXDiff), Math::Abs(otherMaxYDiff));
        minimum1 = Math::Min(minimum1, Math::Abs(otherMaxZDiff));
        float minimum2 =  Math::Min(Math::Abs(otherMinXDiff), Math::Abs(otherMinYDiff));
        minimum2 =  Math::Min(minimum2, Math::Abs(otherMinZDiff));
        float minimum = Math::Min(minimum1, minimum2);

        if (minimum == Math::Abs(otherMaxXDiff))
        {
            offset.x = otherMaxXDiff;
            return CollSide::Front;
        }
        else if (minimum == Math::Abs(otherMaxYDiff))
        {
            offset.y = otherMaxYDiff;
            return CollSide::Right;
        }
        else if (minimum == Math::Abs(otherMaxZDiff))
        {
            offset.z = otherMaxZDiff;
            return CollSide::Top;
        }
        else if (minimum == Math::Abs(otherMinXDiff))
        {
            offset.x = otherMinXDiff;
            return CollSide::Back;
        }
        else if (minimum == Math::Abs(otherMinYDiff))
        {
            offset.y = otherMinYDiff;
            return CollSide::Left;
        }
        else if (minimum == Math::Abs(otherMinZDiff))
        {
            offset.z = otherMinZDiff;
            return CollSide::Bottom;
        }
    }
    
    // if “this” does not intersect with “other”
    return CollSide::None;
}
