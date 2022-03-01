#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include "Math.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>


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
        const float SCREENWIDTH = 512.0f;
    
        Mix_Chunk* GetSound(const std::string& filename);
        int soundMusicLoop;
    
        //Public Functions for player and camera
        class Player* GetPlayer()
        {
            return player;
        }
        Vector2& GetCameraPos()
        {
            return cameraPosition;
        }
        Vector2 cameraPosition {0.0f, 0.0f};
        const std::vector<class Collider*> &GetColliderVector()
        {
            return colliderVector;
        }
        class PathFinder* GetPathFinder()
        {
            return pathFinder;
        }
        const std::vector<class EnemyComponent*> &GetEnemyComponentVector()
        {
            return enemyComponentVector;
        }
        void AddEnemyComponent(class EnemyComponent* enemy)
        {
            enemyComponentVector.push_back(enemy);
        }
        void RemoveEnemyComponent(class EnemyComponent* enemy);
    
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
    
        //Member variable for player and spawner
        class Player* player;
    
        //For sound effects
        std::unordered_map<std::string, Mix_Chunk*> mSoundMap;
    
        //For loading objects
        void LoadInObjects();
        std::vector<class Collider*> colliderVector;
    
        class PathFinder* pathFinder;
    
        //For enemies
        std::vector<class EnemyComponent*> enemyComponentVector;
};
