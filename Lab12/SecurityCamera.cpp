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

SecurityCamera::~SecurityCamera()
{
    if (camSoundChannel != -1)
    {
        Mix_HaltChannel(camSoundChannel);
    }
}

void SecurityCamera::OnUpdate(float deltaTime)
{
    //UPDATE CAMERA SOUNDS IF PLAYING
    if (camSoundChannel != -1)
    {
        Mix_Volume(camSoundChannel, VolumeForPlayerDist());
    }
    
    //Only update the counter if the cone is white
    if (!securityCone->IfYellowCone())
    {
        counter += deltaTime;
        
        //reset soundWhenYellow
        soundWhenYellow = false;
    }
    else
    {
        //Halt the camera moving sound if it is playing
        if (camSoundChannel != -1)
        {
            Mix_HaltChannel(camSoundChannel);
            camSoundChannel = -1;
        }
        
        //Play the Camera Stop sound if it's not already playing
        if (!soundWhenYellow)
        {
            //Play the camStopSoundChannel sound
            Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
            soundWhenYellow = true;
        }
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
        
        //Play sound when camera is rotating
        if (camSoundChannel == -1)
        {
            camSoundChannel = Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
            
            //Set correct volume using playerDist
            Mix_Volume(camSoundChannel, VolumeForPlayerDist());
        }
    }
    else
    {
        if (counter > pauseTime)
        {
            isPaused = false;
            counter = 0.0f;
        }
        
        //Halt camSoundChannel if it is playing
        if (camSoundChannel != -1)
        {
            Mix_HaltChannel(camSoundChannel);
            camSoundChannel = -1;
            
            //Play the camStopSoundChannel sound
            Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
        }
    }
}

int SecurityCamera::VolumeForPlayerDist()
{
    //FOR DETERMINING THE VOLUME OF SOUNDS
    //Determine the distance of the player to the camera
    float playerDist = Vector3::Distance(GetPosition(), mGame->GetPlayer()->GetPosition());
    
    //Use this playerDist to see what volume the individual sounds should be
    if (playerDist > 1500.0f)
    {
        return 0;
    }
    else if (playerDist < 500.0f)
    {
        return 128;
    }
    float section1 = (playerDist - 500.0f)/1000.0f;
    float section2 = 1.0f - section1;
    float volume = (section2) * 128;
    return (int) volume;
}
