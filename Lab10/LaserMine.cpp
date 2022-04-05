//
//  LaserMine.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/4/22.
//

#include "LaserMine.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "LaserComponent.hpp"

LaserMine::LaserMine(class Game* game)
: Actor(game)
{
    MeshComponent* meshComponent = new MeshComponent(this);
    meshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
    
    laserComponent = new LaserComponent(this);
}
