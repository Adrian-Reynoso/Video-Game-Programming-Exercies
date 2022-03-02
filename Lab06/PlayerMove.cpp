//
//  PlayerMove.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "Player.hpp"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "Collider.hpp"
#include "EnemyComponent.hpp"
#include "Sword.hpp"

PlayerMove::PlayerMove(class Player* owner)
:MoveComponent(owner)
{
    mPlayer = owner;
    ownerCollisionComponent = mOwner->GetComponent<CollisionComponent>();
    
    //Create an instance of sword
    sword = new Sword(owner->GetGame());
}

void PlayerMove::Update(float deltaTime)
{
    //Set camera position to player position plus an offset of (-256.0f, -224.0f)
    mOwner->GetGame()->cameraPosition = mPlayer->GetPosition() + Vector2{-256.0f, -224.0f};
    
    //Update the players movement
    mPlayer->SetPosition(mPlayer->GetPosition() + (playerDir * mVelocity * deltaTime));
    
    //Update Attack cooldown if attack is in progress
    if (attackCooldown >= 0.0f)
    {
        attackCooldown += deltaTime;
    }
    
    //Check the swords position and collision component
    setSwordPos();
    
    //Check if Link collides with enemies
    Vector2 tempPos = mPlayer->GetPosition();
    CollSide onObject = CollSide::None;
    for (EnemyComponent* enemy : mOwner->GetGame()->GetEnemyComponentVector())
    {
        //Check if the player has an active attack
        if (attackCooldown < 0.25f)
        {
            //Test whether the Sword intersects with that enemy, and if it does, call TakeDamage
            if (enemy->GetCollisionComponent()->Intersect(sword->collisionComponent))
            {
                enemy->TakeDamage();
            }
        }
        
        Vector2 offSet {0.0f, 0.0f};
        onObject = ownerCollisionComponent->GetMinOverlap(enemy->GetCollisionComponent(), offSet);

        if (onObject != CollSide::None)
        {
            //Add offset to temPos
            tempPos += offSet;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);
        }
    }
    
    //Check if Link collides with objects
    tempPos = mPlayer->GetPosition();
    onObject = CollSide::None;
    for (Collider* collider : mOwner->GetGame()->GetColliderVector())
    {
        Vector2 offSet {0.0f, 0.0f};
        onObject = ownerCollisionComponent->GetMinOverlap(collider->collisionComponent, offSet);

        if (onObject != CollSide::None)
        {
            //Add offset to temPos
            tempPos += offSet;
            
            //Call setPosition
            mOwner->SetPosition(tempPos);
        }
    }
    
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
    //Based off the keystate, determine the direction of the our member variable which stores direction
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    
    //Assume player is moving for right now (If one of the movement keys aren't pressed, then set to false)
    isMoving = true;

    if (keyboardState[SDL_SCANCODE_SPACE] && isSpacePressed == false && attackCooldown > 0.25)
    {
        playerDir.x = 0.0f;
        playerDir.y = 0.0f;
        
        //Call ResetAnimTimer on the AnimatedSprite to ensure the attack animation will always start on the first frame
        mPlayer->spriteComponent->ResetAnimTimer();
        
        //Set attack cooldown to 0 to start its phase
        attackCooldown = 0.0f;
        
        //Play Sound effect for attacking
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/SwordSlash.wav"), 0);
    }
    else if (keyboardState[SDL_SCANCODE_DOWN] && attackCooldown > 0.25f)
    {
        playerDir.x = 0.0f;
        playerDir.y = 1.0f;
        playerState = Direction::down;
    }
    else if (keyboardState[SDL_SCANCODE_UP] && attackCooldown > 0.25f)
    {
        playerDir.x = 0.0f;
        playerDir.y = -1.0f;
        playerState = Direction::up;
    }
    else if (keyboardState[SDL_SCANCODE_LEFT] && attackCooldown > 0.25f)
    {
        playerDir.x = -1.0f;
        playerDir.y = 0.0f;
        playerState = Direction::left;
    }
    else if (keyboardState[SDL_SCANCODE_RIGHT] && attackCooldown > 0.25f)
    {
        playerDir.x = 1.0f;
        playerDir.y = 0.0f;
        playerState = Direction::right;
    }
    else
    {
        playerDir.x = 0.0f;
        playerDir.y = 0.0f;
        isMoving = false;
    }
    
    //Call SetAnim() to play the corresponding animation
    SetAnim();
    
    //For the leading edge of Pressing space bar
    isSpacePressed = keyboardState[SDL_SCANCODE_SPACE];
}

