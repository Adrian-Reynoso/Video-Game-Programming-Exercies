//
//  Effect.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/1/22.
//

#include "Effect.hpp"
#include "AnimatedSprite.h"
#include "Game.h"

Effect::Effect(class Game* game, Vector2 position, std::string animName, std::string soundName)
:Actor(game)
{
    //Set the position of the actor to the position parameter
    SetPosition(position);
    
    //Create an AnimatedSprite
    animatedSprite = new AnimatedSprite(this);
    animatedSprite->LoadAnimations("Assets/Effects");
    animatedSprite->SetAnimation(animName);
    
    //Use GetAnimDuration to get the time the animation lasts, and save it in the lifetime member variable
    lifetime = animatedSprite->GetAnimDuration(animName);
    
    //Get the sound (from the sound name parameter) and use Mix_PlayChannel to play it
    Mix_PlayChannel(-1, GetGame()->GetSound(soundName), 0);
}

void Effect::OnUpdate(float deltaTime)
{
    //Decrement the lifetime variable by delta time and set the actor state to Destroy once the lifetime is <= 0.0f
    lifetime -= deltaTime;
    
    if (lifetime <= 0.0f)
    {
        SetState(ActorState::Destroy);
    }
}
