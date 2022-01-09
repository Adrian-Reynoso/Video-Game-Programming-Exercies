#pragma once
#include "Actor.h"
#include <vector>

class PathNode : public Actor
{
public:
	enum Type {
		Default,
		Ghost,
		Tunnel
	};
	PathNode(class Game* game, Type type);
	
	Type GetType() const { return mType; }
	
	// An id for the node, to aid in debugging.
	int number = 0;
    
	std::vector<PathNode*> mAdjacent;
private:
	Type mType;
};
