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
#include "ProceduralPlaneGenerator.h"
#include "ProceduralUtils.h"

namespace Procedural
{
void PlaneGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	assert(numSegX>0 && numSegY>0 && "Num seg must be positive");
	assert(!normal.isZeroLength() && "Normal must not be null");
	assert(sizeX>0. && sizeY>0. && "Size must be positive");

	Ogre::Vector3 vX = normal.perpendicular();
	Ogre::Vector3 vY = normal.crossProduct(vX);
	Ogre::Vector3 delta1 = sizeX / numSegX * vX;
	Ogre::Vector3 delta2 = sizeY / numSegY * vY;
	// build one corner of the square
	Ogre::Vector3 orig = -0.5*sizeX*vX - 0.5*sizeY*vY;

	for (int i1 = 0; i1<=numSegX; i1++)
		for (int i2 = 0; i2<=numSegY; i2++)
		{
			manual->position(orig+i1*delta1+i2*delta2+position);
			manual->textureCoord(i1/(Ogre::Real)numSegX*uTile, i2/(Ogre::Real)numSegY*vTile);
			manual->normal(normal);
		}

	bool reverse = false;
	if (delta1.crossProduct(delta2).dotProduct(normal)>0)
		reverse= true;
	for (int n1 = 0; n1<numSegX; n1++)
	{
		for (int n2 = 0; n2<numSegY; n2++)
		{
			if (reverse)
			{
				manual->index(offset+0);
				manual->index(offset+(numSegY+1));
				manual->index(offset+1);
				manual->index(offset+1);
				manual->index(offset+(numSegY+1));
				manual->index(offset+(numSegY+1)+1);
			}
			else
			{
				manual->index(offset+0);
				manual->index(offset+1);
				manual->index(offset+(numSegY+1));
				manual->index(offset+1);
				manual->index(offset+(numSegY+1)+1);
				manual->index(offset+(numSegY+1));
			}
			offset++;
		}
		offset++;
	}
	offset+=numSegY+1;

	///TODO : Bounding volumes
}
}
