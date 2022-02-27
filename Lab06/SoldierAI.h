#pragma once
#include "SpriteComponent.h"
#include "Math.h"
#include <vector>
#include <string>

class PathNode;

class SoldierAI : public SpriteComponent
{
public:
	SoldierAI(class Actor* owner);
	
	// Setup this soldier for the correct patrol start/end
	// and start the initial movement
	void Setup(PathNode* start, PathNode* end);
	
	void Update(float deltaTime) override;
	
	void Draw(SDL_Renderer* renderer) override;
	
	// TODO: Add any public functions as needed
private:
	// The start path node for the patrol path
	PathNode* mPatrolStart = nullptr;
	// The end path node for the patrol path
	PathNode* mPatrolEnd = nullptr;
	
	// Goal/target we're trying to path to
	PathNode* mTarget = nullptr;
	// The previous node we were at for the current move
	PathNode* mPrev = nullptr;
	// The next node we're moving to for the current move
	PathNode* mNext = nullptr;
	// The rest of the path after next to target
	std::vector<PathNode*> mPath;
	
	// How many pixels/s the soldier movies
	const float SOLDIER_SPEED = 75.0f;
	// How long the soldier gets stunned when hit
	const float STUN_DURATION = 1.0f;
	
	// TODO: Add any private data/functions as needed
    class PathFinder* pathFinder;
    Vector2 currDirection {0.0f, 0.0f};
    class Soldier* mSoldier;
    
    void calculateDirection();
};
