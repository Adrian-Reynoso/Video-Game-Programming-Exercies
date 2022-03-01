//
//  EnemyComponent.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 2/28/22.
//

#ifndef EnemyComponent_hpp
#define EnemyComponent_hpp

#include <stdio.h>
#include "Component.h"

class EnemyComponent : public Component
{
    public:
        EnemyComponent(class Actor* owner);
        ~EnemyComponent();
        void Update(float deltaTime) override;
        class CollisionComponent* GetCollisionComponent()
        {
            return collisionComponent;
        }
        void SetEnemyHitPoints(int num)
        {
            enemyHitPoints = num;
        }
        int GetEnemyHitPoints()
        {
            return enemyHitPoints;
        }
        void TakeDamage();
    
        
    private:
        class CollisionComponent* collisionComponent;
        int enemyHitPoints;
        float takeDamageCooldown = 1.0f;
};

#endif /* EnemyComponent_hpp */
