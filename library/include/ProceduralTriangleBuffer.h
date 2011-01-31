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

namespace Procedural
{
/**
 * This is ogre-procedural's temporary mesh buffer.
 * It stores all the info needed to build an Ogre Mesh, yet is intented to be more flexible, since
 * there is no link towards hardware.
 */
class TriangleBuffer
{
	std::vector<int> indices;
	std::vector<Ogre::Vector3> vertices;
	std::vector<Ogre::Vector3> normals;
	std::vector<Ogre::Vector2> uvs;

	Ogre::AxisAlignedBox boundingBox;
	Ogre::Real boundingSphereRadius;

	int globalOffset;
	
	public:
	TriangleBuffer() : globalOffset(0)
	{}

	/**
	 * Rebase index offset : call that function before you add a new mesh to the triangle buffer
	 */
	void rebaseOffset()
	{
		globalOffset = vertices.size();
	}

	/**
	 * Merge current bounding sphere with another bounding sphere
	 */
	void updateBoundingSphere(Ogre::Real radius)
	{
		boundingSphereRadius = std::max(boundingSphereRadius, radius);
	}

	/**
	 * Merge current bounding box with another bounding box
	 */
	void updateBoundingBox(const Ogre::AxisAlignedBox& aabb)
	{
		boundingBox.merge(aabb);		
	}

	/**
	 * Merge current bounding box with another bounding box
	 */
	void updateBoundingBox(Ogre::Real minX, Ogre::Real minY, Ogre::Real minZ, Ogre::Real maxX, Ogre::Real maxY, Ogre::Real maxZ)
	{
		boundingBox.merge(Ogre::AxisAlignedBox(minX,minY,minZ,maxX,maxY,maxZ));
	}

	/**
	 * Updates the bounding volumes with a vertex.
	 * @param the vertex to add
	 */
	void updateBoundingVolumes(Ogre::Vector3 vec)
	{
		boundingBox.merge(vec);
		Utils::updateBoundingRadius(boundingSphereRadius, vec);
	}

	/**
	 * Builds an Ogre Mesh from this buffer.
	 */
	Ogre::MeshPtr transformToMesh(Ogre::SceneManager* sceneMgr, const std::string& name,
        const Ogre::String& group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)
	{
		Ogre::ManualObject * manual = sceneMgr->createManualObject();
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		std::vector<Ogre::Vector3>::iterator itPos = vertices.begin();
		std::vector<Ogre::Vector2>::iterator itUvs = uvs.begin();
		std::vector<Ogre::Vector3>::iterator itNormals = normals.begin();

		for (; itPos != vertices.end();itPos++,itUvs++,itNormals++)
		{
			manual->position(*itPos);
			manual->textureCoord(*itUvs);
			manual->normal(*itNormals);
		}
		for (std::vector<int>::iterator it = indices.begin(); it!=indices.end();it++)
		{
			manual->index(*it);
		}
		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name, group);

		mesh->_setBounds( boundingBox, false );
		mesh->_setBoundingSphereRadius(boundingSphereRadius);

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
		vertices.push_back(pos);
		return *this;
	}

	/** Adds a new vertex to the buffer */
	inline TriangleBuffer& position(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		vertices.push_back(Ogre::Vector3(x,y,z));
		return *this;
	}
	
	/** Sets the normal of the current vertex */
	inline TriangleBuffer& normal(const Ogre::Vector3& norm)
	{
		normals.push_back(norm);
		return *this;
	}

	/** Sets the texture coordinates of the current vertex */
	inline TriangleBuffer& textureCoord(float u, float v)
	{
		uvs.push_back(Ogre::Vector2(u,v));
		return *this;
	}

	/** Sets the texture coordinates of the current vertex */
	inline TriangleBuffer& textureCoord(const Ogre::Vector2& vec)
	{
		uvs.push_back(vec);
		return *this;
	}

	/** 
	 * Adds an index to the index buffer.
	 * Index is relative to the latest rebaseOffset().
	 */
	inline TriangleBuffer& index(int i)
	{
		indices.push_back(globalOffset+i);
		return *this;
	}
};
}
#endif
