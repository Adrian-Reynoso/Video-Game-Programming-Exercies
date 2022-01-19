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
    
    //Check if escape, up, or down was pressed. If so, set gameIsRunning to false or its respective changes
    if (keyboardState[SDL_SCANCODE_ESCAPE])
    {
        gameIsRunning = false;
    }
    else if (keyboardState[SDL_SCANCODE_UP])
    {
        //Switch keyUp bool variable to true
        keyUp = true;
    }
    else if (keyboardState[SDL_SCANCODE_DOWN])
    {
        //Switch keyDown bool variable to true
        keyDown = true;
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
    
    //4) Update all game objects by delta time
    //Move the paddles
    if (keyUp == true)
    {
        gamePaddle.y = int (gamePaddle.y - (speed * deltaTime));
        keyUp = false;
    }
    if (keyDown == true)
    {
        gamePaddle.y = int (gamePaddle.y + (speed * deltaTime));
        keyDown = false;
    }
    //Check if position goes off screen. If so, fix it
    if (gamePaddle.y <= wallThickness + (paddleHeight/2))
    {
        gamePaddle.y = wallThickness + (paddleHeight/2);
    }
    else if (gamePaddle.y >= 768 - (wallThickness + (paddleHeight/2)))
    {
        gamePaddle.y = 768 - (wallThickness + (paddleHeight/2));
    }
    
    //For ball
    //Calculate position based off velocity
    gameBall.x += int(ballVelocity.x * deltaTime);
    gameBall.y += int(ballVelocity.y * deltaTime);
    
    //Check if ball hits one of the walls. If so, make correct adjustments
    // 1. For top wall
    if (gameBall.y <= wallThickness + (ballWidth/2))
    {
        ballVelocity.y *= -1;
        gameBall.y = wallThickness + (ballWidth/2);
    }
    //2. For bottom wall
    if (gameBall.y >= 768 - (wallThickness + (ballWidth/2)))
    {
        ballVelocity.y *= -1;
        gameBall.y = 768 - (wallThickness + (ballWidth/2));
    }
    //3. For Right Wall
    if (gameBall.x >= 1024 - (wallThickness + (ballWidth/2)))
    {
        ballVelocity.x *= -1;
        gameBall.x = 1024 - (wallThickness + (ballWidth/2));
    }
    
    //Check if the ball hits the paddle. If so, make correct adjustments
    if (gameBall.x <= paddleDistance + paddleWidth/2 + (ballWidth/2) && (gameBall.y <= gamePaddle.y + (paddleHeight/2) && gameBall.y >= gamePaddle.y - (paddleHeight/2)))
    {
        ballVelocity.x *= -1;
        gameBall.x = paddleDistance + paddleWidth/2 + (ballWidth/2);
    }
    
    //Check if player lost. If so, quit game
    if (gameBall.x <= ballWidth/2 * -1)
    {
        gameIsRunning = false;
    }
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

