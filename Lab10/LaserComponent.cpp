//
//  LaserComponent.cpp
//  Lab01
//
//  Created by Adrian Reynoso on 4/4/22.
//

#include "LaserComponent.hpp"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "SegmentCast.h"

LaserComponent::LaserComponent(class Actor* owner)
: MeshComponent(owner, true)
{
    SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Draw(class Shader* shader)
{
    if (mMesh)
    {
        // Set the world transform
        shader->SetMatrixUniform("uWorldTransform",
            LaserHelper(lineSegment));
        // Set the active texture
        Texture* t = mMesh->GetTexture(mTextureIndex);
        if (t)
        {
            t->SetActive();
        }
        // Set the mesh's vertex array as active
        VertexArray* va = mMesh->GetVertexArray();
        va->SetActive();
        // Draw
        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}

void LaserComponent::Update(float deltaTime)
{
    //Set the LineSegment member variable’s mStart and mEnd points
    lineSegment.mStart = mOwner->GetPosition();
    lineSegment.mEnd = lineSegment.mStart + (mOwner->GetForward() * 500.0f);
    
    //Check if player collides with lineSegment
    CastInfo castInfo;
    if (SegmentCast((Actor*)mOwner->GetGame()->GetPlayer(), lineSegment, castInfo))
    {
        //Change the line segment’s end point to the mPoint you can get from the CastInfo
        lineSegment.mEnd = castInfo.mPoint;
    }
    else
    {
        //Use the version of SegmentCast that takes in a std::vector<Actor*>
        if (SegmentCast(mOwner->GetGame()->GetBlockVector(), lineSegment, castInfo))
        {
            lineSegment.mEnd = castInfo.mPoint;
        }
    
    }
}

//Returns a world transform matrix that transforms the red laser mesh to start and end at the desired LineSegment points
Matrix4 LaserComponent::LaserHelper(LineSegment m_lineSegment)
{
    //Create scale matrix
    Matrix4 scale = Matrix4::CreateScale(m_lineSegment.Length(), 1.0f, 1.0f);
    
    //Use owners rotation for this computation
    Matrix4 rotationZ = Matrix4::CreateRotationZ(mOwner->GetRotation());
    
    //For the translation, use the center point of the line segment
    Matrix4 translation = Matrix4::CreateTranslation(m_lineSegment.PointOnSegment(0.5f));
    
    //Make the world transform matrix and return it
    Matrix4 mWorldTransform = scale * rotationZ * translation;
    return mWorldTransform;
}
