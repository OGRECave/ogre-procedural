/*
-----------------------------------------------------------------------------
This source file is part of ogre-procedural

For the latest info, see http://code.google.com/p/ogre-procedural/

Copyright (c) 2010 Michael Broutin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef PROCEDURAL_TRIANGLEBUFFER_INCLUDED
#define PROCEDURAL_TRIANGLEBUFFER_INCLUDED

#include "OgreManualObject.h"
#include "OgreMesh.h"
#include "OgreSceneManager.h"
#include "ProceduralUtils.h"
#include "ProceduralRoot.h"

namespace Procedural
{
/**
 * This is ogre-procedural's temporary mesh buffer.
 * It stores all the info needed to build an Ogre Mesh, yet is intented to be more flexible, since
 * there is no link towards hardware.
 */
class TriangleBuffer
{
	std::vector<int> mIndices;

	struct Vertex
	{
		Ogre::Vector3 mPosition;
		Ogre::Vector3 mNormal;
		Ogre::Vector2 mUV;
	};

	std::vector<Vertex> mVertices;
	//std::vector<Vertex>::iterator mCurrentVertex;
	Vertex* mCurrentVertex;
	int mEstimatedVertexCount;
	int mEstimatedIndexCount;
	
	int globalOffset;
	
	public:
		TriangleBuffer() : globalOffset(0), mEstimatedVertexCount(0), mEstimatedIndexCount(0), mCurrentVertex(0)//, mCurrentVertex(mVertices.end())
	{}

	/**
	 * Rebase index offset : call that function before you add a new mesh to the triangle buffer
	 */
	void rebaseOffset()
	{
		globalOffset = mVertices.size();
	}

	/**
	 * Builds an Ogre Mesh from this buffer.
	 */
	Ogre::MeshPtr transformToMesh(const std::string& name,
		const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)
	{
		Ogre::SceneManager* sceneMgr = Root::getInstance()->sceneManager;
		Ogre::ManualObject * manual = sceneMgr->createManualObject();
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		for (std::vector<Vertex>::iterator it = mVertices.begin(); it != mVertices.end();it++)
		{
			manual->position(it->mPosition);
			manual->textureCoord(it->mUV);
			manual->normal(it->mNormal);
		}
		for (std::vector<int>::iterator it = mIndices.begin(); it!=mIndices.end();it++)
		{
			manual->index(*it);
		}
		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name, group);

		unsigned short src, dest;
		if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		{
			mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
		}

		sceneMgr->destroyManualObject(manual);

		return mesh;
	}

	/** Adds a new vertex to the buffer */
	inline TriangleBuffer& position(const Ogre::Vector3& pos)
	{	
		Vertex v;
		v.mPosition = pos;
		mVertices.push_back(v);
		mCurrentVertex = &mVertices.back();
		return *this;
	}

	/** Adds a new vertex to the buffer */
	inline TriangleBuffer& position(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		Vertex v;
		v.mPosition = Ogre::Vector3(x,y,z);
		mVertices.push_back(v);		
		mCurrentVertex = &mVertices.back();
		return *this;
	}
	
	/** Sets the normal of the current vertex */
	inline TriangleBuffer& normal(const Ogre::Vector3& normal)
	{
		mCurrentVertex->mNormal = normal;
		return *this;
	}

	/** Sets the texture coordinates of the current vertex */
	inline TriangleBuffer& textureCoord(float u, float v)
	{
		mCurrentVertex->mUV = Ogre::Vector2(u,v);
		return *this;
	}

	/** Sets the texture coordinates of the current vertex */
	inline TriangleBuffer& textureCoord(const Ogre::Vector2& vec)
	{
		mCurrentVertex->mUV = vec;
		return *this;
	}

	/** 
	 * Adds an index to the index buffer.
	 * Index is relative to the latest rebaseOffset().
	 */
	inline TriangleBuffer& index(int i)
	{
		mIndices.push_back(globalOffset+i);
		return *this;
	}

	/** 
	 * Adds a triangle to the index buffer.
	 * Index is relative to the latest rebaseOffset().
	 */
	inline TriangleBuffer& triangle(int i1, int i2, int i3)
	{
		mIndices.push_back(globalOffset+i1);
		mIndices.push_back(globalOffset+i2);
		mIndices.push_back(globalOffset+i3);
		return *this;
	}
	 
	/**
	 * Gives an estimation of the number of vertices need for this triangle buffer.
	 * If this function is called several times, it means an extra vertices count, not an absolute measure.
	 */
	void estimateVertexCount(unsigned int vertexCount)
	{
		mEstimatedVertexCount += vertexCount;
		mVertices.reserve(mEstimatedVertexCount);
		//Utils::log("estimated vertex count : " + Ogre::StringConverter::toString(vertexCount));
	}

	/**
	 * Gives an estimation of the number of indices needed for this triangle buffer.
	 * If this function is called several times, it means an extra indices count, not an absolute measure.
	 */
	void estimateIndexCount(unsigned int indexCount)
	{
		mEstimatedIndexCount += indexCount;
		mIndices.reserve(mEstimatedIndexCount);
		//Utils::log("estimated index count : " + Ogre::StringConverter::toString(indexCount));
	}

	/*void debugOutput()
	{
		Utils::log("final num vertex : " + Ogre::StringConverter::toString(mVertices.size()) + " - final index size : " + Ogre::StringConverter::toString(mIndices.size()));
	}*/
};
}
#endif
