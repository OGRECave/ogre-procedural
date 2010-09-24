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
#ifndef PROCEDURAL_ROUNDED_BOX_GENERATOR_INCLUDED
#define PROCEDURAL_ROUNDED_BOX_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"
#include "ProceduralPlatform.h"

namespace Procedural
{
class _ProceduralExport RoundedBoxGenerator : public MeshGenerator<RoundedBoxGenerator>
{
	float sizeX,sizeY,sizeZ;
	int numSegX,numSegY,numSegZ;
	float chamferSize;
	int chamferNumSeg;
	
public:
	RoundedBoxGenerator() : sizeX(1.f), sizeY(1.f), sizeZ(1.f),
		numSegX(1), numSegY(1), numSegZ(1), chamferSize(.1f), chamferNumSeg(8) {}


	RoundedBoxGenerator& setSizeX(float sizeX)
	{
		this->sizeX = sizeX;
		return *this;
	}

	RoundedBoxGenerator& setSizeY(float sizeY)
	{
		this->sizeY = sizeY;
		return *this;
	}

	RoundedBoxGenerator& setSizeZ(float sizeZ)
	{
		this->sizeZ = sizeZ;
		return *this;
	}

	RoundedBoxGenerator& setNumSegX(int numSegX)
	{
		this->numSegX = numSegX;
		return *this;
	}

	RoundedBoxGenerator& setNumSegY(int numSegY)
	{
		this->numSegY = numSegY;
		return *this;
	}

	RoundedBoxGenerator& setNumSegZ(int numSegZ)
	{
		this->numSegZ = numSegZ;
		return *this;
	}

	 RoundedBoxGenerator& setChamferSize(float chamferSize)
	{
		this->chamferSize = chamferSize;
		return *this;
	}

	void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb);
	
	
	void _addEdge(Ogre::ManualObject* manual, int& offset, short xPos, short yPos, short zPos);
	
	void _addCorner(Ogre::ManualObject* manual, int& offset, bool isXPositive, bool isYPositive, bool isZPositive);

};


}
#endif
