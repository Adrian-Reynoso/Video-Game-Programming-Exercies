//
//  Frog.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 1/27/22.
//

#include "Frog.hpp"
#include "SpriteComponent.h"
#include "Game.h"
#include "Math.h"
#include "CollisionComponent.h"
#include "Vehicle.hpp"
#include "Log.hpp"
#include "DeadFrog.hpp"
#include "WrappingMove.hpp"

Frog::Frog(class Game* game, float xPosition, float yPosition)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new SpriteComponent(this);
    
    //Dynamically allocate a collisionComponent and set width/height to 50
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(50.0f, 50.0f);
    
    //Assign ship texture to sprite component
    spriteComponent->SetTexture(game->GetTexture("Assets/Frog.png"));
    
    //Assign the screen dimensions
    screenWidth = game->SCREENWIDTH;
    screenHeight = game->SCREENHEIGHT;
    
    //Define max height so the frog doesn't go backward when game starts
    maxHeight = game->SCREENHEIGHT - 32;
    
    //Assign the game to the private member variable mGame
    mGame = game;
    xPos = xPosition;
    yPos = yPosition;
}

//Override of OnProcessInput
void Frog::OnProcessInput(const Uint8* keyState)
{
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    
    //Create a temporary vector2
    Vector2 temp (GetPosition());
    
    //Check If the UP key was pressed
    if (keyboardState[SDL_SCANCODE_UP] && lastFrame[SDL_SCANCODE_UP] == false)
    {
        //Edit the current position if the move doesn't go off screen
        temp.y -= 64;
            
        if (temp.y < 32.0f)
        {
            temp.y = 32.0f;
        }
        else if (temp.y > maxHeight)
        {
            temp.y = maxHeight;
        }

        //move position 64 pixels
        SetPosition(temp);
        
    }
    
    //Check If the DOWN key was pressed
    else if (keyboardState[SDL_SCANCODE_DOWN] && lastFrame[SDL_SCANCODE_DOWN] == false)
    {
        //Edit the current position if the move doesn't go off screen
        temp.y += 64;
            
        if (temp.y < 32.0f)
        {
            temp.y = 32.0f;
        }
        else if (temp.y > maxHeight - 64)
        {
            temp.y = maxHeight - 64;
        }

        //move position 64 pixels
        SetPosition(temp);
        
    }
    
    //Check If the RIGHT key was pressed
    else if (keyboardState[SDL_SCANCODE_RIGHT] && lastFrame[SDL_SCANCODE_RIGHT] == false)
    {
        //Edit the current position if the move doesn't go off screen
        if (temp.x + 96 < screenWidth)
        {
            temp.x += 64;
            
            //move position 64 pixels
            SetPosition(temp);
        }
        
    }
    
    //Check If the LEFT key was pressed
    else if (keyboardState[SDL_SCANCODE_LEFT] && lastFrame[SDL_SCANCODE_LEFT] == false)
    {
        //Edit the current position if the move doesn't go off screen
        if (temp.x - 64 > 0)
        {
            temp.x -= 64;
            
            //move position 64 pixels
            SetPosition(temp);
        }
        
    }
    
    //Save the currentstate of input to last frame
    lastFrame[SDL_SCANCODE_UP] = keyboardState[SDL_SCANCODE_UP];
    lastFrame[SDL_SCANCODE_DOWN] = keyboardState[SDL_SCANCODE_DOWN];
    lastFrame[SDL_SCANCODE_LEFT] = keyboardState[SDL_SCANCODE_LEFT];
    lastFrame[SDL_SCANCODE_RIGHT] = keyboardState[SDL_SCANCODE_RIGHT];
}

void Frog::OnUpdate(float deltaTime)
{
    //Check if frog collides with a vehicle
    for (Vehicle* vehicle : mGame->vehicles)
    {
        bool collides = collisionComponent->Intersect(vehicle->collisionComponent);
        
        //check if the frog collides, if so move the frog back to starting position
        if (collides == true)
        {
            //Create a DeadFrog actor at the spot where this frog died
            deadFrog = new DeadFrog(mGame, GetPosition().x, GetPosition().y);
            
            //Reset the position of the frog
            SetPosition(Vector2(xPos, yPos));
        }
    }
    
    //Check if frog collides with a log
    CollSide onLog = CollSide::None;
    bool collisionWithLog = false;
    for (Log* log : mGame->logs)
    {
        Vector2 offSet {0.0f, 0.0f};
        onLog = collisionComponent->GetMinOverlap(log->collisionComponent, offSet);
        
        //Check if there was a collision, if so make the frog ride the log
        if (onLog != CollSide::None)
        {
            collisionWithLog = true;
            //Make frog and logs y Position the same
            SetPosition(Vector2 {GetPosition().x, log->GetPosition().y});
            
            //Move the position of the frog based on the log’s WrappingMove direction, forward speed, and delta time
            //Updates the owning actor’s position based on the owning actor’s forward vector
            Vector2 direction;
            direction = log->wrappingMove->direction * log->GetForward();
            
            Vector2 velocity = log->wrappingMove->GetForwardSpeed() * direction;
            SetPosition(GetPosition() + (velocity * deltaTime));
            
            //Check if CollSide is either left or right, additionally add offset.x + either positive or negative 32 (depending on Left or Right) to the frog’s x-position
            if (onLog == CollSide::Left)
            {
                SetPosition(Vector2 {GetPosition().x + offSet.x + 32.0f, GetPosition().y});
            }
            else if (onLog == CollSide::Right)
            {
                SetPosition(Vector2 {GetPosition().x + offSet.x - 32.0f, GetPosition().y});
            }

        }
        
        //Check if frog position is outside of screen barrier and clamp the values
        Vector2 temp (GetPosition());
        
        if (temp.y < 32.0f)
        {
            temp.y = 32.0f;
            //move position 64 pixels
            SetPosition(temp);
        }
        else if (temp.y > maxHeight)
        {
            temp.y = maxHeight;
            //move position 64 pixels
            SetPosition(temp);
        }

        if (temp.x > screenWidth)
        {
            temp.x = screenWidth - 32;
            
            //move position 64 pixels
            SetPosition(temp);
        }
        else if (temp.x < 0)
        {
            temp.x = 0.0f;
            
            //move position 64 pixels
            SetPosition(temp);
        }
    }
    
    //Check if frog collides with goal
    if (collisionComponent->Intersect(mGame->goal->GetComponent<CollisionComponent>()))
    {
        //Set frogs position as the goals and SetState to ActorState::Paused
        SetPosition(mGame->goal->GetPosition());
        SetState(ActorState::Paused);
    }
    
    //If frog is in y-position water area and not on a log, Kill it
    if (collisionWithLog == false && GetPosition().y >= 0 && GetPosition().y <= 510)
    {
        //Create a DeadFrog actor at the spot where this frog died
        deadFrog = new DeadFrog(mGame, GetPosition().x, GetPosition().y);
        
        //Reset the position of the frog
        SetPosition(Vector2(xPos, yPos));
    }
}
