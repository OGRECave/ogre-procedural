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

#include "ProceduralMeshGenerator.h"
#include "ProceduralPlatform.h"

namespace Procedural
{
/**
 * Builds a rounded box.
 * You can choose the size of the rounded borders to get a sharper or smoother look.
 */
class _ProceduralExport RoundedBoxGenerator : public MeshGenerator<RoundedBoxGenerator>
{
	Ogre::Real sizeX,sizeY,sizeZ;
	int numSegX,numSegY,numSegZ;
	Ogre::Real chamferSize;
	int chamferNumSeg;

	int offset;
	
public:
	RoundedBoxGenerator() : sizeX(1.f), sizeY(1.f), sizeZ(1.f),
		numSegX(1), numSegY(1), numSegZ(1), chamferSize(.1f), chamferNumSeg(8) {}

	/** Sets the size of the box along X axis */
	RoundedBoxGenerator& setSizeX(Ogre::Real sizeX)
	{
		this->sizeX = sizeX;
		return *this;
	}

	/** Sets the size of the box along Y axis */
	RoundedBoxGenerator& setSizeY(Ogre::Real sizeY)
	{
		this->sizeY = sizeY;
		return *this;
	}

	/** Sets the size of the box along Z axis */
	RoundedBoxGenerator& setSizeZ(Ogre::Real sizeZ)
	{
		this->sizeZ = sizeZ;
		return *this;
	}

	/** Sets the number of segments along X axis */
	RoundedBoxGenerator& setNumSegX(int numSegX)
	{
		this->numSegX = numSegX;
		return *this;
	}

	/** Sets the number of segments along Y axis */
	RoundedBoxGenerator& setNumSegY(int numSegY)
	{
		this->numSegY = numSegY;
		return *this;
	}

	/** Sets the number of segments along Z axis */
	RoundedBoxGenerator& setNumSegZ(int numSegZ)
	{
		this->numSegZ = numSegZ;
		return *this;
	}

	/** Sets the size of the chamfer, ie the radius of the rounded part */
	RoundedBoxGenerator& setChamferSize(Ogre::Real chamferSize)
	{
		this->chamferSize = chamferSize;
		return *this;
	}

	/**
	 * Builds the mesh into the given TriangleBuffer
	 * @param buffer The TriangleBuffer on where to append the mesh.
	 */
	void addToTriangleBuffer(TriangleBuffer& buffer) const;
	
	void _addEdge(TriangleBuffer& buffer, short xPos, short yPos, short zPos) const;
	
	void _addCorner(TriangleBuffer& buffer, bool isXPositive, bool isYPositive, bool isZPositive) const;

};


}
#endif
