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
#include "ProceduralLathe.h"

namespace Procedural
{
	void Lathe::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
	{
		assert( shapeToExtrude && "Shape must not be null!");
		int numSegShape = shapeToExtrude->getSegCount();
		assert(numSegShape>1 && "Shape must contain at least two points");

		for (int i=0;i<=numSeg;i++)
		{
			Ogre::Real angle = i/(Ogre::Real)numSeg*Ogre::Math::TWO_PI;
			Ogre::Quaternion q;
			q.FromAngleAxis((Ogre::Radian)angle,Ogre::Vector3::UNIT_Y);

			for (int j=0;j<=numSegShape;j++)
			{
				Ogre::Vector2 v0 = shapeToExtrude->getPoint(j);
				Ogre::Vector3 vp(v0.x,v0.y,0);
				Ogre::Vector2 vp2direction = shapeToExtrude->getAvgDirection(j);
				Ogre::Vector2 vp2normal = vp2direction.perpendicular();
				Ogre::Vector3 normal(vp2normal.x, vp2normal.y, 0);
				normal.normalise();
				if (shapeToExtrude->getOutSide() == SIDE_LEFT)
				{
					normal = -normal;
				}

				manual->position(q*vp);
				manual->normal(q*normal);
				manual->textureCoord(i/(Ogre::Real)numSeg*uTile, j/(Ogre::Real)numSegShape*vTile);

				if (j <numSegShape && i <numSeg)
				{
					manual->index(offset + numSegShape + 2);
					manual->index(offset);
					manual->index(offset + numSegShape + 1);
					manual->index(offset + numSegShape + 2);
					manual->index(offset + 1);
					manual->index(offset);
				}
				offset ++;
			}
		}
	}
}
