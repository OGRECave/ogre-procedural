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
#ifndef PROCEDURAL_BOX_GENERATOR_INCLUDED
#define PROCEDURAL_BOX_GENERATOR_INCLUDED

#include "ProceduralMeshGenerator.h"
#include "ProceduralPlatform.h"

namespace Procedural
{
/**
 * Generates a box mesh
 * Default size is 1.0 with 1 quad per face.
 *
 */
class _ProceduralExport BoxGenerator : public MeshGenerator<BoxGenerator>
{
	Ogre::Real mSizeX,mSizeY,mSizeZ;
	int mNumSegX,mNumSegY,mNumSegZ;
public:
	/// Default constructor
	BoxGenerator() : mSizeX(1.f), mSizeY(1.f), mSizeZ(1.f),
		mNumSegX(1), mNumSegY(1), mNumSegZ(1) {}

	/**
	 * Constructor with arguments
	 */
	BoxGenerator(Ogre::Real sizeX, Ogre::Real sizeY, Ogre::Real sizeZ) : mSizeX(sizeX), mSizeY(sizeY), mSizeZ(sizeZ),
		mNumSegX(1), mNumSegY(1), mNumSegZ(1) {}

	/** Sets size along X axis (default=1) */
	BoxGenerator& setSizeX(Ogre::Real sizeX)
	{
		mSizeX = sizeX;
		return *this;
	}

	/** Sets size along Y axis (default=1) */
	BoxGenerator& setSizeY(Ogre::Real sizeY)
	{
		mSizeY = sizeY;
		return *this;
	}

	/** Sets size along Z axis (default=1) */
	BoxGenerator& setSizeZ(Ogre::Real sizeZ)
	{
		mSizeZ = sizeZ;
		return *this;
	}

	/** Sets the number of segments along X axis (default=1) */
	BoxGenerator& setNumSegX(int numSegX)
	{
		mNumSegX = numSegX;
		return *this;
	}

	/** Sets the number of segments along Y axis (default=1) */
	BoxGenerator& setNumSegY(int numSegY)
	{
		mNumSegY = numSegY;
		return *this;
	}

	/** Sets the number of segments along Z axis (default=1) */
	BoxGenerator& setNumSegZ(int numSegZ)
	{
		mNumSegZ = numSegZ;
		return *this;
	}

	/**
	 * Builds the mesh into the given TriangleBuffer
	 * @param buffer The TriangleBuffer on where to append the mesh.
	 */
	void addToTriangleBuffer(TriangleBuffer& buffer) const;

};


}
#endif
