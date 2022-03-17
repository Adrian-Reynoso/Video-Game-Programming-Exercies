//
//  HeightMap.hpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#ifndef HeightMap_hpp
#define HeightMap_hpp

#include <stdio.h>
#include "HeightMap.hpp"
#include <vector>
#include "Math.h"

class HeightMap
{
public:
    HeightMap();
    const Vector3& CellToWorld (int row, int column);
    std::vector<std::vector<int>> intsFromCSV;
    
private:
    bool IsCellOnTrack(int row, int column);
    float GetHeightFromCell(int row, int column);
    
    float cellSize = 40.55f;
    float gridTop = 1280.0f;
    float gridLeft = -1641.0f;
    
    Vector3 ifFalse {0.0f, 0.0f, 0.0f};
    Vector3 returnVector {0.0f, 0.0f, 0.0f};
};

#endif /* HeightMap_hpp */
