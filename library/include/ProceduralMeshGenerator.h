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
#ifndef PROCEDURAL_MESH_GENERATOR_INCLUDED
#define PROCEDURAL_MESH_GENERATOR_INCLUDED

#include "OgreSceneManager.h"
#include "OgreMesh.h"
#include "OgreManualObject.h"
#include "ProceduralRoot.h"
#include "ProceduralPlatform.h"
#include "ProceduralTriangleBuffer.h"

namespace Procedural
{
template <typename T>
class MeshGenerator
{
protected:
	Ogre::SceneManager* sceneMgr;
	Ogre::Real uTile;
	Ogre::Real vTile;

	bool enableNormals;
	unsigned int numTexCoordSet;
public:
	Ogre::MeshPtr realizeMesh(const std::string& name)
	{
		TriangleBuffer tbuffer;
		addToTriangleBuffer(tbuffer);
		Ogre::MeshPtr mesh = tbuffer.transformToMesh(sceneMgr, name);
		return mesh;
	}

	virtual void addToTriangleBuffer(TriangleBuffer& buffer) const=0;

	MeshGenerator() : uTile(1.f),
					  vTile(1.f),
					  enableNormals(true),
					  numTexCoordSet(1)
	{
		sceneMgr = Root::getInstance()->sceneManager;
		assert(sceneMgr && "Scene Manager must be set in Root");
	}

	inline T& setUTile(Ogre::Real uTile)
	{
		this->uTile = uTile;
		return static_cast<T&>(*this);
	}

	inline T & setVTile(Ogre::Real vTile)
	{
		this->vTile = vTile;
		return static_cast<T&>(*this);
	}

	inline T & setEnableNormals(bool enableNormals)
	{
		this->enableNormals = enableNormals;
		return static_cast<T&>(*this);
	}

	inline T & setNumTexCoordSet(unsigned int numTexCoordSet)
	{
		this->numTexCoordSet = numTexCoordSet;
		return static_cast<T&>(*this);
	}

};
//
}
#endif
