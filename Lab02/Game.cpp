//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"

// TODO
//Implementation for the functions in our Game class
//Constructor
Game::Game()
{
}

//Implements Initialize function
bool Game::Initialize()
{
    //First, call SDL function to see if game can initialize
    int initializeChecker = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    
    //Return false if there was an error with Initialization
    if (initializeChecker != 0)
    {
        return false;
    }
    
    //Create a window: 1024x768 pixels
    windowPtr = SDL_CreateWindow("Pong Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
    
    //Check if the window could open, if not return false
    if (windowPtr == NULL)
    {
        return false;
    }
    
    //Create a renderer
    rendererPtr = SDL_CreateRenderer(windowPtr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    //Check if the window could open, if not return false
    if (rendererPtr == NULL)
    {
        return false;
    }
    
    //Past this point, we know initialization works
    return true;
    
    //Call LoadData
    LoadData();
}

//Implements Shutdown function
void Game::Shutdown()
{
    //Call UnloadData
    UnloadData();
    
    //Destroy renderer, window, and quit SDL
    SDL_DestroyRenderer(rendererPtr);
    SDL_DestroyWindow(windowPtr);
    atexit(SDL_Quit);
}

//Implements RunLoop function
void Game::RunLoop()
{
    //Run an inifnite loop, calling the respective functions needed IN ORDER
    while (gameIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOuput();
    }
}

//Implements ProcessInput function
void Game::ProcessInput()
{
    //Create an event variable
    SDL_Event event;
    
    //Poll for events
    while (SDL_PollEvent(&event))
    {
        
        //Check what type of event it is
        switch (event.type)
        {
            //If the user wants to quit
            case SDL_QUIT:
                //Switch the gameIsRunnig bool value to false
                gameIsRunning = false;
                break;
        }
    }
    
    //Grab the state of the entire Keyboard
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    
    //Make a copy of the actor vector.
    std::vector<Actor*> mActorsCopy = mActors;
    
    //Loop over the copy of all actors and call ProcessInput on each
    for (unsigned long i = 0; i < mActorsCopy.size(); i++)
    {
        mActorsCopy[i]->ProcessInput(keyboardState);
    }
    
    //Check if escape, up, or down was pressed. If so, set gameIsRunning to false or its respective changes
    if (keyboardState[SDL_SCANCODE_ESCAPE])
    {
        gameIsRunning = false;
    }
    
}

//Implements UpdateGame function
void Game::UpdateGame()
{
    //1) Frame Limit with while loop
    while (previousFrameTime + 16 > SDL_GetTicks())
    {
    }
    
    //2) Calculate delta time (as a float in seconds)
    float currentTime = (float) SDL_GetTicks();
    deltaTime = currentTime - previousFrameTime;
    //Update previousFrameTime
    previousFrameTime = currentTime;
    //Convert the deltaTime milliseconds to seconds
    deltaTime /= 1000;
    
    
    //3) Cap to maximum delta time to no more than 0.033 seconds
    if (deltaTime > 0.033)
    {
        deltaTime = float(0.033);
    }
    //SDL_Log("My deltaTime is %f", deltaTime);
    
    //4) Update all Actors by delta time
    //Make a copy of the actors Vector
    std::vector<Actor*> mActorsCopy = mActors;
    
    //Loop over the copy and call Update on each actor
    for (unsigned long i = 0; i < mActorsCopy.size(); i++)
    {
        mActorsCopy[i]->Update(deltaTime);
    }
    
    //Make a temporary Actor* vector for actors to destroy
    std::vector<Actor*> tempActors;
    
    //Loop over the actor vector, and any actors which are in state ActorState::Destroy should be added to the temporary vector from step 3
    for (unsigned long i = 0; i < mActors.size(); i++)
    {
        if (mActors[i]->actorStateGetter() == ActorState::Destroy)
        {
            //Add to the tempActors vector
            tempActors.push_back(mActors[i]);
        }
    }
    
    //Loop over the vector from step 3 and delete each actor in it (this will automatically call the Actor destructor, which then calls RemoveActor, which removes the actor from the Game’s vector).
    for (unsigned long i = 0; i < tempActors.size(); i++)
    {
        delete mActors[i];
    }

}

//Implements GenerateOuput function
void Game::GenerateOuput()
{
    //Set the render draw color to blue
    SDL_SetRenderDrawColor(rendererPtr, 0, 0, 0, 255);
    
    //Clear the backbuffer
    SDL_RenderClear(rendererPtr);
    
    //DRAW YOUR GAME OBJECTS:
    
    //Present the render
    SDL_RenderPresent(rendererPtr);
}

//ACTOR FUNCTIONS

//Takes in an Actor* and adds it to the vector in Game
void Game::AddActor(Actor* actor)
{
    mActors.push_back(actor);
}

//Takes in an Actor*, removes the actor from the vector
void Game::RemoveActor(Actor* actor)
{
    //use std::find (in <algorithm>) to get an iterator of the Actor*
    auto it = std::find(mActors.begin(), mActors.end(), actor);
    
    //then erase to remove the element the iterator points to
    if (it != mActors.end())
    {
        mActors.erase(it);
    }
}

//DATA FUNCTIONS
void Game::LoadData()
{
    
}

void Game::UnloadData()
{
    //Call delete on all the actors in the vector
    while(mActors.size() > 0)
    {
        delete mActors.back();
    }
}

