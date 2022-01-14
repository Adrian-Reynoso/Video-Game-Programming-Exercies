//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

// TODO
//Implementation for the functions in our Game class
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
}

//Implements Shutdown function
void Game::Shutdown()
{
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
    
    //Check if escape was pressed. If so, set gameIsRunning to false
    if (keyboardState[SDL_SCANCODE_ESCAPE])
    {
        gameIsRunning = false;
    }
    
}

//Implements UpdateGame function
void Game::UpdateGame()
{
    
}

//Implements GenerateOuput function
void Game::GenerateOuput()
{
    //Set the render draw color to blue
    SDL_SetRenderDrawColor(rendererPtr, 0, 0, 255, 255);
    
    //Clear the backbuffer
    SDL_RenderClear(rendererPtr);
    
    //DRAW YOUR GAME OBJECTS:
    //1. Three White Walls
    //Create the 3 rectangles
    SDL_Rect gameWallTop {0, 0, 1024, wallThickness};
    SDL_Rect gameWallBottom {0, 768 - wallThickness, 1024, wallThickness};
    SDL_Rect gameWallRight {1024 - wallThickness, 0, wallThickness, 768};
    
    SDL_SetRenderDrawColor(rendererPtr, 255, 255, 255, 255); //Set Render Color to White
    SDL_RenderFillRect(rendererPtr, &gameWallTop); //Draw Walls
    SDL_RenderFillRect(rendererPtr, &gameWallBottom);
    SDL_RenderFillRect(rendererPtr, &gameWallRight);
    
    //2. Game Paddle
    //Create an SDL_Rect with the Paddles dimensions so that it's x/y coordinates
    // are its center and not top right
    SDL_Rect gamePaddleRect {gamePaddle.x - paddleWidth/2, gamePaddle.y - paddleHeight/2, paddleWidth, paddleHeight};
    //Fill in the rectangle
    SDL_RenderFillRect(rendererPtr, &gamePaddleRect);
    
    //3. Game Ball
    //Create an SDL_Rect with the balls dimensions so that it's x/y coordinates
    // are its center and not top right
    SDL_Rect gameBallRect {gameBall.x - ballWidth/2, gameBall.y - ballWidth/2, ballWidth, ballWidth};
    //Fill in the rectangle
    SDL_RenderFillRect(rendererPtr, &gameBallRect);
    
    //Present the render
    SDL_RenderPresent(rendererPtr);
}

