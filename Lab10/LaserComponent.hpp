//
//  LaserComponent.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/4/22.
//

#ifndef LaserComponent_hpp
#define LaserComponent_hpp

#include <stdio.h>
#include <vector>
#include "MeshComponent.h"
#include "SegmentCast.h"

class LaserComponent : public MeshComponent
{
    public:
    LaserComponent(class Actor* owner);
    void Draw(class Shader* shader) override;
    void Update(float deltaTime) override;
    Matrix4 LaserHelper(LineSegment lineSegement);
    
    private:
    std::vector<LineSegment> lineSegmentVector;
    bool hitMirror = false;
};
#endif /* LaserComponent_hpp */
