#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include "Math.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>


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
        
        //Functions for Actors
        void AddActor(class Actor* actor);
        void RemoveActor(class Actor* actor);
        void AddSprite(class SpriteComponent* sprite);
        void RemoveSprite(class SpriteComponent* sprite);
        SDL_Texture* GetTexture(std::string fileName);
        const float SCREENHEIGHT = 448.0f;
        const float SCREENWIDTH = 600.0f;
    
        //Public Functions for blocks and Goombas
        void AddBlock(class Block* block);
        void RemoveBlock(class Block* block);
        void AddGoomba(class Goomba* goomba);
        void RemoveGoomba(class Goomba* goomba);
        Mix_Chunk* GetSound(const std::string& filename);
        int soundMusicLoop;
        const std::vector<class Block*> GetBlockVector()&
        {
            return blockVector;
        }
        const std::vector<class Goomba*> GetGoombaVector()&
        {
            return goombaVector;
        }
        Vector2 cameraPosition {0.0f, 0.0f};
    
        //Public Functions for player
        class Player* GetPlayer()
        {
            return player;
        }
    
    private:
        //Pointer to window and renderer
        SDL_Window* windowPtr;
        SDL_Renderer* rendererPtr;
        bool gameIsRunning = true;
        
        //For Update Game
        float previousFrameTime = 0.0;
        float deltaTime;
    
        //Actors
        std::vector<class Actor*> mActors;
    
        //LoadData and UnloadData private Functions
        void LoadData();
        void UnloadData();
    
        //Sprite Functions
        std::unordered_map<std::string, SDL_Texture*> mHashmap;
        std::vector<class SpriteComponent*> spriteCompVector;
    
        //Function to read in a text file
        void readFile(std::string fileName);
    
        //Vector for Blocks and enemies
        std::vector<class Block*> blockVector;
        std::vector<class Goomba*> goombaVector;
    
        //Member variable for player and spawner
        class Player* player;
    
        //For sound effects
        std::unordered_map<std::string, Mix_Chunk*> mSoundMap;
};
