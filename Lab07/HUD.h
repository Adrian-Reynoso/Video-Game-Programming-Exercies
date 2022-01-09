#pragma once
#include "Math.h"
#include "UIComponent.h"
#include <vector>

class HUD : public UIComponent
{
public:
	HUD(class Actor* owner);
	
	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	
	void DoABarrelRoll();
private:
	int GetPlayerHitPoints();
	
	std::vector<class Texture*> mShieldTextures;
	std::vector<class Texture*> mPeppyTextures;
	int mBarrelRollChannel = -1;
	bool mDoingABarrelRoll = false;
	float mPeppyAnimTime = 0.0f;
};
