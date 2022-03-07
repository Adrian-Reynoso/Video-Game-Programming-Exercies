//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

int main(int argc, char** argv)
{
    //Create an instance of a game class
    Game gameVar;
    
    //Calling initialize
    bool check = gameVar.Initialize();
    
    //Check if initialize returns true. If so, call RunLoop and Shutdown
    if (check)
    {
        gameVar.RunLoop();
        gameVar.Shutdown();
    }
	return 0;
}
