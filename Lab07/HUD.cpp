#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Player.hpp"

HUD::HUD(Actor* owner)
	:UIComponent(owner)
{
	Renderer* r = owner->GetGame()->GetRenderer();
	mPeppyTextures = std::vector<Texture*>{
		r->GetTexture("Assets/HUD/Peppy0.png"),
		r->GetTexture("Assets/HUD/Peppy1.png"),
	};
	
	mShieldTextures = std::vector<Texture*>{
		r->GetTexture("Assets/HUD/ShieldBar.png"),
		r->GetTexture("Assets/HUD/ShieldBlue.png"),
		r->GetTexture("Assets/HUD/ShieldOrange.png"),
		r->GetTexture("Assets/HUD/ShieldRed.png"),
	};
}

void HUD::Update(float deltaTime)
{
	if (mDoingABarrelRoll)
	{
		if (Mix_Playing(mBarrelRollChannel))
		{
			mPeppyAnimTime += deltaTime * 10;
			while (mPeppyAnimTime >= static_cast<float>(mPeppyTextures.size()))
			{
				mPeppyAnimTime -= static_cast<float>(mPeppyTextures.size());
			}
		}
		else
		{
			mDoingABarrelRoll = false;
		}
	}
}

void HUD::Draw(Shader* shader)
{
	// Show the health meter (based on current health)
	Vector2 healthBarPos = Vector2(-250.0f, 325.0f);
	int hitPoints = GetPlayerHitPoints();
	// Draw orange/middle segment first
	if (hitPoints >= 2)
	{
		DrawTexture(shader, mShieldTextures[2], healthBarPos, 0.75f);
	}
	// Then draw red/left segment
	if (hitPoints >= 1)
	{
		DrawTexture(shader, mShieldTextures[3], healthBarPos, 0.75f);
	}
	// Then the blue/right segment
	if (hitPoints >= 3)
	{
		DrawTexture(shader, mShieldTextures[1], healthBarPos, 0.75f);
	}
	// Now the bar on top
	DrawTexture(shader, mShieldTextures[0], healthBarPos, 0.75f);
	
	// Show the peppy comms animation if doing a barrel roll
	if (mDoingABarrelRoll)
	{
		DrawTexture(shader, mPeppyTextures[static_cast<size_t>(mPeppyAnimTime)], Vector2(-375.0f, -250.0f));
	}
}

void HUD::DoABarrelRoll()
{
	if (!mDoingABarrelRoll)
	{
		mDoingABarrelRoll = true;
		mPeppyAnimTime = 0.0f;
		mBarrelRollChannel = Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/BarrelRoll.wav"), 0);
	}
}

int HUD::GetPlayerHitPoints()
{
	// TODO: Fix this so it correctly returns the corrent hit points (0-3)
	return mOwner->GetGame()->GetPlayer()->shieldLevel;
}
