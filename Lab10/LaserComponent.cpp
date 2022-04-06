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
#include "Block.hpp"

LaserComponent::LaserComponent(class Actor* owner)
: MeshComponent(owner, true)
{
    SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Draw(class Shader* shader)
{
    for (LineSegment lineSegment : lineSegmentVector)
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
}

void LaserComponent::Update(float deltaTime)
{
    //Clear out lineSegment Vector
    lineSegmentVector.clear();

    //Set the LineSegment member variable’s mStart and mEnd points
    LineSegment lineSegment;
    lineSegment.mStart = mOwner->GetPosition();
    lineSegment.mEnd = lineSegment.mStart + (mOwner->GetQuatForward() * 500.0f);

    //Check if player collides with lineSegment
    CastInfo castInfo;
    if (SegmentCast((Actor*)mOwner->GetGame()->GetPlayer(), lineSegment, castInfo))
    {
        //Change the line segment’s end point to the mPoint you can get from the CastInfo
        lineSegment.mEnd = castInfo.mPoint;
        lineSegmentVector.push_back(lineSegment);
    }
    //Check if block collides with lineSegment
    else if (SegmentCast(mOwner->GetGame()->GetBlockVector(), lineSegment, castInfo))
    {
        lineSegment.mEnd = castInfo.mPoint;
        lineSegmentVector.push_back(lineSegment);

        //Check if block is a mirror to then add ANOTHER line segment
        Block* currentBlock = reinterpret_cast <Block*>(castInfo.mActor);
        if (currentBlock->GetIsMirror())
        {
            //Make make the new line segment start point mPoint of castInfo
            LineSegment lineSegment2;
            lineSegment2.mStart = castInfo.mPoint;

            //Make forward vector reflection of previous forward vector line segment
            LineSegment prevLineSeg = lineSegmentVector.back();
            Vector3 prevForward = prevLineSeg.mEnd - prevLineSeg.mStart;
            prevForward.Normalize(); 
            Vector3 newDir = Vector3::Reflect(prevForward, castInfo.mNormal);
            lineSegment2.mEnd = lineSegment2.mStart + (newDir * 500.0f);

            //Push the new lineSegment into the vector
            lineSegmentVector.push_back(lineSegment2);
            
            CastInfo castInfo2;
            //Do an additional SegmentCast ignoring the block actor
            if (SegmentCast(mOwner->GetGame()->GetBlockVector(), lineSegment2, castInfo2, currentBlock))
            {
                //Check if block is a mirror to then add ANOTHER line segment
                Block* currentBlock2 = reinterpret_cast <Block*>(castInfo2.mActor);
                if (currentBlock2->GetIsMirror())
                {
                    //Make a new line segment start point mPoint of castInfo
                    LineSegment lineSegment3;
                    lineSegment3.mStart = castInfo2.mPoint;

                    //Make forward vector reflection of previous forward vector line segment
                    prevForward = lineSegment2.mEnd - lineSegment2.mStart;
                    prevForward.Normalize();
                    newDir = Vector3::Reflect(prevForward, castInfo2.mNormal);
                    lineSegment3.mEnd = lineSegment3.mStart + (newDir * 500.0f);

                    //Push the new lineSegment into the vector
                    lineSegmentVector.push_back(lineSegment3);
                }
            }

        }
    }
    else
    {
        lineSegmentVector.push_back(lineSegment);
    }
    
}

//Returns a world transform matrix that transforms the red laser mesh to start and end at the desired LineSegment points
Matrix4 LaserComponent::LaserHelper(LineSegment m_lineSegment)
{
    //Create scale matrix
    Matrix4 scale = Matrix4::CreateScale(m_lineSegment.Length(), 1.0f, 1.0f);
    
    //Find rotation from Quaternion
    Matrix4 rotation;
    Vector3 oldFacing = Vector3::UnitX;
    oldFacing.Normalize();
    Vector3 newFacing = m_lineSegment.mEnd - m_lineSegment.mStart;
    newFacing.Normalize();
    
    //Check dot product to see if it's 1 or -1
    if (Vector3::Dot(oldFacing, newFacing) == 1)
    {
        rotation = Matrix4::CreateFromQuaternion(Quaternion::Identity);
    }
    else if (Vector3::Dot(oldFacing, newFacing) == -1)
    {
        rotation = Matrix4::CreateFromQuaternion(Quaternion(Vector3::UnitZ, Math::Pi));
    }
    else
    {
        float angle = Math::Acos(Vector3::Dot(oldFacing, newFacing));
        Vector3 axis = Vector3::Cross(oldFacing, newFacing);
        axis.Normalize();
        
        rotation = Matrix4::CreateFromQuaternion(Quaternion(axis, angle));
    }
    
    //For the translation, use the center point of the line segment
    Matrix4 translation = Matrix4::CreateTranslation(m_lineSegment.PointOnSegment(0.5f));
    
    //Make the world transform matrix and return it
    Matrix4 mWorldTransform = scale * rotation * translation;
    return mWorldTransform;
}
