#include "PacManMove.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include "Ghost.h"
#include "PacMan.h"
#include "PowerPellet.h"
#include "Pellet.h"

PacManMove::PacManMove(class Actor* owner)
:MoveComponent(owner)
,mPrevNode(nullptr)
{
	mChompSound = Mix_PlayChannel(-1, mOwner->GetGame()->
		GetSound("Assets/Sounds/ChompLoop.wav"), -1);
	Mix_Pause(mChompSound);
	
	mSirenSound = Mix_PlayChannel(-1, mOwner->GetGame()->
		GetSound("Assets/Sounds/Siren.wav"), -1);
	Mix_Pause(mSirenSound);
	
	mSirenFrightSound = Mix_PlayChannel(-1, mOwner->GetGame()->
		GetSound("Assets/Sounds/SirenFright.wav"), -1);
	Mix_Pause(mSirenFrightSound);
}

void PacManMove::Update(float deltaTime)
{
	// In the process of respawning, don't
	// do anything else
	if (mRespawnTimer > 0.0f)
	{
		UpdateRespawn(deltaTime);
		return;
	}
	
	auto coll = mOwner->GetComponent<CollisionComponent>();
	auto game = mOwner->GetGame();
	
	// Check if we intersect with any ghosts
	for (auto g : game->mGhosts)
	{
		if (!g->IsDead() &&
			coll->Intersect(g->GetComponent<CollisionComponent>()))
		{
			if (g->IsFrightened())
			{
				// Kill ghost
				g->Die();
			}
			else
			{
				// Kill Pac-Man
				StartRespawn();
				return;
			}
		}
	}
	
	// Check if we intersect with a path node
	bool collided = false;
	for (auto p : game->mPathNodes)
	{
		if (coll->Intersect(p->GetComponent<CollisionComponent>()))
		{
			collided = true;
			mPrevNode = p;
			
			HandleNodeIntersect(p);
		}
	}
	
	// Check if we intersect with a power pellet
	for (auto p : game->mPowerPellets)
	{
		if (coll->Intersect(p->GetComponent<CollisionComponent>()))
		{
			for (auto g : game->mGhosts)
			{
				g->Frighten();
			}
			p->SetState(ActorState::Destroy);
		}
	}
	
	// Check if we interesect with a pellet
	for (auto p : game->mPellets)
	{
		if (coll->Intersect(p->GetComponent<CollisionComponent>()))
		{
			p->SetState(ActorState::Destroy);
			mChompSoundTimer = CHOMP_TIME_PER;
		}
	}
	
	// See if we want to switch directions
	if (!Math::NearZero(mMoveDir.x) &&
		!Math::NearZero(mInput.x))
	{
		mMoveDir.x = mInput.x;
	}
	else if (!Math::NearZero(mMoveDir.y) &&
			 !Math::NearZero(mInput.y))
	{
		mMoveDir.y = mInput.y;
	}
	
	// Update position
	Vector2 pos = mOwner->GetPosition();
	pos += mMoveDir * MAX_SPEED * deltaTime;
	
	// Turn smoothing
	// If we collided with a node, smooth us onto the opposite axis
	// of our movement
	// If we aren't moving at all, then smooth to the position of the node
	// (In case we can't move in the desired direction anymore)
	if (collided)
	{
		if (!Math::NearZero(mMoveDir.x))
		{
			pos.y = Math::Lerp(pos.y, mPrevNode->GetPosition().y, 0.25f);
		}
		else if (!Math::NearZero(mMoveDir.y))
		{
			pos.x = Math::Lerp(pos.x, mPrevNode->GetPosition().x, 0.25f);
		}
		else if (Math::NearZero(mMoveDir.Length()))
		{
			pos = Vector2::Lerp(pos, mPrevNode->GetPosition(), 0.25f);
		}
	}
	
	mOwner->SetPosition(pos);
	
	// Now update animations based on movement
	AnimatedSprite* asc = mOwner->GetComponent<AnimatedSprite>();
	if (!Math::NearZero(mMoveDir.Length()))
	{
		asc->SetIsPaused(false);
		if (mMoveDir.y < 0.0f)
		{
			asc->SetAnimation("up");
			mFacingDir = Vector2::NegUnitY;
		}
		else if (mMoveDir.y > 0.0f)
		{
			asc->SetAnimation("down");
			mFacingDir = Vector2::UnitY;
		}
		else if (mMoveDir.x > 0.0f)
		{
			asc->SetAnimation("right");
			mFacingDir = Vector2::UnitX;
		}
		else
		{
			asc->SetAnimation("left");
			mFacingDir = Vector2::NegUnitX;
		}
	}
	else
	{
		asc->SetIsPaused(true);
	}
	
	// Update the ghost siren sound
	UpdateSounds(deltaTime);
}

