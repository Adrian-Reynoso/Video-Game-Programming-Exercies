//
//  LaserMine.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/4/22.
//

#ifndef LaserMine_hpp
#define LaserMine_hpp

#include <stdio.h>
#include "Actor.h"

class LaserMine : public Actor
{
    public:
    LaserMine(class Game* game);
    class LaserComponent *laserComponent;
};
#endif /* LaserMine_hpp */
