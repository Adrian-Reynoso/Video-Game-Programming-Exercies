//
//  WrappingMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#include "WrappingMove.hpp"
#include "Actor.h"

WrappingMove::WrappingMove(class Actor* owner, float dir)
:MoveComponent(owner)
{
    //Assign the direction to our vector2
    direction.x *= dir;
}

void WrappingMove::Update(float deltaTime)
{
    //Updates the owning actor’s position based on the owning actor’s forward vector
    //find velocity
    Vector2 velocity = mOwner->GetForward() * mForwardSpeed * direction;
    mOwner->SetPosition(mOwner->GetPosition() + (velocity * deltaTime));
    
    //If position is passed screen edges, wrap the sprite
    //Off of left screen
    if (mOwner->GetPosition().x < 0)
    {
        mOwner->SetPosition(Vector2{screenWidth, mOwner->GetPosition().y});
    }
    //Off of right screen
    else if (mOwner->GetPosition().x > screenWidth)
    {
        mOwner->SetPosition(Vector2{0, mOwner->GetPosition().y});
    }
}
