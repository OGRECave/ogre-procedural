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
#ifndef PROCEDURAL_LATHE_INCLUDED
#define PROCEDURAL_LATHE_INCLUDED

#include "ProceduralShape.h"
#include "ProceduralPlatform.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{
/**
 * Builds a mesh by rotating a shape 360° around Y-axis.
 */
class _ProceduralExport Lathe : public MeshGenerator<Lathe>
{
	Shape* mShapeToExtrude;
	int mNumSeg;

public:
	Lathe(Shape* shapeToExtrude = 0, int numSeg = 16) : mShapeToExtrude(shapeToExtrude), mNumSeg(numSeg)
	{}

	/** Sets the number of segments when rotating around the axis (default=16)*/
	inline Lathe& setNumSeg(int numSeg)
	{
		mNumSeg = numSeg;
		return *this;
	}
	
	/** Sets the shape to extrude */
	inline Lathe & setShapeToExtrude(Shape* shapeToExtrude)
	{
		mShapeToExtrude = shapeToExtrude;
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