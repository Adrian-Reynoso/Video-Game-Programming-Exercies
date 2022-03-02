//
//  Soldier.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/22/22.
//

#include "Soldier.hpp"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "PathNode.h"
#include "SoldierAI.h"
#include "EnemyComponent.hpp"
#include "Effect.hpp"

Soldier::Soldier(class Game* game, class PathNode* start, class PathNode* end)
:Actor(game)
{
    //Allocate a new collision component and Animatedsprite component
    collisionComponent = new CollisionComponent(this);
    collisionComponent->SetSize(32.0f, 32.0f);
    
    spriteComponent = new AnimatedSprite(this);
    spriteComponent->LoadAnimations("Assets/Soldier");
    spriteComponent->SetAnimation("WalkDown");
    spriteComponent->SetAnimFPS(5.0f);
    
    //Create a SoldierAI component and call the Setup function on it, passing in the start/end nodes respectively
    soldierAI = new SoldierAI(this);
    soldierAI->Setup(start, end);
    
    //Create the enemy component and set it's health to 2
    enemyComponent = new EnemyComponent(this);
    enemyComponent->SetEnemyHitPoints(2);
    
    enemyComponent->SetOnDamage([this](){
        //When Damage Occurs pause the movement for 1.0s
        soldierAI->StunSoldier();
        
        //Set up effect with respective parameters
        effect = new Effect(GetGame(), GetPosition(), "Hit", "Assets/Sounds/EnemyHit.wav");
    });
    
    enemyComponent->SetOnDeath([this](){        
        //Set up effect with respective parameters
        effect = new Effect(GetGame(), GetPosition(), "Death", "Assets/Sounds/EnemyDie.wav");
    });
}
