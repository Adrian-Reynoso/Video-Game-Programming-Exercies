#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	// TODO: Implement
    //If mAnimName.empty() IS NOT empty
    if (!mAnimName.empty())
    {
        if (mIsPaused == false)
        {
            //Update Animation timer
            mAnimTimer += mAnimFPS * deltaTime;
            
            //Wrap the mAnimTimer if >= the animation FPS
            while (mAnimTimer >= (float)mAnims[mAnimName].size())
            {
                mAnimTimer -= (float)mAnims[mAnimName].size();
            }
            
            //Set the right texture of the animation
            SetTexture(mAnims[mAnimName][(int)mAnimTimer]);
        }
    }
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
