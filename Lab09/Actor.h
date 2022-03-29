#pragma once
#include <vector>
#include <SDL2/SDL_stdinc.h>
#include "Math.h"

enum class ActorState
{
	Active,
	Paused,
	Destroy
};

class Actor
{
public:
	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const Uint8* keyState);

	// Getters/setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
    float GetRollAngle() const { return mRollAngle; }
    void SetRollAngle(float rollAngle) { mRollAngle = rollAngle; }
	void SetRotation(float rotation) { mRotation = rotation; }
    const Matrix4& GetWorldTransform() const { return mWorldTransform; }
    
    const Vector3 GetRight() const
    {
        //Get the rotation in radians
        float rotation = GetRotation();
        
        //Calculate the x and y components of the vector
        float vX = cos(rotation + Math::PiOver2);
        float vY = sin(rotation + Math::PiOver2);
        float vZ = 0.0f;
        
        //Return vector
        return Vector3(vX, vY, vZ);
    }
    
    //Returns the forward direction vector
    const Vector3 GetForward() const
    {
        //Get the rotation in radians
        float rotation = GetRotation();
        
        //Calculate the x and y components of the vector
        float vX = cos(rotation);
        float vY = sin(rotation);
        float vZ = 0.0f;
        
        //Return vector
        return Vector3(vX, vY, vZ);
    }
	
	ActorState GetState() const { return mState; }
	void SetState(ActorState state) { mState = state; }

	class Game* GetGame() { return mGame; }
	
	// Returns component of type T, or null if doesn't exist
	template <typename T>
	T* GetComponent() const
	{
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}
		
		return nullptr;
	}
    
    
protected:
	// Any actor-specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void OnProcessInput(const Uint8* keyState);
	
	class Game* mGame;
	// Actor's state
	ActorState mState;

	// Transform
	Vector3 mPosition;
	float mScale;
	float mRotation;
    float mRollAngle = 0.0f;
	
	// Components
	std::vector<class Component*> mComponents;
private:
	friend class Component;
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddComponent(class Component* c);
    
    Matrix4 mWorldTransform;
};
