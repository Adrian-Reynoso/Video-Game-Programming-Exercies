//
//  Player.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/3/22.
//

#include "Player.hpp"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "PlayerMove.hpp"
#include "Game.h"

Player::Player(class Game* game)
:Actor(game)
{
    // TODO: Add code here
    //Dynamically allocate a SpriteComponent and assign it to the member variables
    spriteComponent = new AnimatedSprite(this, 200);
    spriteComponent->SetTexture(game->GetTexture("Assets/Mario/Idle.png"));
    
    //Dynamically allocate a collisionComponent
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(32.0f, 32.0f);
    
    //Create a PlayerMove in the Player constructor
    playerMove = new PlayerMove(this);
    
    //Establish all animations Mario will be using
    std::vector<SDL_Texture*> idleAnim{
        GetGame()->GetTexture("Assets/Mario/Idle.png")
    };
    std::vector<SDL_Texture*> deadAnim{
        GetGame()->GetTexture("Assets/Mario/Dead.png")
    };
    std::vector<SDL_Texture*> jumpLeftAnim{
        GetGame()->GetTexture("Assets/Mario/JumpLeft.png")
    };
    std::vector<SDL_Texture*> jumpRightAnim{
        GetGame()->GetTexture("Assets/Mario/JumpRight.png")
    };
    std::vector<SDL_Texture*> runLeftAnim{
        GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
        GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
        GetGame()->GetTexture("Assets/Mario/RunLeft2.png")
    };
    std::vector<SDL_Texture*> runRightAnim{
        GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
        GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
        GetGame()->GetTexture("Assets/Mario/RunRight2.png")
    };

    //Add the animations to the AnimatedSprite vector
    spriteComponent->AddAnimation("idle", idleAnim);
    spriteComponent->AddAnimation("dead", deadAnim);
    spriteComponent->AddAnimation("jumpLeft", jumpLeftAnim);
    spriteComponent->AddAnimation("jumpRight", jumpRightAnim);
    spriteComponent->AddAnimation("runLeft", runLeftAnim);
    spriteComponent->AddAnimation("runRight", runRightAnim);
    
    //Set the current animation to idle
    spriteComponent->SetAnimation("idle");
}
