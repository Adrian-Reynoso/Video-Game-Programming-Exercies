#include "PowerPellet.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Ghost.h"
#include <algorithm>

PowerPellet::PowerPellet(class Game* game)
:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/PowerPellet.png"));
	
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(2.0f, 2.0f);
	
	game->mPowerPellets.emplace_back(this);
}

PowerPellet::~PowerPellet()
{
	auto iter = std::find(mGame->mPowerPellets.begin(),
						  mGame->mPowerPellets.end(),
						  this);
	mGame->mPowerPellets.erase(iter);
}
