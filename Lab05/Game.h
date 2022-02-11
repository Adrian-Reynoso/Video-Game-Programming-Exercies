#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);
	Mix_Chunk* GetSound(const std::string& fileName);

	void LoadLevel(const std::string& fileName);
	void LoadPaths(const std::string& fileName);
	
	std::vector<class PathNode*> mPathNodes;
	std::vector<class PowerPellet*> mPowerPellets;
	std::vector<class Pellet*> mPellets;
	class PacMan* mPlayer = nullptr;
	class PathNode* mTunnelLeft = nullptr;
	class PathNode* mTunnelRight = nullptr;
	class PathNode* mGhostPen = nullptr;
    
	// Start out supporting only ONE ghost (Blinky)
	// Update this as the lab instructs you to
    static const int GHOST_COUNT = 4;
    class Ghost* mGhosts[GHOST_COUNT] = { nullptr };
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void DebugDrawPaths();
	void LoadData();
	void UnloadData();
	void DoGameIntro();
	void DoGameWin();

	// Map of textures loaded
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;

	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;
	
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount = 0;
	bool mIsRunning;
	
	bool mShowGraph = false;
	bool mShowGhostPaths = true;
	bool mPrev1Input = false;
	bool mPrev2Input = false;
};