void PlayerMove::SetAnim()
{
    //Based off the velocity and playerState, play the right animation
    //first determine if player is moving or not
    if (playerDir.x == 0.0f && playerDir.y == 0.0f)
    {
        if (attackCooldown < 0.25f && attackCooldown >= 0.0f)
        {
            //Determine the right animation based off the direction link is facing
            if (playerState == Direction::up && mPlayer->spriteComponent->GetAnimName() != "AttackUp")
            {
                mPlayer->spriteComponent->SetAnimation("AttackUp");
            }
            else if (playerState == Direction::down && mPlayer->spriteComponent->GetAnimName() != "AttackDown")
            {
                mPlayer->spriteComponent->SetAnimation("AttackDown");
            }
            else if (playerState == Direction::left && mPlayer->spriteComponent->GetAnimName() != "AttackLeft")
            {
                mPlayer->spriteComponent->SetAnimation("AttackLeft");
            }
            else if (playerState == Direction::right && mPlayer->spriteComponent->GetAnimName() != "AttackRight")
            {
                mPlayer->spriteComponent->SetAnimation("AttackRight");
            }
        }
        else
        {
            //Determine the right animation based off the direction link is facing
            if (playerState == Direction::up && mPlayer->spriteComponent->GetAnimName() != "StandUp")
            {
                mPlayer->spriteComponent->SetAnimation("StandUp");
            }
            else if (playerState == Direction::down && mPlayer->spriteComponent->GetAnimName() != "StandDown")
            {
                mPlayer->spriteComponent->SetAnimation("StandDown");
            }
            else if (playerState == Direction::left && mPlayer->spriteComponent->GetAnimName() != "StandLeft")
            {
                mPlayer->spriteComponent->SetAnimation("StandLeft");
            }
            else if (playerState == Direction::right && mPlayer->spriteComponent->GetAnimName() != "StandRight")
            {
                mPlayer->spriteComponent->SetAnimation("StandRight");
            }
        }
    }
    else
    {
        //Determine the right animation based off the direction link is facing
        if (playerState == Direction::up && mPlayer->spriteComponent->GetAnimName() != "WalkUp")
        {
            mPlayer->spriteComponent->SetAnimation("WalkUp");
        }
        else if (playerState == Direction::down && mPlayer->spriteComponent->GetAnimName() != "WalkDown")
        {
            mPlayer->spriteComponent->SetAnimation("WalkDown");
        }
        else if (playerState == Direction::left && mPlayer->spriteComponent->GetAnimName() != "WalkLeft")
        {
            mPlayer->spriteComponent->SetAnimation("WalkLeft");
        }
        else if (playerState == Direction::right && mPlayer->spriteComponent->GetAnimName() != "WalkRight")
        {
            mPlayer->spriteComponent->SetAnimation("WalkRight");
        }
    }
}

void PlayerMove::setSwordPos()
{
    //Check the swords position and collision component
    Vector2 tempPos = mPlayer->GetPosition();
    if (playerState == up)
    {
        sword->SetPosition(tempPos + Vector2{0.0f, -40.0f});
        sword->collisionComponent->SetSize(20.0f, 28.0f);
    }
    else if (playerState == down)
    {
        sword->SetPosition(tempPos + Vector2{0.0f, 40.0f});
        sword->collisionComponent->SetSize(20.0f, 28.0f);
    }
    else if (playerState == left)
    {
        sword->SetPosition(tempPos + Vector2{-32.0f, 0.0f});
        sword->collisionComponent->SetSize(28.0f, 20.0f);
    }
    else if (playerState == right)
    {
        sword->SetPosition(tempPos + Vector2{32.0f, 0.0f});
        sword->collisionComponent->SetSize(28.0f, 20.0f);
    }
}
