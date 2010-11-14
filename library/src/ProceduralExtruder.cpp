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
#include "ProceduralExtruder.h"

namespace Procedural
{
	void Extruder::addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::AxisAlignedBox& aabb)
	{
		assert(extrusionPath && shapeToExtrude && "Shape and Path must not be null!");
		int numSegPath = extrusionPath->getSegCount();
		int numSegShape = shapeToExtrude->getSegCount();
		assert(numSegPath>1 && numSegShape>1 && "Shape and path must contain at least two points");

	for (int i = 0; i <= numSegPath;i++)
	{
		const Ogre::Vector3& v0 = extrusionPath->getPoint(i);

		Ogre::Vector3 direction = extrusionPath->getAvgDirection(i);

		// First, compute an approximate quaternion (everything is ok except Roll angle)
		Ogre::Quaternion quat = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
		// Then, compute a correction quaternion : we want the "up" direction to be always the same
		Ogre::Vector3 projectedY = Ogre::Vector3::UNIT_Y - Ogre::Vector3::UNIT_Y.dotProduct(direction) * direction;
		Ogre::Vector3 tY = quat * Ogre::Vector3::UNIT_Y;
		Ogre::Quaternion quat2 = tY.getRotationTo(projectedY);
		Ogre::Quaternion q = quat2 * quat;

		for (int j =0;j<=numSegShape;j++)
		{
			Ogre::Vector2 vp2 = shapeToExtrude->getPoint(j);
			Ogre::Vector2 vp2direction = shapeToExtrude->getAvgDirection(j);
			Ogre::Vector2 vp2normal = vp2direction.perpendicular();
			Ogre::Vector3 vp(vp2.x, vp2.y, 0);
			Ogre::Vector3 normal(vp2normal.x, vp2normal.y, 0);
			normal.normalise();

			if (shapeToExtrude->getOutSide() == SIDE_LEFT)
			{
				normal = -normal;
			}

            Ogre::Vector3 newPoint = v0+q*vp;
			manual->position(newPoint);
			Utils::updateAABB(aabb, newPoint);
			Utils::updateBoundingRadius(boundingRadius, newPoint);
			manual->normal(q*normal);
			manual->textureCoord(i/(Ogre::Real)numSegPath*uTile, j/(Ogre::Real)numSegShape*vTile);

			if (j <numSegShape && i <numSegPath)
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
