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
#include "ProceduralStableHeaders.h"
#include "ProceduralLathe.h"

using namespace Ogre;

namespace Procedural
{
void Lathe::addToTriangleBuffer(TriangleBuffer& buffer) const
	{
		assert( mShapeToExtrude && "Shape must not be null!");
		int numSegShape = mShapeToExtrude->getSegCount();
		assert(numSegShape>1 && "Shape must contain at least two points");
		int offset =0;

		buffer.rebaseOffset();
		buffer.estimateIndexCount(mNumSeg*numSegShape*6);
		buffer.estimateVertexCount((numSegShape+1)*(mNumSeg+1));

		for (int i=0;i<=mNumSeg;i++)
		{
			Real angle = i/(Real)mNumSeg*Math::TWO_PI;
			Quaternion q;
			q.FromAngleAxis((Radian)angle,Vector3::UNIT_Y);

			for (int j=0;j<=numSegShape;j++)
			{
				Vector2 v0 = mShapeToExtrude->getPoint(j);
				Vector3 vp(v0.x,v0.y,0);
				Vector2 vp2direction = mShapeToExtrude->getAvgDirection(j);
				Vector2 vp2normal = vp2direction.perpendicular();
				Vector3 normal(vp2normal.x, vp2normal.y, 0);
				normal.normalise();
				if (mShapeToExtrude->getOutSide() == SIDE_LEFT)
				{
					normal = -normal;
				}

				addPoint(buffer, q*vp,
								 q*normal,
								 Vector2(i/(Real)mNumSeg, j/(Real)numSegShape));

				if (j <numSegShape && i <mNumSeg)
				{
					buffer.index(offset + numSegShape + 2);
					buffer.index(offset);
					buffer.index(offset + numSegShape + 1);
					buffer.index(offset + numSegShape + 2);
					buffer.index(offset + 1);
					buffer.index(offset);
				}
				offset ++;
			}
		}		
	}
}
