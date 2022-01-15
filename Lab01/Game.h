#pragma once
#include "SDL2/SDL.h"

// TODO
//Create a new Game class with respective public member functions
class Game
{
    public:
        Game();
        bool Initialize();
        void Shutdown();
        void RunLoop();
        //Functions for game loop
        void ProcessInput();
        void UpdateGame();
        void GenerateOuput();
    
    private:
        //Pointer to window and renderer
        SDL_Window* windowPtr;
        SDL_Renderer* rendererPtr;
        bool gameIsRunning = true;
        int paddleDistance = 20;
        SDL_Point gamePaddle {paddleDistance, 384}; //Center coordinates
        SDL_Point gameBall {512, 384}; //Center coordinates
    
        //Drawing constants
        const int wallThickness = 15;
        const int paddleHeight = 110;
        const int paddleWidth = 15;
        const int ballWidth = 15;
    
        //For Update Game
        float previousFrameTime = 0.0;
        float deltaTime;
        bool keyUp = false;
        bool keyDown = false;
        int speed = 700;
        SDL_Point ballVelocity {500, -500};
};
