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
#ifndef PROCEDURAL_PLANE_GENERATOR_INCLUDED
#define PROCEDURAL_PLANE_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"
#include "ProceduralPlatform.h"

namespace Procedural
{

class _ProceduralExport PlaneGenerator : public MeshGenerator<PlaneGenerator>
{
	int numSegX;
	int numSegY;
	Ogre::Vector3 normal;
	float sizeX;
	float sizeY;
	Ogre::Vector3 position;
public:

	PlaneGenerator(): numSegX(1), numSegY(1),
		normal(Ogre::Vector3::UNIT_Y),
		sizeX(1), sizeY(1),position(Ogre::Vector3::ZERO)
	{}


	void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline PlaneGenerator & setNumSegX(int numSegX)
	{
		this->numSegX = numSegX;
		return *this;
	}

	inline PlaneGenerator & setNumSegY(int numSegY)
	{
		this->numSegY = numSegY;
		return *this;
	}

	inline PlaneGenerator & setNormal(Ogre::Vector3 normal)
	{
		this->normal = normal;
		return *this;
	}

	inline PlaneGenerator & setSizeX(float sizeX)
	{
		this->sizeX = sizeX;
		return *this;
	}

	inline PlaneGenerator & setSizeY(float sizeY)
	{
		this->sizeY = sizeY;
		return *this;
	}

	inline PlaneGenerator & setPosition(Ogre::Vector3 position)
	{
		this->position = position;
		return *this;
	}

};
}
#endif
