//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/3/22.
//

#include "PlayerMove.hpp"
#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "SideBlock.hpp"

PlayerMove::PlayerMove(class Player* owner)
: MoveComponent(owner)
{
    mPlayer = owner;
    
    //Initially create blocks up to 3500
    for (int i = 0; i <= 3500; i += 500)
    {
        //Create four side block
        SideBlock* side1 = new SideBlock(mPlayer->GetGame(), sideIndexPattern[indexCounter % sideIndexPattern.size()]);
        side1->SetPosition(Vector3{(float)i, 500.0f, 0.0f});
        side1->SetRotation(Math::Pi);
        SideBlock* side2 = new SideBlock(mPlayer->GetGame(), sideIndexPattern[indexCounter % sideIndexPattern.size()]);
        side2->SetPosition(Vector3{(float)i, -500.0f, 0.0f});
        SideBlock* side3 = new SideBlock(mPlayer->GetGame(), 5);
        side3->SetPosition(Vector3{(float)i, 0.0f, -500.0f});
        SideBlock* side4 = new SideBlock(mPlayer->GetGame(), topIndexPattern[indexCounter % topIndexPattern.size()]);
        side4->SetPosition(Vector3{(float)i, 0.0f, 500.0f});
        
        //Update the index counter
        indexCounter++;
    }
}

void PlayerMove::Update(float deltaTime)
{
    //Make a temp variable for the position
    Vector3 tempPos = mPlayer->GetPosition();
    
    //Update the players movement
    tempPos += (direction * velocity * deltaTime);
    
    //Clamp the ship position so it doesn't go off screen for y and z components
    tempPos.y = Math::Clamp(tempPos.y, -180.0f, 180.0f);
    tempPos.z = Math::Clamp(tempPos.z, -225.0f, 225.0f);
    
    //Assign the value of tempPos to the PLayer Position
    mPlayer->SetPosition(tempPos);
    
    //Edit the eye position and then use that for the target position
    Vector3 eyePos = tempPos - (mPlayer->GetForward() * HDist) + (Vector3::UnitZ * VDist);
    eyePos.z = 0.0f;
    Vector3 targetPos = tempPos + (mPlayer->GetForward() * TargetDist);
    
    //Call Matrix4::CreateLookAt, passing in these two as parameters well as Vector3::UnitZ for the up vector
    Matrix4 lookAt = Matrix4::CreateLookAt(eyePos, targetPos, Vector3::UnitZ);
    
    //Once you create the matrix, call the SetViewMatrix on the renderer
    mPlayer->GetGame()->GetRenderer()->SetViewMatrix(lookAt);
    
    //Create a new SideBlock group to maintain the endless spawning
    if (mPlayer->GetPosition().x >= spawnCheckpoint)
    {
        //Update spawnCheckpoint to the next position 500 units ahead
        spawnCheckpoint += 500.0f;
        
        //Create the blocks +500 the last Block position
        lastBlockPosition += 500;
        SideBlock* side1 = new SideBlock(mPlayer->GetGame(), sideIndexPattern[indexCounter % sideIndexPattern.size()]);
        side1->SetPosition(Vector3{lastBlockPosition, 500.0f, 0.0f});
        side1->SetRotation(Math::Pi);
        SideBlock* side2 = new SideBlock(mPlayer->GetGame(), sideIndexPattern[indexCounter % sideIndexPattern.size()]);
        side2->SetPosition(Vector3{lastBlockPosition, -500.0f, 0.0f});
        SideBlock* side3 = new SideBlock(mPlayer->GetGame(), 5);
        side3->SetPosition(Vector3{lastBlockPosition, 0.0f, -500.0f});
        SideBlock* side4 = new SideBlock(mPlayer->GetGame(), topIndexPattern[indexCounter % topIndexPattern.size()]);
        side4->SetPosition(Vector3{lastBlockPosition, 0.0f, 500.0f});
        
        //Update the index counter
        indexCounter++;
    }
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Based off the keystate, determine the direction of the our member variable which stores direction
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    //As long as both W and S aren't pressed at the same time
    if (!keyboardState[SDL_SCANCODE_W] || !keyboardState[SDL_SCANCODE_S])
    {
        //Check which one is pressed, and edit the direction
        if(keyboardState[SDL_SCANCODE_W])
        {
            direction.z = 300.0f;
        }
        else if (keyboardState[SDL_SCANCODE_S])
        {
            direction.z = -300.0f;
        }
        else
        {
            direction.z = 0.0f;
        }
    }
    
    //As long as both A and D aren't pressed at the same time
    if (!keyboardState[SDL_SCANCODE_A] || !keyboardState[SDL_SCANCODE_D])
    {
        //Check which one is pressed, and edit the direction
        if(keyboardState[SDL_SCANCODE_D])
        {
            direction.y = 300.0f;
        }
        else if (keyboardState[SDL_SCANCODE_A])
        {
            direction.y = -300.0f;
        }
        else
        {
            direction.y = 0.0f;
        }
    }
    
    
    
    
}
