//
//  EnemyMove.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/20/22.
//

#ifndef EnemyMove_hpp
#define EnemyMove_hpp

#include <stdio.h>
#include "VehicleMove.hpp"
#include <vector>
class EnemyMove : public VehicleMove
{
public:
    EnemyMove(class Actor* owner);
    void Update(float deltaTime) override;
    std::vector<Vector3> pointsOnRoute;
    
private:
    int nextTargetIndex = 0;
    float pointThreshold = 60.0f;
    float turnThreshold = 1.0f - 0.01f;
    float crossProdMin = 0.0f;
};
#endif /* EnemyMove_hpp */