void PacManMove::ProcessInput(const Uint8* keyState)
{
	mInput = Vector2::Zero;
	if (keyState[SDL_SCANCODE_LEFT])
	{
		mInput = Vector2::NegUnitX;
	}
	else if (keyState[SDL_SCANCODE_RIGHT])
	{
		mInput = Vector2::UnitX;
	}
	else if (keyState[SDL_SCANCODE_UP])
	{
		mInput = Vector2::NegUnitY;
	}
	else if (keyState[SDL_SCANCODE_DOWN])
	{
		mInput = Vector2::UnitY;
	}
}

void PacManMove::HandleNodeIntersect(PathNode* p)
{
	// If we collide with a tunnel, need to teleport
	if (p == mOwner->GetGame()->mTunnelLeft)
	{
		Vector2 newPos = p->GetPosition();
		newPos.x = mOwner->GetGame()->mTunnelRight->GetPosition().x;
		newPos.x -= 16.0f;
		mOwner->SetPosition(newPos);
		mPrevNode = mOwner->GetGame()->mTunnelRight;
		p = mPrevNode;
	}
	else if (p == mOwner->GetGame()->mTunnelRight)
	{
		Vector2 newPos = p->GetPosition();
		newPos.x = mOwner->GetGame()->mTunnelLeft->GetPosition().x;
		newPos.x += 16.0f;
		mOwner->SetPosition(newPos);
		mPrevNode = mOwner->GetGame()->mTunnelLeft;
		p = mPrevNode;
	}

	// Figure out which directions of movement are allowed
	bool leftAllowed = false;
	bool rightAllowed = false;
	bool upAllowed = false;
	bool downAllowed = false;
	
	for (auto n : p->mAdjacent)
	{
		if (n->GetType() != PathNode::Ghost)
		{
			Vector2 diff = n->GetPosition() - p->GetPosition();
			if (diff.x > 1.0f)
			{
				rightAllowed = true;
			}
			else if (diff.x < -1.0f)
			{
				leftAllowed = true;
			}
			else if (diff.y > 1.0f)
			{
				downAllowed = true;
			}
			else if (diff.y < -1.0f)
			{
				upAllowed = true;
			}
		}
	}
	
	// Give priority to turns first
	if (!Math::NearZero(mMoveDir.x) &&
		!Math::NearZero(mInput.y))
	{
		// Switch to up/down, if it's allowed
		if ((mInput.y < 0.0f && upAllowed) ||
			(mInput.y > 0.0f && downAllowed))
		{
			mMoveDir.x = 0.0f;
			mMoveDir.y = mInput.y;
		}
	}
	else if (!Math::NearZero(mMoveDir.y) &&
			 !Math::NearZero(mInput.x))
	{
		// Switch to left/right, if it's allowed
		if ((mInput.x < 0.0f && leftAllowed) ||
			(mInput.x > 0.0f && rightAllowed))
		{
			mMoveDir.x = mInput.x;
			mMoveDir.y = 0.0f;
		}
	}
	// Otherwise just move in input directions
	else if ((mInput.y < 0.0f && upAllowed) ||
			 (mInput.y > 0.0f && downAllowed))
	{
		mMoveDir.x = 0.0f;
		mMoveDir.y = mInput.y;
	}
	else if ((mInput.x < 0.0f && leftAllowed) ||
			 (mInput.x > 0.0f && rightAllowed))
	{
		mMoveDir.x = mInput.x;
		mMoveDir.y = 0.0f;
	}
	
	// Now also verify we can keep moving in the
	// current move direction
	if ((mMoveDir.x < 0.0f && !leftAllowed) ||
		(mMoveDir.x > 0.0f && !rightAllowed))
	{
		mMoveDir.x = 0.0f;
	}
	if ((mMoveDir.y < 0.0f && !upAllowed) ||
		(mMoveDir.y > 0.0f && !downAllowed))
	{
		mMoveDir.y = 0.0f;
	}
}

