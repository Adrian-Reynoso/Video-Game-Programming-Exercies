//
//  Enemy.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/20/22.
//

#include "Enemy.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "EnemyMove.hpp"

Enemy::Enemy(Game* game)
: Actor(game)
{
    //Do necessary initializations
    SetScale(0.75);
    
    meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
    meshComponent->SetTextureIndex(6);
    
    enemyMove = new EnemyMove(this);
    
}
