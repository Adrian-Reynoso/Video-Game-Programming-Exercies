//
//  HeightMap.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 3/15/22.
//

#include "HeightMap.hpp"
#include <string>
#include <fstream>
#include "CSVHelper.h"
#include "SDL2/SDL.h"

HeightMap::HeightMap()
{
    //Create your ifstream
    std::string fileName = "Assets/HeightMap/HeightMap.csv";
    std::ifstream filein(fileName);
    
    //Make a for-loop that goes through the file and takes each line
    while (filein)
    {
        //Use the CSVHelper::split function to turn the
        std::string line;
        std::getline(filein, line);
        std::vector<std::string> temp;
        
        //Check if string is empty, if so don't try to split it
        if (!line.empty())
        {
            temp = CSVHelper::Split(line);
            std::vector<int> temp2;
            
            //Turn the string in each vector to an integer in the new temp2 vector
            for (std::string stringNum : temp)
            {
                temp2.push_back(std::stoi(stringNum));
            }
            
            //Push this vector into the intsFromCSV member variable vector
            intsFromCSV.push_back(temp2);
        }
    }
    
}

bool HeightMap::IsCellOnTrack(int row, int column)
{
    //Returns true if that cell is on the track and false if otherwise
    if (row >= (int)intsFromCSV.size() || row < 0)
    {
        return false;
    }
    else if (column >= (int)intsFromCSV[row].size() || column < 0)
    {
        return false;
    }
    else if (intsFromCSV[row][column] == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

float HeightMap::GetHeightFromCell(int row, int column)
{
    //Returns a float value corresponding to the height of the cell
    return -40.0f + intsFromCSV[row][column] * 5.0f;
}

const Vector3& HeightMap::CellToWorld(int row, int column)
{
    //If the cell is not on the track
    if (IsCellOnTrack(row, column) == false)
    {
        return ifFalse;
    }

    returnVector.x = gridTop - cellSize * row;
    returnVector.y = gridLeft + cellSize * column;
    returnVector.z = GetHeightFromCell(row, column);
    return returnVector;
}

Vector2 HeightMap::WorldToCell(const Vector2& pos)
{
    //Returns the row/col of the cell corresponding to the x, y coordinate
    Vector2 rowCol;
    rowCol.x = (Math::Abs(pos.x - gridTop) + cellSize / 2.0f) / cellSize;
    rowCol.y = (Math::Abs(pos.y - gridLeft) + cellSize / 2.0f) / cellSize;
    return rowCol;
}

bool HeightMap::IsOnTrack(const Vector2& pos)
{
    //Returns true if the cell of that (x, y) coordinate is on the track, false if not
    Vector2 temp = WorldToCell(pos);
    return IsCellOnTrack((int)temp.x, (int)temp.y);
}

int HeightMap::GetHeight(const Vector2& pos)
{
    //Returns the height of the cell at that coordinate, or -1000 if that cell is not on the track
    Vector2 temp = WorldToCell(pos);
    
    if (!IsCellOnTrack((int)temp.x, (int)temp.y))
    {
        return -1000;
    }
    else
    {
        return (int) GetHeightFromCell((int)temp.x, (int)temp.y);
    }
}
