//
//  EnemyComponent.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/28/22.
//

#include "EnemyComponent.hpp"
#include "Actor.h"
#include "Game.h"
#include <vector>
#include "CollisionComponent.h"

EnemyComponent::EnemyComponent(class Actor* owner)
:Component(owner)
{
    //Add itself to the game’s enemy vector
    owner->GetGame()->AddEnemyComponent(this);
    
    //Then use GetComponent to get the collision component of the owner and save it in the member variable
    collisionComponent = mOwner->GetComponent<CollisionComponent>();
}

EnemyComponent::~EnemyComponent()
{
    mOwner->GetGame()->RemoveEnemyComponent(this);
}

void EnemyComponent::Update(float deltaTime)
{
    //Update takeDamageCooldown if takeDamage was called
    takeDamageCooldown += deltaTime;
}

void EnemyComponent::TakeDamage()
{
    //Do this as long as you didnt call the function 0.25 seconds ago
    if(takeDamageCooldown > 0.25)
    {
        //reset takeDamageCooldown
        takeDamageCooldown = 0.0f;
        
        //Reduce enemy hit points by 1
        enemyHitPoints--;
        
        //If the hit points hits 0, then the enemy should die
        if (enemyHitPoints == 0)
        {
            mOwner->SetState(ActorState::Destroy);
        }
    }
}