void PacManMove::StartRespawn(bool isGameIntro)
{
	if (!isGameIntro)
	{
		mRespawnTimer = RESPAWN_TIME;
	}
	else
	{
		mRespawnTimer = INTRO_TIME;
	}
	
	// Zero out move direction
	mMoveDir = Vector2::Zero;
	
	// Start playing death animation if not intro
	if (!isGameIntro)
	{
		AnimatedSprite* asc = mOwner->GetComponent<AnimatedSprite>();
		asc->SetAnimation("death");
		asc->ResetAnimTimer();
		asc->SetIsPaused(false);
		
		Mix_PlayChannel(-1, mOwner->GetGame()->
						GetSound("Assets/Sounds/Death.wav"), 0);
	}
	
	// Pause all ghosts
	for (auto g : mOwner->GetGame()->mGhosts)
	{
		g->SetState(ActorState::Paused);
	}
	
	Mix_Pause(mChompSound);
	Mix_Pause(mSirenSound);
	Mix_Pause(mSirenFrightSound);
}

void PacManMove::UpdateRespawn(float deltaTime)
{
	mRespawnTimer -= deltaTime;
	if (mRespawnTimer <= 0.0f)
	{
		// Reset pac-man position and animation
		PathNode* spawn = static_cast<PacMan*>(mOwner)->GetSpawnNode();
		mOwner->SetPosition(spawn->GetPosition());
		
		AnimatedSprite* asc = mOwner->GetComponent<AnimatedSprite>();
		asc->SetAnimation("right");
		asc->ResetAnimTimer();
		asc->SetIsPaused(true);
		
		// Reset all the ghosts
		for (auto g : mOwner->GetGame()->mGhosts)
		{
			g->SetState(ActorState::Active);
			g->Start();
		}
		
		Mix_Resume(mSirenSound);
	}
}

void PacManMove::UpdateSounds(float deltaTime)
{
	mChompSoundTimer -= deltaTime;
	if (mChompSoundTimer < 0.0f)
	{
		mChompSoundTimer = 0.0f;
	}
	
	bool anyFrightened = false;
	bool anyAlive = false;
	for (auto g : mOwner->GetGame()->mGhosts)
	{
		if (g->IsFrightened())
		{
			anyFrightened = true;
		}
		if (!g->IsDead())
		{
			anyAlive = true;
		}
	}
	
	if (anyFrightened)
	{
		Mix_Pause(mSirenSound);
		Mix_Resume(mSirenFrightSound);
		
		Mix_Pause(mChompSound);
	}
	else if (anyAlive)
	{
		Mix_Resume(mSirenSound);
		Mix_Pause(mSirenFrightSound);
		
		if (mChompSoundTimer > 0.0f)
		{
			Mix_Resume(mChompSound);
		}
		else
		{
			Mix_Pause(mChompSound);
		}
	}
	else
	{
		// They're all dead, no siren for now
		Mix_Pause(mSirenSound);
		Mix_Pause(mSirenFrightSound);
		
		if (mChompSoundTimer > 0.0f)
		{
			Mix_Resume(mChompSound);
		}
		else
		{
			Mix_Pause(mChompSound);
		}
	}
}

