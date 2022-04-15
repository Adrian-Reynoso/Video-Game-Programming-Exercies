//
//  Arrow.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/12/22.
//

#include "Arrow.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Checkpoint.hpp"
#include "Player.hpp"

Arrow::Arrow(class Game* game, Actor* parent)
: Actor(game, parent)
{
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
    
    SetScale(0.15f);
}

void Arrow::OnUpdate(float deltaTime)
{
    //Make a vector from the player position to the active checkpoint, and normalize it
    Vector3 playerToCheck;
    if (!mGame->checkpointQueue.empty())
    {
        //Make a vector from the player position to the active checkpoint, and normalize it
        playerToCheck = mGame->checkpointQueue.front()->GetPosition() - mGame->GetPlayer()->GetPosition();
        playerToCheck.Normalize();
    }
    
    //Use the dot product/cross product between Vector3::UnitX and the player-to-checkpoint vector to compute the angle of rotation and the axis of rotation
    
    //Find rotation from Quaternion
    Matrix4 rotation;
    Vector3 oldFacing = Vector3::UnitX;
    oldFacing.Normalize();
    
    //Check dot product to see if it's 1 or -1
    if ( mGame->checkpointQueue.empty() || Vector3::Dot(oldFacing, playerToCheck) == 1)
    {
        SetQuaternion(Quaternion::Identity);
    }
    else if (Vector3::Dot(oldFacing, playerToCheck) == -1)
    {
        SetQuaternion(Quaternion(Vector3::UnitZ, Math::Pi));
    }
    else
    {
        float angle = Math::Acos(Vector3::Dot(oldFacing, playerToCheck));
        Vector3 axis = Vector3::Cross(oldFacing, playerToCheck);
        axis.Normalize();
        
        SetQuaternion(Quaternion(axis, angle));
    }
    
    
    //Set position of array to unproject
    SetPosition(mGame->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f)));
}
