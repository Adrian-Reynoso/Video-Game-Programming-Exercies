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
#include <functional>

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
        void SetOnDamage(std::function<void()> value)
        {
            OnDamage = value;
        }
        void SetOnDeath(std::function<void()> value)
        {
            OnDeath = value;
        }
    
        
    private:
        class CollisionComponent* collisionComponent = nullptr;
        int enemyHitPoints = 0;;
        float takeDamageCooldown = 1.0f;
        std::function<void()> OnDamage;
        std::function<void()> OnDeath;
};

#endif /* EnemyComponent_hpp */
