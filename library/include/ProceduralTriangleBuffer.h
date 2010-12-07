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

#include "OgreMesh.h"

/**
 * This is ogre-procedural's temporary mesh buffer. 
 * It stores all the info needed to build an Ogre Mesh, yet is intented to be more flexible, since
 * there is no link towards hardware.
 */
class TriangleBuffer
{
	std::vector<int> indices;
	std::vector<Ogre::Vector3> vertices;
	
	Ogre::AxisAlignedBox boundingBox;
	Ogre::Real boundingSphereRadius;
	
	int globalOffset;

	//TODO : add UV, normals, tangents, colors?
	
	public:
	TriangleBuffer() : globalOffset(0)
	{}
	
	void rebaseOffset()
	{
		globalOffset = vertices.size();
	}
	
	void updateBoundingSphere(Ogre::Real radius)
	{
		boundingSphereRadius = max(boundingSphereRadius, radius);
	}
	
	void updateBoundingbox(const Ogre::AxisAlignedBoundingBox& aabb)
	{
		Utils::updateAABB(boundingBox, aabb);
	}
	
	/**
	 * Builds an Ogre Mesh from this buffer.
	 */
	Ogre::MeshPtr transformToMesh(Ogre::SceneManager* sceneMgr, const std::string& name)
	{
		Ogre::ManualObject * manual = sceneMgr->createManualObject(name);
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);
		
		for (std::vector<Ogre::Vector3>::iterator it = vertices.begin(); it != vertices.end();it++)
		{
			manual->position(it);
		}
		for (std::vector<int>::iterator it = indices.begin(); it != indices.end();it++)
		{
			manual->index(it);
		}

		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name);

		mesh->_setBounds( boundingBox, false );
		mesh->_setBoundingSphereRadius(boundingSphereRadius);

		unsigned short src, dest;
		if (!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		{
			mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
		}
		
		//TODO :  destroy manualobject???

		return mesh;
	}
	
	inline TriangleBuffer& position(const Ogre::Vector3& pos)
	{
		vertices.push_back(pos);
		return *this;
	}
	
	inline TriangleBuffer& index(int i)
	{
		indices.push_back(globalOffset+i);
		return *this;
	}
};

#endif