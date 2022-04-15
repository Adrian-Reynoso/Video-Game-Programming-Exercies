//
//  SecurityCone.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/13/22.
//

#include "SecurityCone.hpp"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.hpp"

SecurityCone::SecurityCone(class Game* game, Actor* parent)
: Actor(game, parent)
{
    meshComponent = new MeshComponent(this, true);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));
    
    //Set the actor’s position to (30, 0, 0) and the scale to 3.0f
    SetPosition(Vector3{30.0f, 0.0f, 0.0f});
    SetScale(3.0f);
    
    //Set the correct height of cone by the factor of the player scale
    heightByScale = 100.0f * mScale;
}

void SecurityCone::OnUpdate(float deltaTime)
{
    //PROXIMITY CHECK: Using the current position and forward of the cone, see if the player is at a closer distance to the position than the farthest end of the cone mesh to check for proximity
    Vector3 farthestPointOfCone = GetWorldPosition() + (GetWorldForward() * heightByScale);
    float playerToCamera = Vector3::Distance(mGame->GetPlayer()->GetPosition(), GetWorldPosition());
    float farthestPointToCamera =  Vector3::Distance(farthestPointOfCone, GetWorldPosition());
    
    if (playerToCamera <= farthestPointToCamera)
    {
        isInProximity = true;
    }
    else
    {
        isInProximity = false;
    }
    
    //FOV CHECK: See if the player is in the field of view of the camera
    Vector3 cameraToPlayer = mGame->GetPlayer()->GetPosition() - GetWorldPosition();
    cameraToPlayer.Normalize();
    
    //Calculate the angle from the vehicle direction to frog using dot product
    float angle = Math::Acos(Vector3::Dot(GetWorldForward(), cameraToPlayer));
    
    //See if this calculated angle is less than the half angle of the cone. If so, player is in FOV
    if (angle <= halfAngleOfCone)
    {
        isInFOV = true;
    }
    else
    {
        isInFOV = false;
    }
    
    //If both checks pass, then the player is being detected by the camera
    if (playerIsDetected)
    {
        //Increment the countdown more for the half second respawn delay
        cameraCountdown += deltaTime;
        
        if (cameraCountdown >= 2.5f)
        {
            cameraCountdown = 0.0f;
            playerIsDetected = false;
            mGame->GetPlayer()->SetToBeRespawned(true);
        }
        
    }
    else if (isInProximity && isInFOV)
    {
        meshComponent->SetTextureIndex(1);
        
        //Increment the countdown
        cameraCountdown += deltaTime;
        
        //Check if the camera countdown is more than two seconds. If so, turn cone red
        if (cameraCountdown >= 2.0f)
        {
            meshComponent->SetTextureIndex(2);
            playerIsDetected = true;
        }
    }
    else
    {
        meshComponent->SetTextureIndex(0);
        
        //Reset the countdown
        cameraCountdown = 0.0f;
    }
}
