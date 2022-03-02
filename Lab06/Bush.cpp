//
//  Bush.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#include "Bush.hpp"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "EnemyComponent.hpp"
#include "PathFinder.h"
#include "Effect.hpp"

Bush::Bush(class Game* game)
:Actor(game)
{
    //Allocate a new collision component and sprite component
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(32.0f, 32.0f);
    
    spriteComponent = new SpriteComponent(this);
    spriteComponent->SetTexture(game->GetTexture("Assets/Bush.png"));
    
    //Create the enemy component and set health points to 1
    enemyComponent = new EnemyComponent(this);
    enemyComponent->SetEnemyHitPoints(1);
    
    enemyComponent->SetOnDeath([this](){
        PushOnCallback();
        
        //Set up effect with respective parameters
        effect = new Effect(GetGame(), GetPosition(), "BushDeath", "Assets/Sounds/BushDie.wav");
    });
}

Bush::~Bush()
{
    
}

void Bush::PushOnCallback()
{
    //Call the SetIsBlocked function on the PathFinder, passing in false as the 3rd parameter
    //The row is the Bush position.y / 32 and the column is the Bush position.x / 32.
    GetGame()->GetPathFinder()->SetIsBlocked((int)(mPosition.y / 32.0f), (int)(mPosition.x / 32.0f), false);
}
