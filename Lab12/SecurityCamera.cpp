//
//  SecurityCamera.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/13/22.
//

#include "SecurityCamera.hpp"
#include "SecurityCone.hpp"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.hpp"

SecurityCamera::SecurityCamera(class Game* game, Actor* parent)
: Actor(game, parent)
{
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
    
    //Dynamically allocate a new SecurityCone, passing in this as the second parameter to the constructor
    securityCone = new SecurityCone(game, this);
    
}

void SecurityCamera::OnUpdate(float deltaTime)
{
    //Only update the counter if the cone is white
    if (!securityCone->IfYellowCone())
    {
        counter += deltaTime;
    }
    
    if (counter >= interpTime)
    {
        currentQ = endQ;
        endQ = startQ;
        startQ = currentQ;
        counter = 0.0f;
        isPaused = true;
    }
    
    if (!isPaused)
    {
        float normalize = counter / interpTime;
        //Lerp between the start and end Quaternion
        currentQ = Quaternion::Slerp(startQ, endQ, normalize);
        SetQuaternion(currentQ);
    }
    else
    {
        if (counter > pauseTime)
        {
            isPaused = false;
            counter = 0.0f;
        }
    }
}
