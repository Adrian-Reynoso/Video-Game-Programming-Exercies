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
#include "Bullet.hpp"
#include "CollisionComponent.h"
#include "Block.hpp"
#include "HUD.h"
#include "Random.h"

PlayerMove::PlayerMove(class Player* owner)
: MoveComponent(owner)
{
    mPlayer = owner;
    
    //Initially create blocks up to 3500
    for (int i = 0; i <= 3500; i += 500)
    {
        lastBlockPosition = (float)i;
        
        loadBlock(500.0f, 0.0f, sideIndexPattern[indexCounter % sideIndexPattern.size()], true);
        loadBlock(-500.0f, 0.0f, sideIndexPattern[indexCounter % sideIndexPattern.size()], false);
        loadBlock(0.0f, -500.0f, 5, false);
        loadBlock(0.0f, 500.0f, topIndexPattern[indexCounter % topIndexPattern.size()], false);
        
        indexCounter++;
    }
}

void PlayerMove::Update(float deltaTime)
{
    //Speed Multiplier check
    speedCount += deltaTime;
    if (speedCount >= 10.0f)
    {
        speedCount = 0.0f;
        speedMultiplier += 0.15f;
    }
    
    //All barrell roll related functionality
    peppyCooldown -= deltaTime;
    handlePeppyCooldown();
    trackBarrelRoll(deltaTime);
    
    //Update Position
    Vector3 tempPos = mPlayer->GetPosition();
    tempPos += (direction * velocity * speedMultiplier * deltaTime);
    tempPos.y = Math::Clamp(tempPos.y, -180.0f, 180.0f);
    tempPos.z = Math::Clamp(tempPos.z, -225.0f, 225.0f);
    mPlayer->SetPosition(tempPos);
    
    //Edit the eye position and then use that for the target position
    Vector3 eyePos = tempPos - (mPlayer->GetForward() * HDist) + (Vector3::UnitZ * VDist);
    eyePos.z = 0.0f;
    Vector3 targetPos = tempPos + (mPlayer->GetForward() * TargetDist);
    
    //create Matrix4::CreateLookAt and call on renderer
    Matrix4 lookAt = Matrix4::CreateLookAt(eyePos, targetPos, Vector3::UnitZ);
    mPlayer->GetGame()->GetRenderer()->SetViewMatrix(lookAt);
    
    //Create a new SideBlock group to maintain the endless spawning
    if (mPlayer->GetPosition().x >= spawnCheckpoint)
    {
        spawnCheckpoint += 500.0f;
        
        lastBlockPosition += 500;
        
        loadBlock(500.0f, 0.0f, sideIndexPattern[indexCounter % sideIndexPattern.size()], true);
        loadBlock(-500.0f, 0.0f, sideIndexPattern[indexCounter % sideIndexPattern.size()], false);
        loadBlock(0.0f, -500.0f, 5, false);
        loadBlock(0.0f, 500.0f, topIndexPattern[indexCounter % topIndexPattern.size()], false);
        
        indexCounter++;
    }
    
    //For health/damage functionality
    isShipDamaged();
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Grab the state of the entire Keyboard and determine movement
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    if (!keyboardState[SDL_SCANCODE_W] || !keyboardState[SDL_SCANCODE_S])
    {
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
    
    if (!keyboardState[SDL_SCANCODE_A] || !keyboardState[SDL_SCANCODE_D])
    {
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
    
    //For shooting bullets
    if (keyboardState[SDL_SCANCODE_SPACE] && !spaceIsPressed)
    {
        bullet = new Bullet(mPlayer->GetGame());
        bullet->SetPosition(mPlayer->GetPosition());
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Shoot.wav"), 0);
    }
    
    //For Barrel Roll
    if (keyboardState[SDL_SCANCODE_Q] && !qIsPressed && !isRolling)
    {
        if (mPlayer->shieldLevel < 3)
        {
            mPlayer->shieldLevel++;
            
            if (mPlayer->shieldLevel > 1)
            {
                Mix_Pause(mPlayer->GetGame()->soundChannel2);
            }
        }
        
        isRolling = true;
        
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Boost.wav"), 0);
    }
                      
    //Update bool values for leading edges
    spaceIsPressed = keyboardState[SDL_SCANCODE_SPACE];
    qIsPressed = keyboardState[SDL_SCANCODE_Q];
}

void PlayerMove::DestroyExplodingBlock(class Block* block)
{
    //Delete the exploding block and all others in a 50 unit radius
    block->SetState(ActorState::Destroy);
    
    //Remove the current block from the blockVector so it doesn't intersect with itself
    mPlayer->GetGame()->RemoveBlock(block);
    
    //Check if the explosion collides with any other blocks. If so, destroy the blocks
    for (Block* otherBlock : mPlayer->GetGame()->GetBlockVector())
    {
        if (Vector3::Distance(block->GetPosition(), otherBlock->GetPosition()) <= 50.0f)
        {
            if (otherBlock->explodingType)
            {
                DestroyExplodingBlock(otherBlock);
            }
            else
            {
                otherBlock->SetState(ActorState::Destroy);
            }
        }
    }
}

void PlayerMove::handlePeppyCooldown()
{
    //Check if this cooldown is less than zero and shieldHealth isn't full
    if (peppyCooldown <= 0.0f && mPlayer->shieldLevel != 3)
    {
        mPlayer->hud->DoABarrelRoll();
        peppyCooldown = Random::GetFloatRange(15.0f, 25.0f);
    }
}

void PlayerMove::trackBarrelRoll(float deltaTime)
{
    //Add to the rollCount if ship is rolling
    if (isRolling)
    {
        rollCount += deltaTime;
        
        if (rollCount >= 0.5f)
        {
            //Stop rolling
            rollCount = 0.0f;
            mPlayer->SetRollAngle(0.0f);
            isRolling = false;
        }
        else
        {
            mPlayer->SetRollAngle(mPlayer->GetRollAngle() + 8.0f * Math::Pi * deltaTime);
        }
    }
}

void PlayerMove::loadBlock(float pos1, float pos2, int indexPattern, bool isRotated)
{
    //Check which type of sideBlock it is, and to the respective actions
    SideBlock* sideBlock = new SideBlock(mPlayer->GetGame(), indexPattern);
    
    sideBlock->SetPosition(Vector3{lastBlockPosition, pos1, pos2});
    
    if (isRotated)
    {
        sideBlock->SetRotation(Math::Pi);
    }
}

void PlayerMove::isShipDamaged()
{
    gotDamaged = false;
    
    //If the player collides with any of the blocks, those blocks get destroyed (and exploded)
    for (Block* block : mPlayer->GetGame()->GetBlockVector())
    {
        if (mPlayer->GetCollisionComponent()->Intersect(block->collisionComponent))
        {
            gotDamaged = true;
            
            if (block->explodingType)
            {
                DestroyExplodingBlock(block);
            }
            else
            {
                block->SetState(ActorState::Destroy);
            }
        }
    }
    
    //Check if damaged with damage function. If so, decrease shieldLevel
    if (gotDamaged)
    {
        damageEffects();
    }
}

void PlayerMove::damageEffects()
{
    mPlayer->shieldLevel--;
    
    //Check if shield Level == 0. If so, pause ship.
    if (mPlayer->shieldLevel == 0)
    {
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
        Mix_Pause(mPlayer->GetGame()->soundChannel1);
        Mix_Pause(mPlayer->GetGame()->soundChannel2);
        mPlayer->SetState(ActorState::Paused);
    }
    else if (mPlayer->shieldLevel == 1)
    {
        Mix_Resume(mPlayer->GetGame()->soundChannel2);
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/ShipHit.wav"), 0);
    }
    else
    {
        Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/ShipHit.wav"), 0);
    }
}
